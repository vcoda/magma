/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "swapchain.h"
#include "device.h"
#include "physicalDevice.h"
#include "surface.h"
#include "image2DAttachment.h"
#include "queue.h"
#include "semaphore.h"
#include "fence.h"
#include "debugReportCallback.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../helpers/stackArray.h"
#include "../helpers/stringize.h"
#include "../helpers/checkFeatureSupport.h"

namespace magma
{
Swapchain::Swapchain(std::shared_ptr<Device> device, std::shared_ptr<const Surface> surface,
    uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& extent,
    VkImageUsageFlags usage,
    VkSurfaceTransformFlagBitsKHR preTransform,
    VkCompositeAlphaFlagBitsKHR compositeAlpha,
    VkPresentModeKHR presentMode,
    VkSwapchainCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<const DebugReportCallback> debugReportCallback /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_SWAPCHAIN_KHR, std::move(device), std::move(allocator)),
    surfaceFormat(surfaceFormat),
    extent(extent),
    imageIndex(0)
{
    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.surface = *surface;
    info.minImageCount = minImageCount;
    info.imageFormat = surfaceFormat.format;
    info.imageColorSpace = surfaceFormat.colorSpace;
    info.imageExtent = extent;
    info.imageArrayLayers = 1;
    info.imageUsage = usage;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.preTransform = preTransform;
    info.compositeAlpha = compositeAlpha;
    info.presentMode = presentMode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = VK_NULL_HANDLE;
    helpers::checkImageUsageSupport(surface, info.imageUsage, this->device->getPhysicalDevice());
    VkResult create;
#ifdef VK_KHR_display_swapchain
    if (std::dynamic_pointer_cast<const DisplaySurface>(surface))
    {
        MAGMA_DEVICE_EXTENSION(vkCreateSharedSwapchainsKHR, VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME);
        create = vkCreateSharedSwapchainsKHR(MAGMA_HANDLE(device), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    }
    else
#endif // VK_KHR_display_swapchain
    {
        create = vkCreateSwapchainKHR(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    }
#ifdef VK_EXT_debug_report
    if (create != VK_SUCCESS && debugReportCallback)
    {
        debugReportCallback->message(VK_DEBUG_REPORT_ERROR_BIT_EXT, getObjectType(), VK_NULL_HANDLE, 0, 0, "magma",
            "swapchain initialization failed with the following parameters:\n"
            "minImageCount: %d\nimageFormat: %s\nimageColorSpace: %s\nimageExtent: {%d, %d}\n"
            "imageArrayLayers: %d\nimageUsage: %s\nimageSharingMode: %s\n"
            "preTransform: %s\ncompositeAlpha: %s\npresentMode: %s\nclipped: %s\n"
            "oldSwapchain: %d\n",
            info.minImageCount,
            helpers::stringize(info.imageFormat),
            helpers::stringize(info.imageColorSpace),
            info.imageExtent.width,
            info.imageExtent.height,
            info.imageArrayLayers,
            helpers::stringize(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT),
            helpers::stringize(info.imageSharingMode),
            helpers::stringize(info.preTransform),
            helpers::stringize(info.compositeAlpha),
            helpers::stringize(info.presentMode),
            helpers::stringize(info.clipped),
            info.oldSwapchain);
    }
#else
    MAGMA_UNUSED(debugReportCallback);
#endif // VK_EXT_debug_report
    switch (create) {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("failed to create swapchain");
    case VK_ERROR_DEVICE_LOST:
        throw exception::DeviceLost("failed to create swapchain");
    case VK_ERROR_SURFACE_LOST_KHR:
        throw exception::SurfaceLost("failed to create swapchain");
#ifdef VK_KHR_display_swapchain
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        throw exception::IncompatibleDisplay("failed to create swapchain");
#endif
    }
    MAGMA_THROW_FAILURE(create, "failed to create swapchain");
}

Swapchain::~Swapchain()
{
    vkDestroySwapchainKHR(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

uint32_t Swapchain::acquireNextImage(std::shared_ptr<const Semaphore> semaphore, std::shared_ptr<const Fence> fence,
    uint64_t timeout /* UINT64_MAX */)
{
    uint32_t imageIndex = 0;
    const VkResult acquire = vkAcquireNextImageKHR(MAGMA_HANDLE(device), handle, timeout,
        MAGMA_OPTIONAL_HANDLE(semaphore),
        MAGMA_OPTIONAL_HANDLE(fence),
        &imageIndex);
    switch (acquire) {
    case VK_ERROR_DEVICE_LOST:
        throw exception::DeviceLost("failed to acquire next image");
    case VK_ERROR_OUT_OF_DATE_KHR:
        throw exception::OutOfDate("failed to acquire next image");
    case VK_ERROR_SURFACE_LOST_KHR:
        throw exception::SurfaceLost("failed to acquire next image");
#ifdef VK_EXT_full_screen_exclusive
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        throw exception::FullScreenExclusiveModeLost("failed to acquire next image");
#endif
    } // switch
    MAGMA_THROW_FAILURE(acquire, "failed to acquire next image");
    return imageIndex;
}

uint32_t Swapchain::getImageCount() const
{
    uint32_t imageCount;
    const VkResult get = vkGetSwapchainImagesKHR(MAGMA_HANDLE(device), handle, &imageCount, nullptr);
    MAGMA_THROW_FAILURE(get, "failed to get swapchain image count");
    return imageCount;
}

std::vector<std::shared_ptr<SwapchainColorAttachment2D>> Swapchain::getImages() const
{
    uint32_t imageCount = getImageCount();
    MAGMA_STACK_ARRAY(VkImage, swapchainImages, imageCount);
    const VkResult get = vkGetSwapchainImagesKHR(MAGMA_HANDLE(device), handle, &imageCount, swapchainImages);
    MAGMA_THROW_FAILURE(get, "failed to get swapchain images");
    std::vector<std::shared_ptr<SwapchainColorAttachment2D>> colorAttachments;
    for (const VkImage image : swapchainImages)
        colorAttachments.emplace_back(new SwapchainColorAttachment2D(device, image, surfaceFormat.format, extent));
    return colorAttachments;
}
} // namespace magma
