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
#include "image2D.h"
#include "device.h"
#include "deviceMemory.h"
#include "commandBuffer.h"
#include "transferBuffer.h"
#include "queue.h"
#include "../misc/imageMemoryBarrier.h"
#include "../shared.h"

namespace magma
{
Image2D::Image2D(std::shared_ptr<const Device> device, VkFormat format,
    const VkExtent2D& extent, uint32_t mipLevels, VkImageUsageFlags usage):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1}, mipLevels, 1, usage)
{}

Image2D::Image2D(std::shared_ptr<const Device> device, 
    VkFormat format,
    const std::vector<VkExtent2D>& mipExtents,
    const std::vector<const void *>& mipData,
    const std::vector<VkDeviceSize>& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{mipExtents[0].width, mipExtents[0].height, 1},
        MAGMA_COUNT(mipExtents), // mipLevels
        1, // arrayLayers 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
{
    // Define copy regions
    std::vector<VkBufferImageCopy> copyRegions;
    VkDeviceSize offset = 0;
    for (uint32_t level = 0; level < mipLevels; ++level)
    {
        VkBufferImageCopy copy;
        copy.bufferOffset = offset;
        copy.bufferRowLength = 0;
        copy.bufferImageHeight = 0;
        copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copy.imageSubresource.mipLevel = level;
        copy.imageSubresource.baseArrayLayer = 0;
        copy.imageSubresource.layerCount = 1;
        copy.imageOffset = {0, 0, 0};
        copy.imageExtent.width = mipExtents[level].width;
        copy.imageExtent.height = mipExtents[level].height;
        copy.imageExtent.depth = 1;
        copyRegions.push_back(copy);
        offset += mipSizes[level];
    }
    // Copy mip levels to host visible buffer
    std::shared_ptr<SourceTransferBuffer> srcTransferBuffer(new SourceTransferBuffer(device, offset));
    if (uint8_t *data = reinterpret_cast<uint8_t *>(srcTransferBuffer->getMemory()->map()))
    {
        for (uint32_t level = 0; level < mipLevels; ++level)
        {
            const VkDeviceSize bufferOffset = copyRegions[level].bufferOffset;
            void *mipLevel = data + bufferOffset;
            memcpy(mipLevel, mipData[level], static_cast<size_t>(mipSizes[level]));
        }
        srcTransferBuffer->getMemory()->unmap();
    }
    // Define mip levels to copy
    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;
    cmdBuffer->begin();
    {
        // Change layout from undefined to transfer optimal
        const VkImageLayout optimalLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        const ImageMemoryBarrier preCopyBarrier(handle, VK_IMAGE_LAYOUT_UNDEFINED, optimalLayout, subresourceRange);
        cmdBuffer->pipelineImageBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, {preCopyBarrier});
        // Copy image data
        vkCmdCopyBufferToImage(*cmdBuffer, *srcTransferBuffer, handle, optimalLayout,
            MAGMA_COUNT(copyRegions), copyRegions.data());
        // Change layout from transfer optimal to shader read only
        layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        const ImageMemoryBarrier postCopyBarrier(handle, optimalLayout, layout, subresourceRange);
        cmdBuffer->pipelineImageBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, {postCopyBarrier});
    }
    cmdBuffer->end();
    // Flush
    std::shared_ptr<Queue> queue = device->getQueue(VK_QUEUE_GRAPHICS_BIT, 0);
    queue->submit(cmdBuffer, 0, nullptr, nullptr, nullptr);
    queue->waitIdle();
}
} // namespace magma
