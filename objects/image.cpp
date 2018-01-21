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
#include "commandBuffer.h"
#include "transferBuffer.h"
#include "queue.h"
#include "../misc/imageMemoryBarrier.h"
#include "../shared.h"

namespace magma
{
Image::Image(std::shared_ptr<const Device> device, VkImageType imageType, VkFormat format, 
    const VkExtent3D& extent, uint32_t mipLevels, VkImageUsageFlags usage):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, device),
    format(format),
    layout(VK_IMAGE_LAYOUT_UNDEFINED),
    extent(extent),
    mipLevels(mipLevels)
{
    VkImageCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.imageType = imageType;
    info.format = format;
    info.extent = extent;
    info.mipLevels = mipLevels;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
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

Image::Image(std::shared_ptr<const Device> device, VkFormat format):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, device),
    format(format),
    mipLevels(1)
{}

Image::~Image()
{
    if (handle != VK_NULL_HANDLE) // Handle swapchain case
        vkDestroyImage(*device, handle, nullptr);
}

void Image::bindMemory(std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    const VkResult bind = vkBindImageMemory(*device, handle, *memory, offset);
    MAGMA_THROW_FAILURE(bind, "failed to bind image memory");
    this->memory = memory;
}

Image1D::Image1D(std::shared_ptr<const Device> device, VkFormat format, 
    uint32_t width, uint32_t mipLevels, VkImageUsageFlags usage):
    Image(device, VK_IMAGE_TYPE_1D, format, VkExtent3D{width, 1, 1}, mipLevels, usage)
{}

Image2D::Image2D(std::shared_ptr<const Device> device, VkFormat format,
    const VkExtent2D& extent, uint32_t mipLevels, VkImageUsageFlags usage):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1}, mipLevels, usage)
{}

Image2D::Image2D(std::shared_ptr<const Device> device, VkFormat format,
    const std::vector<const void *>& mipLevelData,
    const std::vector<VkExtent2D>& mipLevelExtents,
    const std::vector<VkDeviceSize>& mipLevelSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{mipLevelExtents[0].width, mipLevelExtents[0].height, 1}, 
        static_cast<uint32_t>(mipLevelData.size()), VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
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
        copy.imageExtent.width = mipLevelExtents[level].width;
        copy.imageExtent.height = mipLevelExtents[level].height;
        copy.imageExtent.depth = 1;
        copyRegions.push_back(copy);
        offset += mipLevelSizes[level];
    }
    // Copy mip levels to host visible buffer
    std::shared_ptr<SourceTransferBuffer> srcTransferBuffer(new SourceTransferBuffer(device, offset));
    if (uint8_t *data = reinterpret_cast<uint8_t *>(srcTransferBuffer->getMemory()->map()))
    {
        for (uint32_t level = 0; level < mipLevels; ++level)
        {
            void *mipData = data + copyRegions[level].bufferOffset;
            memcpy(mipData, mipLevelData[level], static_cast<size_t>(mipLevelSizes[level]));
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

Image3D::Image3D(std::shared_ptr<const Device> device, VkFormat format, 
    const VkExtent3D& extent, VkImageUsageFlags usage):
    Image(device, VK_IMAGE_TYPE_3D, format, extent, 1, usage)
{}

SwapchainImage::SwapchainImage(std::shared_ptr<const Device> device, VkImage image, VkFormat format):
    Image(device, format)
{
    handle = image;
}

SwapchainImage::~SwapchainImage()
{
    // Do not call vkDestroyImage() as handle was not created using vkCreateImage()
    handle = VK_NULL_HANDLE;
}
} // namespace magma
