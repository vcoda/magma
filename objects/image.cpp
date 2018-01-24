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
#include "image.h"
#include "device.h"
#include "deviceMemory.h"
#include "buffer.h"
#include "commandBuffer.h"
#include "queue.h"
#include "../misc/imageMemoryBarrier.h"
#include "../shared.h"

namespace magma
{
Image::Image(std::shared_ptr<const Device> device, VkImageType imageType, VkFormat format,
    const VkExtent3D& extent, uint32_t mipLevels, uint32_t arrayLayers, uint32_t samples,
    VkImageUsageFlags usage, VkImageCreateFlags flags /* 0 */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, device),
    imageType(imageType),
    format(format),
    layout(VK_IMAGE_LAYOUT_UNDEFINED),
    extent(extent),
    mipLevels(mipLevels),
    arrayLayers(arrayLayers),
    samples(samples),
    flags(flags)
{
    VkImageCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.imageType = imageType;
    info.format = format;
    info.extent = extent;
    info.mipLevels = mipLevels;
    info.arrayLayers = arrayLayers;
    switch (samples)
    {
    case 1: info.samples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: info.samples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: info.samples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: info.samples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: info.samples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: info.samples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: info.samples = VK_SAMPLE_COUNT_64_BIT; break;
    default:
        MAGMA_THROW("invalid <samples> parameter");
    }
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = usage;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.initialLayout = layout;
    const VkResult create = vkCreateImage(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create image");
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(*device, handle, &memoryRequirements);
    std::shared_ptr<DeviceMemory> memory(new DeviceMemory(device,
        memoryRequirements.size,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
    bindMemory(memory);
}

Image::Image(std::shared_ptr<const Device> device, VkImage image, VkImageType imageType, VkFormat format):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, image, device),
    imageType(imageType),
    format(format),
    mipLevels(1),
    arrayLayers(1),
    samples(1),
    flags(0)
{}

Image::~Image()
{
    vkDestroyImage(*device, handle, nullptr);
}

void Image::bindMemory(std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    const VkResult bind = vkBindImageMemory(*device, handle, *memory, offset);
    MAGMA_THROW_FAILURE(bind, "failed to bind image memory");
    this->memory = memory;
}

VkDeviceSize Image::getCopyRegions(const std::vector<VkExtent2D>& mipExtents, 
     const std::vector<VkDeviceSize>& mipSizes,
     std::vector<VkBufferImageCopy>& copyRegions) const
{
    VkDeviceSize offset = 0;
    for (uint32_t layer = 0; layer < arrayLayers; ++layer)
    {
        for (uint32_t level = 0; level < mipLevels; ++level)
        {
            VkBufferImageCopy copy;
            copy.bufferOffset = offset;
            copy.bufferRowLength = 0;
            copy.bufferImageHeight = 0;
            copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copy.imageSubresource.mipLevel = level;
            copy.imageSubresource.baseArrayLayer = layer;
            copy.imageSubresource.layerCount = 1;
            copy.imageOffset = {0, 0, 0};
            copy.imageExtent.width = mipExtents[level].width;
            copy.imageExtent.height = mipExtents[level].height;
            copy.imageExtent.depth = 1;
            copyRegions.push_back(copy);
            offset += mipSizes[level];
        }
    }
    return offset;
}   

void Image::copyFromBuffer(std::shared_ptr<Buffer> buffer, 
    const std::vector<VkBufferImageCopy>& copyRegions, 
    std::shared_ptr<CommandBuffer> cmdBuffer)
{
    // Define array layers to copy
    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = arrayLayers;
    cmdBuffer->begin();
    {
        // Change layout from undefined to transfer optimal
        const VkImageLayout optimalLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        const ImageMemoryBarrier preCopyBarrier(handle, VK_IMAGE_LAYOUT_UNDEFINED, optimalLayout, subresourceRange);
        cmdBuffer->pipelineImageBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, {preCopyBarrier});
        // Copy image data
        vkCmdCopyBufferToImage(*cmdBuffer, *buffer, handle, optimalLayout,
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
