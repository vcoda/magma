/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "pch.h"
#pragma hdrstop
#include "blitRectangle.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/framebuffer.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/shaderModule.h"
#include "../objects/sampler.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"
#include "../objects/pipelineLayout.h"
#include "../objects/pipeline.h"
#include "../objects/commandBuffer.h"
#include "../shaders/shaderStages.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../states/samplerState.h"
#include "../misc/exception.h"

namespace magma
{
namespace aux
{
BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BlitRectangle(renderPass,
        VertexShaderStage(createVertexShader(renderPass->getDevice(), allocator), "main"),
        FragmentShaderStage(createFragmentShader(renderPass->getDevice(), allocator), "main"),
        std::move(pipelineCache), std::move(allocator))
{}

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass,
    const PipelineShaderStage& vertexShader, const PipelineShaderStage& fragmentShader,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    renderPass(std::move(renderPass))
{   // Check for hardware support
    std::shared_ptr<Device> device = this->renderPass->getDevice();
    std::shared_ptr<PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    // Descriptor set for single image view in fragment shader
    const Descriptor imageSampler(descriptors::CombinedImageSampler(1));
    descriptorPool = std::make_shared<DescriptorPool>(device, 1, std::vector<Descriptor>{imageSampler}, false, allocator);
    descriptorSetLayout = std::make_shared<DescriptorSetLayout>(device, bindings::FragmentStageBinding(0, imageSampler), 0, allocator);
    descriptorSet = descriptorPool->allocateDescriptorSet(descriptorSetLayout);
    // Create texture samplers
    nearestSampler = std::make_shared<Sampler>(device, samplers::magMinMipNearestClampToEdge, allocator);
    bilinearSampler = std::make_shared<Sampler>(device, samplers::magMinLinearMipNearestClampToEdge, allocator);
#ifdef VK_EXT_filter_cubic
    if (physicalDevice->checkExtensionSupport("VK_IMG_filter_cubic") || physicalDevice->checkExtensionSupport("VK_EXT_filter_cubic"))
        cubicSampler = std::make_shared<Sampler>(device, samplers::magCubicMinLinearMipNearestClampToEdge, allocator);
#endif
    // Create blit pipeline
    pipelineLayout = std::make_shared<PipelineLayout>(descriptorSetLayout, std::initializer_list<PushConstantRange>{}, allocator);
    pipeline = std::make_shared<GraphicsPipeline>(device,
        std::vector<PipelineShaderStage>{vertexShader, fragmentShader},
        renderstates::nullVertexInput,
        renderstates::triangleList,
#ifdef VK_NV_fill_rectangle
        physicalDevice->checkExtensionSupport("VK_NV_fill_rectangle") ? renderstates::fillRectangleCullNoneCCW :
#endif
        renderstates::fillCullNoneCCW,
        renderstates::noMultisample,
        renderstates::depthAlwaysDontWrite,
        renderstates::dontBlendRgba,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        pipelineLayout,
        this->renderPass, 0,
        std::move(pipelineCache),
        nullptr, // basePipeline
        std::move(allocator));
    for (const auto& attachment : this->renderPass->getAttachments())
    {
        if (VK_ATTACHMENT_LOAD_OP_CLEAR == attachment.loadOp)
        {   // The Vulkan spec states: clearValueCount must be greater than the largest attachment index in renderPass that specifies a loadOp
            // (or stencilLoadOp, if the attachment has a depth/stencil format) of VK_ATTACHMENT_LOAD_OP_CLEAR.
            clearValues = std::vector<ClearValue>(this->renderPass->getAttachments().size(), clears::blackColor);
            break;
        }
    }
}

void BlitRectangle::blit(std::shared_ptr<Framebuffer> bltDst, std::shared_ptr<const ImageView> bltSrc,
    std::shared_ptr<CommandBuffer> cmdBuffer, VkFilter filter,
    bool negativeViewportHeight /* false */, const char *labelName /* nullptr */,
    uint32_t labelColor /* 0xFFFFFFFF */) const noexcept
{
    MAGMA_ASSERT(bltDst);
    MAGMA_ASSERT(bltSrc);
    MAGMA_ASSERT(cmdBuffer);
    if (bltSrc != prevBltSrc || filter != prevFilter)
    {
        descriptorSet->update(0, bltSrc,
            (VK_FILTER_NEAREST == filter) ? nearestSampler :
            ((VK_FILTER_LINEAR == filter) ? bilinearSampler : cubicSampler));
        prevBltSrc = bltSrc;
        prevFilter = filter;
    }
    cmdBuffer->setRenderArea(0, 0, bltDst->getExtent());
    if (labelName)
        cmdBuffer->beginRenderPass(renderPass, bltDst, clearValues, labelName, labelColor);
    else
        cmdBuffer->beginRenderPass(renderPass, bltDst, clearValues);
    {
        const uint32_t width = bltDst->getExtent().width;
        const int32_t height = static_cast<int32_t>(bltDst->getExtent().height);
        cmdBuffer->setViewport(0, 0, width, negativeViewportHeight ? -height : height);
        cmdBuffer->setScissor(Scissor(0, 0, bltDst->getExtent()));
        cmdBuffer->bindDescriptorSet(pipeline, descriptorSet);
        cmdBuffer->bindPipeline(pipeline);
        cmdBuffer->draw(3);
    }
    cmdBuffer->endRenderPass();
}

std::shared_ptr<ShaderModule> BlitRectangle::createVertexShader(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator) const
{   // https://www.khronos.org/registry/OpenGL/extensions/NV/NV_fill_rectangle.txt;
    if (device->getPhysicalDevice()->checkExtensionSupport("VK_NV_fill_rectangle"))
    {
#include "spirv/output/blitv_nv"
        return std::make_shared<ShaderModule>(std::move(device), vsBlitNV, 0, nullptr, std::move(allocator));
    }
#include "spirv/output/blitv"
    return std::make_shared<ShaderModule>(std::move(device), vsBlit, 0, nullptr, std::move(allocator));
}

std::shared_ptr<ShaderModule> BlitRectangle::createFragmentShader(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator) const
{
#include "spirv/output/blitf"
    return std::make_shared<ShaderModule>(std::move(device), fsBlit, 0, nullptr, std::move(allocator));
}
} // namespace aux
} // namespace magma
