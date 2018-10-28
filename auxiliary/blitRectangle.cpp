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
#include "framebuffers.h"
#include "../objects/device.h"
#include "../objects/framebuffer.h"
#include "../objects/image2D.h"
#include "../objects/imageView.h"
#include "../objects/renderPass.h"
#include "../objects/shaderStages.h"
#include "../objects/sampler.h"
#include "../objects/descriptorPool.h"
#include "../objects/descriptorSet.h"
#include "../objects/pipelineLayout.h"
#include "../objects/pipeline.h"
#include "../objects/commandPool.h"
#include "../objects/commandBuffer.h"
#include "../objects/semaphore.h"
#include "../objects/fence.h"
#include "../objects/queue.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../states/samplerState.h"
#include "../states/viewportState.h"

namespace magma
{
namespace aux
{
BlitRectangle::BlitRectangle(std::shared_ptr<aux::Framebuffer> framebuffer, std::shared_ptr<CommandPool> cmdPool,
    const VertexShaderStage& vertexShader, const FragmentShaderStage& fragmentShader):
    BlitRectangle(framebuffer->getRenderPass(), framebuffer->getFramebuffer(), std::move(cmdPool), vertexShader, fragmentShader)
{}

BlitRectangle::BlitRectangle(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<magma::Framebuffer> framebuffer, std::shared_ptr<CommandPool> cmdPool,
    const VertexShaderStage& vertexShader, const FragmentShaderStage& fragmentShader):
    device(renderPass->getDevice()),
    queue(std::move(device->getQueue(VK_QUEUE_GRAPHICS_BIT, 0))),
    renderPass(renderPass),
    framebuffer(framebuffer),
    cmdPool(cmdPool)
{   // Descriptor set for single image view in fragment shader
    const Descriptor imageSamplerDesc = descriptors::CombinedImageSampler(1);
    setLayout = std::make_shared<DescriptorSetLayout>(device, bindings::FragmentStageBinding(0, imageSamplerDesc));
    descriptorPool = std::make_shared<DescriptorPool>(device, 1, std::vector<Descriptor>{imageSamplerDesc});
    descriptorSet = descriptorPool->allocateDescriptorSet(setLayout);
    // Create blit pipeline
    pipelineLayout = std::make_shared<PipelineLayout>(setLayout);
    pipeline = std::make_shared<GraphicsPipeline>(device, nullptr,
        std::vector<PipelineShaderStage>{vertexShader, fragmentShader},
        states::nullVertexInput,
        states::triangleStrip,
        TesselationState(),
        ViewportState(0, 0, framebuffer->getExtent()),
        states::fillCullBackCCW,
        states::noMultisample,
        states::depthAlwaysDontWrite,
        states::dontBlendWriteRGBA,
        std::initializer_list<VkDynamicState>{},
        pipelineLayout,
        renderPass);
}

void BlitRectangle::setImageView(std::shared_ptr<ImageView> attachment,
    std::shared_ptr<Sampler> sampler /* nullptr */)
{   // Bind attachment to descriptor set
    if (sampler)
        descriptorSet->update(0, attachment, sampler);
    else
    {   // Default to nearest sampler
        if (!nearestSampler)
            nearestSampler = std::make_shared<Sampler>(device, samplers::nearestMipmapNearestClampToEdge);
        descriptorSet->update(0, attachment, nearestSampler);
    }
    if (!cmdBuffer)
    {
        cmdBuffer = cmdPool->allocateCommandBuffer(true);
        cmdBuffer->setRenderArea(0, 0, framebuffer->getExtent());
    }
    // Record command buffer
    cmdBuffer->reset(false);
    cmdBuffer->begin();
    {
        cmdBuffer->beginRenderPass(renderPass, framebuffer, {clears::blackColor});
        {
            cmdBuffer->bindDescriptorSet(pipelineLayout, descriptorSet);
            cmdBuffer->bindPipeline(pipeline);
            cmdBuffer->draw(4, 0);
        }
        cmdBuffer->endRenderPass();
    }
    cmdBuffer->end();
}

void BlitRectangle::blit(std::shared_ptr<Semaphore> renderFinished, std::shared_ptr<Semaphore> blitFinished, std::shared_ptr<Fence> fence) const noexcept
{
    queue->submit(cmdBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        renderFinished, blitFinished, fence);
}
} // namespace aux
} // namespace magma
