/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "swapchainImage.h"
#include "device.h"
#include "swapchain.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_swapchain
SwapchainImage::SwapchainImage(std::shared_ptr<Device> device, VkImage handle, VkFormat format,
    const VkExtent2D& extent, uint32_t chainIndex):
    Image2D(std::move(device), handle, format, extent),
    implementationControlled(true),
    chainIndex(static_cast<int32_t>(chainIndex))
{}

// Images can also be created by using vkCreateImage with VkImageSwapchainCreateInfoKHR
// and bound to swapchain memory using vkBindImageMemory2 with VkBindImageMemorySwapchainInfoKHR.
// These images can be used anywhere swapchain images are used, and are useful in logical devices
// with multiple physical devices to create peer memory bindings of swapchain memory.
// These images and bindings have no effect on what memory is presented.
// Unlike images retrieved from vkGetSwapchainImagesKHR, these images must be destroyed with vkDestroyImage.
SwapchainImage::SwapchainImage(std::shared_ptr<Swapchain> swapchain):
    Image2D(swapchain->getDevice(), VK_NULL_HANDLE, swapchain->getSurfaceFormat().format, swapchain->getExtent()),
    implementationControlled(false),
    chainIndex(-1)
{   // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#swapchain-wsi-image-create-info
    VkImageCreateFlags flags = 0;
#ifdef VK_KHR_device_group
    if (swapchain->getFlags() & VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR)
        flags |= VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR;
#endif
    if (swapchain->getFlags() & VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR)
        flags |= VK_IMAGE_CREATE_PROTECTED_BIT;
#ifdef VK_KHR_swapchain_mutable_format
    if (swapchain->getFlags() & VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR)
        flags |= (VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT | VK_IMAGE_CREATE_EXTENDED_USAGE_BIT_KHR);
#endif
    VkImageCreateInfo imageInfo;
    VkImageSwapchainCreateInfoKHR imageSwapchainInfo;
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = &imageSwapchainInfo;
    imageInfo.flags = flags;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = swapchain->getSurfaceFormat().format;
    imageInfo.extent = extent;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1; // TODO: swapchain->getArrayLayers()
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = swapchain->getImageUsage();
    imageInfo.sharingMode = swapchain->getSharing().getMode();
    imageInfo.queueFamilyIndexCount = swapchain->getSharing().getQueueFamiliesCount();
    imageInfo.pQueueFamilyIndices = swapchain->getSharing().getQueueFamilyIndices().data();
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageSwapchainInfo.sType = VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR;
    imageSwapchainInfo.pNext = nullptr;
    imageSwapchainInfo.swapchain = *swapchain;
    const VkResult result = vkCreateImage(MAGMA_HANDLE(device), &imageInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create swapchain image");
}

SwapchainImage::~SwapchainImage()
{
    if (implementationControlled)
    {   // Image is a presentable image and it is controlled by the
        // implementation and is destroyed with vkDestroySwapchainKHR().
        handle = VK_NULL_HANDLE; // vkDestroyImage() will have no effect
    }
}

void SwapchainImage::bindMemory(std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    MAGMA_UNUSED(memory);
    MAGMA_UNUSED(offset);
}

#ifdef VK_KHR_device_group
void SwapchainImage::bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
    const std::vector<uint32_t>& deviceIndices,
    VkDeviceSize offset /* 0 */)
{
    MAGMA_UNUSED(memory);
    MAGMA_UNUSED(deviceIndices);
    MAGMA_UNUSED(offset);
}

void SwapchainImage::bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions,
    VkDeviceSize offset /* 0 */)
{
    MAGMA_UNUSED(memory);
    MAGMA_UNUSED(deviceIndices);
    MAGMA_UNUSED(splitInstanceBindRegions);
    MAGMA_UNUSED(offset);
}
#endif // VK_KHR_device_group

void SwapchainImage::setChainIndex(uint32_t chainIndex_) noexcept
{
    chainIndex = static_cast<int32_t>(chainIndex_);
}
#endif // VK_KHR_swapchain
} // namespace magma
