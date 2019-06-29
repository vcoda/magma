/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "blitRectangle.h"
#include "../objects/device.h"
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
#include "../states/vertexInputState.h"
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
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BlitRectangle(renderPass,
        VertexShaderStage(createShader(renderPass->getDevice(), allocator, true), "main"),
        FragmentShaderStage(createShader(renderPass->getDevice(), allocator, false), "main"),
        std::move(allocator))
{}

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass, const PipelineShaderStage& vertexShader, const PipelineShaderStage& fragmentShader,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    renderPass(std::move(renderPass))
{   // Descriptor set for single image view in fragment shader
    const Descriptor imageSampler(descriptors::CombinedImageSampler(1));
    auto device = this->renderPass->getDevice();
    descriptorPool = std::make_shared<DescriptorPool>(device, 1, std::vector<Descriptor>{imageSampler}, false, allocator);
    descriptorSetLayout = std::make_shared<DescriptorSetLayout>(device, bindings::FragmentStageBinding(0, imageSampler), 0, allocator);
    descriptorSet = descriptorPool->allocateDescriptorSet(descriptorSetLayout);
    nearestSampler = std::make_shared<Sampler>(device, samplers::nearestMipmapNearestClampToEdge, 0.f, allocator);
    // Create blit pipeline
    pipelineLayout = std::make_shared<PipelineLayout>(descriptorSetLayout, std::initializer_list<VkPushConstantRange>{}, allocator);
    pipeline = std::make_shared<GraphicsPipeline>(device, nullptr,
        std::vector<PipelineShaderStage>{vertexShader, fragmentShader},
        renderstates::nullVertexInput,
        renderstates::triangleStrip,
        renderstates::fillCullBackCCW,
        renderstates::noMultisample,
        renderstates::depthAlwaysDontWrite,
        renderstates::dontBlendWriteRGBA,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        pipelineLayout,
        this->renderPass,
        0, nullptr, 0, allocator);
}

void BlitRectangle::blit(const std::shared_ptr<Framebuffer>& bltDst, const std::shared_ptr<ImageView>& bltSrc, const std::shared_ptr<CommandBuffer>& cmdBuffer,
    const char *labelName /* nullptr */, uint32_t labelColor /* 0xFFFFFFFF */) const noexcept
{
    MAGMA_ASSERT(bltDst);
    MAGMA_ASSERT(bltSrc);
    MAGMA_ASSERT(cmdBuffer);
    if (prevBltSrc != bltSrc)
    {
        descriptorSet->update(0, bltSrc, nearestSampler);
        prevBltSrc = bltSrc;
    }
    cmdBuffer->setRenderArea(0, 0, bltDst->getExtent());
    if (labelName)
        cmdBuffer->beginRenderPass(renderPass, bltDst, {}, labelName, labelColor);
    else
        cmdBuffer->beginRenderPass(renderPass, bltDst);
    {
        cmdBuffer->setViewport(Viewport(0, 0, bltDst->getExtent()));
        cmdBuffer->setScissor(Scissor(0, 0, bltDst->getExtent()));
        cmdBuffer->bindDescriptorSet(pipelineLayout, descriptorSet);
        cmdBuffer->bindPipeline(pipeline);
        cmdBuffer->draw(4, 0);
    }
    cmdBuffer->endRenderPass();
}

std::shared_ptr<ShaderModule> BlitRectangle::createShader(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator, bool vertexShader) const
{
#include "spirv/output/blitv"
    if (vertexShader)
        return std::make_shared<ShaderModule>(device, vsBlit, sizeof(vsBlit), allocator);
#include "spirv/output/blitf"
    return std::make_shared<ShaderModule>(device, fsBlit, sizeof(fsBlit), allocator);
}
} // namespace aux
} // namespace magma
