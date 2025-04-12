/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/shaderModule.h"
#include "../objects/sampler.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"
#include "../objects/pipelineLayout.h"
#include "../objects/pipelineLibrary.h"
#include "../objects/graphicsPipeline.h"
#include "../objects/commandBuffer.h"
#include "../shaders/shaderStages.h"
#include "../shaders/shaderReflection.h"
#include "../shaders/shaderReflectionFactory.h"
#include "../shaders/specialization.h"
#include "../descriptors/imageDescriptor.h"
#include "../states/vertexInputStructure.h"
#include "../states/inputAssemblyState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../states/samplerState.h"

namespace magma::aux
{
constexpr
#include "spirv/output/blitf"

struct BlitRectangle::DescriptorSetTable
{
    descriptor::CombinedImageSampler image = 0;
};

BlitRectangle::DescriptorSetTable BlitRectangle::setTableTemplate;

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass,
    lent_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    BlitRectangle(renderPass,
        std::make_shared<ShaderModule>(renderPass->getDevice(), fsBlit, core::hashArray(fsBlit), allocator, true),
        nullptr, // No specialization
        std::move(pipelineCache), std::move(allocator))
{}

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass,
    std::shared_ptr<ShaderModule> fragmentShader,
    std::shared_ptr<Specialization> specialization /* nullptr */,
    lent_ptr<PipelineCache> pipelineCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    renderPass(std::move(renderPass))
{
    std::shared_ptr<Device> device = this->renderPass->getDevice();
    std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    constexpr uint32_t maxDescriptorSets = 10;
    descriptorPool = std::make_shared<DescriptorPool>(device, maxDescriptorSets,
        descriptor::CombinedImageSamplerPool(maxDescriptorSets), allocator);
    descriptorSet = std::make_unique<DescriptorSet>(descriptorPool, setTableTemplate, VK_SHADER_STAGE_FRAGMENT_BIT, allocator);
    // Create texture samplers
    nearestSampler = std::make_unique<Sampler>(device, sampler::magMinMipNearestClampToEdge, allocator);
    bilinearSampler = std::make_unique<Sampler>(device, sampler::magMinLinearMipNearestClampToEdge, allocator);
    // Check for cubic filtering support
#ifdef VK_EXT_filter_cubic
    bool hasCubicFilter = device->extensionEnabled(VK_EXT_FILTER_CUBIC_EXTENSION_NAME);
    #ifdef VK_IMG_filter_cubic
    hasCubicFilter |= device->extensionEnabled(VK_IMG_FILTER_CUBIC_EXTENSION_NAME);
    #endif
    if (hasCubicFilter)
        cubicSampler = std::make_unique<Sampler>(device, sampler::magCubicMinLinearMipNearestClampToEdge, allocator);
#endif // VK_EXT_filter_cubic
    // Setup shader stages
    FillRectangleVertexShader vertexShaderStage(device, allocator);
    const RasterizationState rasterizationState = vertexShaderStage.getRasterizationState();
    const char *entryPointName = fragmentShader->getReflection() ? fragmentShader->getReflection()->getEntryPointName(0) : "main";
    std::vector<PipelineShaderStage> shaderStages;
    shaderStages.emplace_back(std::move(vertexShaderStage));
    shaderStages.emplace_back(VK_SHADER_STAGE_FRAGMENT_BIT, std::move(fragmentShader), entryPointName, std::move(specialization));
    // Setup multisample state
    const VkSampleCountFlagBits samples = this->renderPass->getAttachments().front().samples;
    const MultisampleState multisampleState =
        (samples & VK_SAMPLE_COUNT_1_BIT) ? renderstate::dontMultisample :
        (samples & VK_SAMPLE_COUNT_2_BIT) ? renderstate::msaa2x :
        (samples & VK_SAMPLE_COUNT_4_BIT) ? renderstate::msaa4x :
        (samples & VK_SAMPLE_COUNT_8_BIT) ? renderstate::msaa8x :
        (samples & VK_SAMPLE_COUNT_16_BIT) ? renderstate::msaa16x :
        (samples & VK_SAMPLE_COUNT_32_BIT) ? renderstate::msaa32x :
        (samples & VK_SAMPLE_COUNT_64_BIT) ? renderstate::msaa64x :
        renderstate::dontMultisample;
    // Create blit pipeline
    std::unique_ptr<PipelineLayout> pipelineLayout = std::make_unique<PipelineLayout>(
        descriptorSet->getLayout(), allocator);
    pipeline = std::make_unique<GraphicsPipeline>(std::move(device),
        shaderStages,
        renderstate::nullVertexInput,
        renderstate::triangleList,
        rasterizationState,
        multisampleState,
        renderstate::depthAlwaysDontWrite,
        renderstate::dontBlendRgba,
        std::vector<VkDynamicState>{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR},
        std::move(pipelineLayout),
        this->renderPass, 0,
        std::move(allocator),
        std::move(pipelineCache),
        nullptr); // basePipeline
    for (auto const& attachment: this->renderPass->getAttachments())
    {
        if (VK_ATTACHMENT_LOAD_OP_CLEAR == attachment.loadOp)
        {   // The Vulkan spec states: clearValueCount must be greater than the largest attachment index in renderPass that specifies a loadOp
            // (or stencilLoadOp, if the attachment has a depth/stencil format) of VK_ATTACHMENT_LOAD_OP_CLEAR.
            clearValues = std::vector<ClearValue>(this->renderPass->getAttachments().size(), clear::black);
            break;
        }
    }
}

void BlitRectangle::blit(lent_ptr<CommandBuffer> cmdBuffer, lent_ptr<const ImageView> imageView, VkFilter filter, const VkRect2D& rc,
    bool negativeViewportHeight /* false */) const noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(cmdBuffer);
    auto it = descriptorSets.find(imageView.get());
    if (it != descriptorSets.end())
        cmdBuffer->bindDescriptorSet(pipeline, 0, it->second);
    else
    {
        const descriptor::ImageSampler imageSampler{
            std::move(imageView),
            (VK_FILTER_NEAREST == filter) ? nearestSampler :
            ((VK_FILTER_LINEAR == filter) ? bilinearSampler : cubicSampler)
        };
        setTables.emplace_front();
        setTables.front().image = imageSampler;
        // Allocate each descriptor set per unique image
        auto descriptorSet = std::make_unique<DescriptorSet>(descriptorPool, setTables.front(), VK_SHADER_STAGE_FRAGMENT_BIT, descriptorPool->getHostAllocator());
        cmdBuffer->bindDescriptorSet(pipeline, 0, descriptorSet);
        descriptorSets[imageSampler.first.get()] = std::move(descriptorSet);
    }
    int32_t height = static_cast<int32_t>(rc.extent.height);
    if (negativeViewportHeight)
        height = -height;
    cmdBuffer->setViewport(rc.offset.x, rc.offset.y, rc.extent.width, height);
    cmdBuffer->setScissor(rc);
    cmdBuffer->bindPipeline(pipeline);
    cmdBuffer->draw(3);
}
} // namespace magma::aux
