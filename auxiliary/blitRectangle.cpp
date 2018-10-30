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

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass, const VertexShaderStage& vertexShader, const FragmentShaderStage& fragmentShader,
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
        states::nullVertexInput,
        states::triangleStrip,
        states::fillCullBackCCW,
        states::noMultisample,
        states::depthAlwaysDontWrite,
        states::dontBlendWriteRGBA,
        std::initializer_list<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        pipelineLayout,
        this->renderPass,
        0, nullptr, 0, allocator);
}

void BlitRectangle::blit(std::shared_ptr<Framebuffer>& bltDst, std::shared_ptr<ImageView>& bltSrc, std::shared_ptr<CommandBuffer>& cmdBuffer,
    const char *markerName /* nullptr */, uint32_t markerColor /* 0x0 */) const
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
    if (markerName)
        cmdBuffer->beginRenderPass(renderPass, bltDst, {}, markerName, markerColor);
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
    if (vertexShader)
    {
        const uint32_t vertexShaderBytecode[] = {
#include "spirv/blitv.h"
        };
        return std::make_shared<ShaderModule>(device, vertexShaderBytecode, sizeof(vertexShaderBytecode), allocator);
    }
    else
    {
        const uint32_t fragmentShaderBytecode[] = {
#include "spirv/blitf.h"
        };
        return std::make_shared<ShaderModule>(device, fragmentShaderBytecode, sizeof(fragmentShaderBytecode), allocator);
    }
}
} // namespace aux
} // namespace magma
