/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_swapchain
SwapchainImage::SwapchainImage(VkImage handle, std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    uint32_t arrayLayers, VkImageUsageFlags usage, uint32_t chainIndex):
    Image2D(std::move(device), format, extent, /* mipLevels */1, arrayLayers, /* samples */1, 0, usage, VK_IMAGE_TILING_OPTIMAL),
    implementationControlled(true),
    chainIndex(static_cast<int32_t>(chainIndex))
{
    Image::handle = handle;
}

// Images can also be created by using vkCreateImage with VkImageSwapchainCreateInfoKHR
// and bound to swapchain memory using vkBindImageMemory2 with VkBindImageMemorySwapchainInfoKHR.
// These images can be used anywhere swapchain images are used, and are useful in logical devices
// with multiple physical devices to create peer memory bindings of swapchain memory.
// These images and bindings have no effect on what memory is presented.
// Unlike images retrieved from vkGetSwapchainImagesKHR, these images must be destroyed with vkDestroyImage.
#ifdef VK_VERSION_1_1
SwapchainImage::SwapchainImage(lent_ptr<const Swapchain> swapchain):
    Image2D(swapchain->getDevice(),
        swapchain->getSurfaceFormat().format,
        swapchain->getExtent(),
        1, // mipLevels
        swapchain->getArrayLayers(),
        1, // samples
        swapchain->getImageFlags(),
        swapchain->getImageUsage(),
        VK_IMAGE_TILING_OPTIMAL),
    implementationControlled(false),
    chainIndex(-1)
{   // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#swapchain-wsi-image-create-info
    VkImageCreateInfo imageInfo;
    VkImageSwapchainCreateInfoKHR imageSwapchainInfo;
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = &imageSwapchainInfo;
    imageInfo.flags = flags;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = format;
    imageInfo.extent = extent;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = arrayLayers;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.sharingMode = swapchain->getSharing().getMode();
    imageInfo.queueFamilyIndexCount = swapchain->getSharing().getQueueFamiliesCount();
    imageInfo.pQueueFamilyIndices = swapchain->getSharing().getQueueFamilyIndices().data();
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageSwapchainInfo.sType = VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR;
    imageSwapchainInfo.pNext = nullptr;
    imageSwapchainInfo.swapchain = *swapchain;
    const VkResult result = vkCreateImage(getNativeDevice(), &imageInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create swapchain image");
}
#endif // VK_VERSION_1_1

SwapchainImage::~SwapchainImage()
{
    if (implementationControlled)
    {   // Image is a presentable image and it is controlled by the
        // implementation and is destroyed with vkDestroySwapchainKHR().
        handle = VK_NULL_HANDLE; // vkDestroyImage() will have no effect
    }
}

void SwapchainImage::bindMemory(std::shared_ptr<IDeviceMemory> /* memory */, VkDeviceSize /* offset */)
{
    // Memory is owned by the presentation engine
}

#ifdef VK_KHR_device_group
void SwapchainImage::bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> /* memory */,
    const std::vector<uint32_t>& /* deviceIndices */,
    const std::vector<VkRect2D>& /* splitInstanceBindRegions */,
    VkDeviceSize /* offset */)
{
    // Memory is owned by the presentation engine
}
#endif // VK_KHR_device_group

void SwapchainImage::setChainIndex(uint32_t chainIndex_) noexcept
{
    chainIndex = static_cast<int32_t>(chainIndex_);
}
#endif // VK_KHR_swapchain
} // namespace magma
