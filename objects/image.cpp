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
#include "fence.h"
#include "../allocator/allocator.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"

namespace magma
{
Image::Image(std::shared_ptr<Device> device, VkImageType imageType, VkFormat format,
    const VkExtent3D& extent, uint32_t mipLevels, uint32_t arrayLayers, uint32_t samples,
    VkImageUsageFlags usage, VkImageCreateFlags flags,
    std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE, std::move(device), std::move(allocator)),
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
    const VkResult create = vkCreateImage(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create image");
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(MAGMA_HANDLE(device), handle, &memoryRequirements);
    std::shared_ptr<DeviceMemory> memory(std::make_shared<DeviceMemory>(
        this->device, memoryRequirements.size, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
    bindMemory(memory);
}

Image::Image(std::shared_ptr<Device> device, VkImage handle, VkImageType imageType, VkFormat format, const VkExtent3D& extent):
    NonDispatchable(VK_OBJECT_TYPE_IMAGE, std::move(device), nullptr),
    imageType(imageType),
    format(format),
    layout(VK_IMAGE_LAYOUT_UNDEFINED),
    extent(extent),
    mipLevels(1),
    arrayLayers(1),
    samples(1),
    flags(0)
{
    this->handle = handle;
}

Image::~Image()
{
    vkDestroyImage(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

VkExtent3D Image::getMipExtent(uint32_t level) const
{
    if (0 == level)
        return extent;
    if (level >= mipLevels)
        MAGMA_THROW("invalid <level> argument");
    VkExtent3D mipExtent = extent;
    for (uint32_t i = 0; i < level; ++i)
    {
        if (mipExtent.width > 1)
			mipExtent.width >>= 1;
        if (mipExtent.height > 1)
			mipExtent.height >>= 1;
        if (mipExtent.depth > 1)
			mipExtent.depth >>= 1;
    }
    return mipExtent;
}

void Image::bindMemory(std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    const VkResult bind = vkBindImageMemory(MAGMA_HANDLE(device), handle, *memory, offset);
    MAGMA_THROW_FAILURE(bind, "failed to bind image memory");
    this->memory = std::move(memory);
}

void Image::bindMemoryDeviceGroup(const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions,
    std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    VkBindImageMemoryDeviceGroupInfo deviceGroupBindInfo;
    deviceGroupBindInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO;
    deviceGroupBindInfo.pNext = nullptr;
    deviceGroupBindInfo.deviceIndexCount = MAGMA_COUNT(deviceIndices);
    deviceGroupBindInfo.pDeviceIndices = deviceIndices.data();
    deviceGroupBindInfo.splitInstanceBindRegionCount = MAGMA_COUNT(splitInstanceBindRegions);
    deviceGroupBindInfo.pSplitInstanceBindRegions = splitInstanceBindRegions.data();
    VkBindImageMemoryInfo bindInfo;
    bindInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
    bindInfo.pNext = &deviceGroupBindInfo;
    bindInfo.image = handle;
    bindInfo.memory = *memory;
    bindInfo.memoryOffset = offset;
    MAGMA_DEVICE_EXTENSION(vkBindImageMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    const VkResult bind = vkBindImageMemory2KHR(MAGMA_HANDLE(device), 1, &bindInfo);
    MAGMA_THROW_FAILURE(bind, "failed to bind image memory within device group");
    this->memory = std::move(memory);
}

void Image::copyMipLevel(uint32_t mipLevel, std::shared_ptr<Buffer> buffer, uint32_t bufferOffset,
    const VkOffset3D& imageOffset, std::shared_ptr<CommandBuffer> copyCmdBuffer,
    VkPipelineStageFlags barrierDstStageMask /* VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT */,
    bool flush /* true */)
{
    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = mipLevel;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;
    copyCmdBuffer->begin();
    {   // Change layout from undefined to transfer optimal
        const ImageMemoryBarrier preCopyBarrier(shared_from_this(),
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange);
        copyCmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, preCopyBarrier);
        VkBufferImageCopy region;
        region.bufferOffset = bufferOffset;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = mipLevel;
        region.imageSubresource.baseArrayLayer = subresourceRange.baseArrayLayer;
        region.imageSubresource.layerCount = subresourceRange.layerCount;
        region.imageOffset = imageOffset;
        region.imageExtent = getMipExtent(mipLevel);
        // Copy image data
        copyCmdBuffer->copyBufferToImage(buffer, shared_from_this(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, region);
        // Change layout from transfer optimal to shader read only
        const ImageMemoryBarrier postCopyBarrier(shared_from_this(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            subresourceRange);
        copyCmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, barrierDstStageMask, postCopyBarrier);
    }
    copyCmdBuffer->end();
    if (flush)
    {
        std::shared_ptr<Queue> queue(device->getQueue(VK_QUEUE_GRAPHICS_BIT, 0));
        std::shared_ptr<Fence> fence(copyCmdBuffer->getFence());
        if (!queue->submit(std::move(copyCmdBuffer), 0, nullptr, nullptr, fence))
            MAGMA_THROW("failed to submit command buffer to transfer queue");
        if (!fence->wait())
            MAGMA_THROW("failed to wait fence");
    }
}

std::vector<VkBufferImageCopy> Image::getCopyRegions(const std::vector<VkDeviceSize>& mipSizes,
     VkDeviceSize *offset) const noexcept
{
    std::vector<VkBufferImageCopy> copyRegions;
    *offset = 0;
    for (uint32_t layer = 0; layer < arrayLayers; ++layer)
    {
        for (uint32_t level = 0; level < mipLevels; ++level)
        {
            VkBufferImageCopy region;
            region.bufferOffset = *offset;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = level;
            region.imageSubresource.baseArrayLayer = layer;
            region.imageSubresource.layerCount = 1;
            region.imageOffset = {0, 0, 0};
            region.imageExtent = getMipExtent(level);
            copyRegions.push_back(region);
            *offset += mipSizes[level];
            *offset = MAGMA_ALIGN(*offset);
        }
    }
    return copyRegions;
}

void Image::copyFromBuffer(std::shared_ptr<Buffer> buffer,
    const std::vector<VkBufferImageCopy>& copyRegions,
    std::shared_ptr<CommandBuffer> copyCmdBuffer)
{
    // Define array layers to copy
    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = arrayLayers;
    copyCmdBuffer->begin();
    {   // We couldn't call shared_from_this() from ctor, so use custom ref object w/ empty deleter
        const auto weakRef = std::shared_ptr<Image>(this, [](Image *) {});
        // Change layout from undefined to transfer optimal
        const VkImageLayout optimalLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        const ImageMemoryBarrier preCopyBarrier(weakRef, VK_IMAGE_LAYOUT_UNDEFINED, optimalLayout, subresourceRange);
        copyCmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, preCopyBarrier);
        // Copy image data
        vkCmdCopyBufferToImage(*copyCmdBuffer, *buffer, handle, optimalLayout,
            MAGMA_COUNT(copyRegions), copyRegions.data());
        // Change layout from transfer optimal to shader read only
        layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        const ImageMemoryBarrier postCopyBarrier(weakRef, optimalLayout, layout, subresourceRange);
        copyCmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, postCopyBarrier);
    }
    copyCmdBuffer->end();
    // Flush
    std::shared_ptr<Queue> queue(device->getQueue(VK_QUEUE_GRAPHICS_BIT, 0));
    std::shared_ptr<Fence> fence(copyCmdBuffer->getFence());
    if (!queue->submit(std::move(copyCmdBuffer), 0, nullptr, nullptr, fence))
        MAGMA_THROW("failed to submit command buffer to graphics queue");
    if (!fence->wait())
        MAGMA_THROW("failed to wait fence");
}
} // namespace magma
