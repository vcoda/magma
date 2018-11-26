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
#include "mipmapBuilder.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/image1D.h"
#include "../objects/image2D.h"
#include "../objects/commandBuffer.h"
#include "../objects/queue.h"
#include "../objects/fence.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/imageSubresourceRange.h"
#include "../shared.h"

namespace magma
{
namespace aux
{
MipmapBuilder::MipmapBuilder(std::shared_ptr<Device> device):
    device(std::move(device)),
    queue(this->device->getQueue(VK_QUEUE_GRAPHICS_BIT, 0))
{}

bool MipmapBuilder::checkFormatSupport(VkFormat format) const noexcept
{
    std::shared_ptr<PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    const VkFormatProperties& properties = physicalDevice->getFormatProperties(format);
    bool srcBlit = (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT);
    bool dstBlit = (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
    return srcBlit && dstBlit;
}

bool MipmapBuilder::buildMipmap1D(std::shared_ptr<Image1D> image, uint32_t firstLevel, VkFilter /*filter*/,
    std::shared_ptr<CommandBuffer> commandBuffer, bool /* commit */) const noexcept
{
    MAGMA_ASSERT(firstLevel > 0);
    // TODO:
    return true;
}

bool MipmapBuilder::buildMipmap2D(std::shared_ptr<Image2D> image, uint32_t firstLevel, VkFilter filter,
    std::shared_ptr<CommandBuffer> commandBuffer, bool commit) const noexcept
{
    MAGMA_ASSERT(firstLevel > 0);
    if (commit)
        commandBuffer->begin();
    for (uint32_t level = firstLevel; level < image->getMipLevels(); ++level)
    {
        const VkExtent3D& prevMipExtent = image->getMipExtent(level - 1);
        const VkExtent3D& currMipExtent = image->getMipExtent(level);
        VkImageBlit blitRegion;
        blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.srcSubresource.mipLevel = level - 1;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcOffsets[0] = VkOffset3D{0, 0, 0};
        blitRegion.srcOffsets[1] = VkOffset3D{int32_t(prevMipExtent.width), int32_t(prevMipExtent.height), 1};
        blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.dstSubresource.mipLevel = level;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount = 1;
        blitRegion.dstOffsets[0] = VkOffset3D{0, 0, 0};
        blitRegion.dstOffsets[1] = VkOffset3D{int32_t(currMipExtent.width), int32_t(currMipExtent.height), 1};
        const ImageSubresourceRange currMipRange(image, level, 1);
        // Transition current mip level to transfer dest
        const ImageMemoryBarrier undefinedToDstBarrier(
            image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            currMipRange);
        commandBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, undefinedToDstBarrier);
        // Downsample larger mip to smaller one
        commandBuffer->blitImage(image, image, blitRegion, filter);
        // Transition current mip level back to transfer source
        const ImageMemoryBarrier dstToSrcBarrier(
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            currMipRange);
        commandBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, dstToSrcBarrier);
    }
    // Blitted mip levels are transitioned to shader read layout
    const ImageSubresourceRange blitMipsRange(image, firstLevel, image->getMipLevels() - firstLevel);
    const ImageMemoryBarrier shaderReadBarrier(
        image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        blitMipsRange);
    commandBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, shaderReadBarrier);
    if (commit)
    {
        commandBuffer->end();
        std::shared_ptr<magma::Fence> fence(commandBuffer->getFence());
        if (!queue->submit(commandBuffer, 0, nullptr, nullptr, fence))
            return false;
        return fence->wait();
    }
    return true;
}
} // namespace aux
} // namespace magma
