/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "fillRectangleVertexShader.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/framebuffer.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/shaderModule.h"
#include "../objects/sampler.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"
#include "../objects/graphicsPipeline.h"
#include "../objects/pipelineLayout.h"
#include "../objects/commandBuffer.h"
#include "../shaders/shaderStages.h"
#include "../shaders/shaderReflection.h"
#include "../shaders/specialization.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../states/samplerState.h"
#include "../core/constexprHash.h"

namespace magma
{
namespace aux
{
constexpr
#include "spirv/output/blitf"

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BlitRectangle(renderPass,
        std::make_shared<ShaderModule>(renderPass->getDevice(), fsBlit, core::hashArray(fsBlit), allocator, 0, true),
        nullptr, // No specialization
        std::move(pipelineCache), std::move(allocator))
{}

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<ShaderModule> fragmentShader,
    std::shared_ptr<Specialization> specialization /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    renderPass(std::move(renderPass))
{
    std::shared_ptr<Device> device = this->renderPass->getDevice();
    std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    constexpr uint32_t maxDescriptorSets = 10;
    descriptorPool = std::make_shared<DescriptorPool>(device, maxDescriptorSets,
        descriptors::CombinedImageSampler(maxDescriptorSets), allocator);
    const SetLayout setLayout;
    std::shared_ptr<DescriptorSet> descriptorSet = std::make_shared<DescriptorSet>(descriptorPool, 0, setLayout, VK_SHADER_STAGE_FRAGMENT_BIT, allocator);
    // Create texture samplers
    nearestSampler = std::make_shared<Sampler>(device, samplers::magMinMipNearestClampToEdge, allocator);
    bilinearSampler = std::make_shared<Sampler>(device, samplers::magMinLinearMipNearestClampToEdge, allocator);
    // Check for cubic filtering support
#ifdef VK_EXT_filter_cubic
    bool hasCubicFilter = device->extensionEnabled(VK_EXT_FILTER_CUBIC_EXTENSION_NAME);
    #ifdef VK_IMG_filter_cubic
    hasCubicFilter |= device->extensionEnabled(VK_IMG_FILTER_CUBIC_EXTENSION_NAME);
    #endif
    if (hasCubicFilter)
        cubicSampler = std::make_shared<Sampler>(device, samplers::magCubicMinLinearMipNearestClampToEdge, allocator);
#endif // VK_EXT_filter_cubic
    // Load fullscreen vertex shader
    auto vertexShader = std::make_unique<FillRectangleVertexShader>(device, allocator);
    const std::vector<PipelineShaderStage> shaderStages = {
        VertexShaderStage(vertexShader->getShader(), vertexShader->getEntryPointName()),
        FragmentShaderStage(fragmentShader, fragmentShader->getReflection() ? fragmentShader->getReflection()->getEntryPointName(0) : "main", std::move(specialization))
    };
    const VkSampleCountFlagBits samples = this->renderPass->getAttachments().front().samples;
    const MultisampleState multisampleState = 
        (samples & VK_SAMPLE_COUNT_2_BIT) ? renderstates::multisample2 :
        (samples & VK_SAMPLE_COUNT_4_BIT) ? renderstates::multisample4 : 
        (samples & VK_SAMPLE_COUNT_8_BIT) ? renderstates::multisample8 :
        (samples & VK_SAMPLE_COUNT_16_BIT) ? renderstates::multisample16 :
        (samples & VK_SAMPLE_COUNT_32_BIT) ? renderstates::multisample32 :
        (samples & VK_SAMPLE_COUNT_64_BIT) ? renderstates::multisample64 :
        renderstates::dontMultisample;
    // Create blit pipeline
    auto pipelineLayout = std::make_shared<PipelineLayout>(descriptorSet->getLayout(), allocator);
    pipeline = std::make_shared<GraphicsPipeline>(std::move(device),
        shaderStages,
        renderstates::nullVertexInput,
        renderstates::triangleList,
        vertexShader->getRasterizationState(),
        multisampleState,
        renderstates::depthAlwaysDontWrite,
        renderstates::dontBlendRgba,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        std::move(pipelineLayout),
        this->renderPass, 0,
        std::move(allocator),
        std::move(pipelineCache),
        nullptr); // basePipeline
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

void BlitRectangle::blit(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const ImageView> imageView, VkFilter filter, const VkRect2D& rc,
    bool negativeViewportHeight /* false */) const noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(cmdBuffer);
    std::shared_ptr<DescriptorSet> imageDescriptorSet;
    auto it = descriptorSets.find(imageView);
    if (it != descriptorSets.end())
        imageDescriptorSet = it->second;
    else
    {   
        std::shared_ptr<Sampler> sampler = (VK_FILTER_NEAREST == filter) ? nearestSampler :
            ((VK_FILTER_LINEAR == filter) ? bilinearSampler : cubicSampler);
        setLayouts.push_back(SetLayout());
        setLayouts.back().image = {imageView, sampler};
        // Allocate descriptor set per image
        imageDescriptorSet = std::make_shared<DescriptorSet>(descriptorPool, 0, setLayouts.back(), VK_SHADER_STAGE_FRAGMENT_BIT, descriptorPool->getHostAllocator());
        descriptorSets[imageView] = imageDescriptorSet;
    }
    int32_t height = static_cast<int32_t>(rc.extent.height);
    if (negativeViewportHeight)
        height = -height;
    cmdBuffer->setViewport(rc.offset.x, rc.offset.y, rc.extent.width, height);
    cmdBuffer->setScissor(rc);
    cmdBuffer->bindDescriptorSet(pipeline, imageDescriptorSet);
    cmdBuffer->bindPipeline(pipeline);
    cmdBuffer->draw(3);
}
} // namespace aux
} // namespace magma
