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
#include "accumulationBuffer.h"
#include "fillRectangleVertexShader.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/framebuffer.h"
#include "../objects/image2DAttachment.h"
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
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/viewportState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../states/samplerState.h"
#include "../misc/format.h"
#include "../allocator/allocator.h"

namespace magma
{
namespace aux
{
struct alignas(16) PushConstant
{
    float value;
};

AccumulationBuffer::AccumulationBuffer(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    std::shared_ptr<ShaderModule> fragmentShader,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */):
    count(0),
    maxCount(std::numeric_limits<uint32_t>::max())
{
    std::shared_ptr<IAllocator> hostAllocator = MAGMA_HOST_ALLOCATOR(allocator);
    const AttachmentDescription attachment(format, 1,
        op::store,
        op::dontCare,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    renderPass = std::make_shared<RenderPass>(device, attachment, hostAllocator);
    // Create high-precision color buffer
    accumBuffer = std::make_shared<ColorAttachment>(device, format, extent, 1, 1, allocator);
    bufferView = std::make_shared<ImageView>(accumBuffer);
    framebuffer = std::make_shared<Framebuffer>(renderPass, bufferView, hostAllocator);
    // Setup descriptor set
    constexpr uint32_t maxDescriptorSets = 1;
    descriptorPool = std::make_shared<DescriptorPool>(device, maxDescriptorSets,
        descriptors::CombinedImageSampler(1),
        hostAllocator, false);
    descriptorSetLayout = std::make_shared<DescriptorSetLayout>(device,
        bindings::FragmentStageBinding(0, descriptors::CombinedImageSampler(1)),
        hostAllocator, 0);
    descriptorSet = descriptorPool->allocateDescriptorSet(descriptorSetLayout);
    nearestSampler = std::make_shared<Sampler>(device, samplers::magMinMipNearestClampToEdge, hostAllocator);
    // Load fullscreen vertex shader
    std::unique_ptr<FillRectangleVertexShader> fillRectangleVertexShader = std::make_unique<FillRectangleVertexShader>(renderPass->getDevice(), hostAllocator);
    const std::vector<PipelineShaderStage> shaderStages = {
        VertexShaderStage(fillRectangleVertexShader->getShader(), fillRectangleVertexShader->getEntryPointName()),
        FragmentShaderStage(fragmentShader, fragmentShader->getReflection() ? fragmentShader->getReflection()->getEntryPointName(0) : "main")
    };
    // Create blending pipeline
    const uint8_t components = Format(format).components();
    constexpr pushconstants::FragmentConstantRange<PushConstant> pushConstantRange;
    auto pipelineLayout = std::make_shared<PipelineLayout>(descriptorSetLayout, pushConstantRange, hostAllocator);
    blendPipeline = std::make_shared<GraphicsPipeline>(device,
        shaderStages,
        renderstates::nullVertexInput,
        renderstates::triangleList,
        TesselationState(),
        ViewportState(0.f, 0.f, extent),
        fillRectangleVertexShader->getRasterizationState(),
        renderstates::dontMultisample,
        renderstates::depthAlwaysDontWrite,
        (1 == components) ? renderstates::blendNormalR :
        (2 == components) ? renderstates::blendNormalRg :
        (3 == components) ? renderstates::blendNormalRgb :
        (4 == components) ? renderstates::blendNormalRgba :
                            renderstates::dontBlendRgba,
        std::initializer_list<VkDynamicState>{},
        std::move(pipelineLayout),
        renderPass, 0,
        hostAllocator,
        std::move(pipelineCache),
        nullptr); // basePipeline
}

void AccumulationBuffer::accumulate(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<ImageView> imageView) noexcept
{
    if (count < maxCount)
    {
        descriptorSet->writeDescriptor(0, imageView, nearestSampler);
        cmdBuffer->beginRenderPass(renderPass, framebuffer);
        {   // Calculate blending weight
            PushConstant blendWeight;
            blendWeight.value = 1.f - count / (1.f + count);
            cmdBuffer->pushConstantBlock(blendPipeline->getLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, blendWeight);
            cmdBuffer->bindDescriptorSet(blendPipeline, descriptorSet);
            cmdBuffer->bindPipeline(blendPipeline);
            cmdBuffer->draw(3);
        }
        cmdBuffer->endRenderPass();
        ++count;
    }
}
} // namespace aux
} // namespace magma
