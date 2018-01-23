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
    const VkExtent3D& extent, uint32_t mipLevels, uint32_t arrayLayers, VkImageUsageFlags usage):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, device),
    format(format),
    layout(VK_IMAGE_LAYOUT_UNDEFINED),
    extent(extent),
    mipLevels(mipLevels),
    arrayLayers(arrayLayers)
{
    VkImageCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.imageType = imageType;
    info.format = format;
    info.extent = extent;
    info.mipLevels = mipLevels;
    info.arrayLayers = arrayLayers;
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
    mipLevels(1),
    arrayLayers(1)
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
