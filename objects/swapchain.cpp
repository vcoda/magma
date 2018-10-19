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
#include "swapchain.h"
#include "device.h"
#include "surface.h"
#include "image2D.h"
#include "queue.h"
#include "semaphore.h"
#include "fence.h"
#include "debugReportCallback.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../helpers/stackArray.h"
#include "../helpers/stringize.h"

namespace magma
{
Swapchain::Swapchain(std::shared_ptr<Device> device, std::shared_ptr<const Surface> surface,
    uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& imageExtent,
    VkSurfaceTransformFlagBitsKHR preTransform,
    VkCompositeAlphaFlagBitsKHR compositeAlpha,
    VkPresentModeKHR presentMode,
    VkSwapchainCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<DebugReportCallback> debugReportCallback /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_SWAPCHAIN_KHR, std::move(device), std::move(allocator)),
    surfaceFormat(surfaceFormat),
    imageExtent(imageExtent)
{
    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.surface = *surface;
    info.minImageCount = minImageCount;
    info.imageFormat = surfaceFormat.format;
    info.imageColorSpace = surfaceFormat.colorSpace;
    info.imageExtent = imageExtent;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.preTransform = preTransform;
    info.compositeAlpha = compositeAlpha;
    info.presentMode = presentMode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = VK_NULL_HANDLE;
    VkResult create;
    const bool isDisplaySurface = std::dynamic_pointer_cast<const DisplaySurface>(surface) ? true : false;
    if (!isDisplaySurface)
        create = vkCreateSwapchainKHR(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    else
    {
        MAGMA_DEVICE_EXTENSION(vkCreateSharedSwapchainsKHR, VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME);
        create = vkCreateSharedSwapchainsKHR(MAGMA_HANDLE(device), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    }
#ifdef MAGMA_DEBUG
    if (create != VK_SUCCESS && debugReportCallback)
        dump(info, debugReportCallback);
#endif
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
    MAGMA_THROW_FAILURE(acquire, "failed to acquire swapchain image");
    return imageIndex;
}

uint32_t Swapchain::getImageCount() const
{
    uint32_t imageCount = 0;
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
    std::vector<std::shared_ptr<SwapchainColorAttachment2D>> images;
    for (const VkImage handle : swapchainImages)
        images.emplace_back(new SwapchainColorAttachment2D(device, handle, surfaceFormat.format, imageExtent));
    return images;
}

void Swapchain::dump(const VkSwapchainCreateInfoKHR& info,
    std::shared_ptr<DebugReportCallback> debugReportCallback) const noexcept
{
#ifdef MAGMA_DEBUG
    debugReportCallback->message(VK_DEBUG_REPORT_ERROR_BIT_EXT, objectType, VK_NULL_HANDLE, 0, 0, "Magma",
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
        info.clipped ? "VK_TRUE" : "VK_FALSE",
        info.oldSwapchain);
#elif defined(_MSC_VER)
    info;
    debugReportCallback;
#endif // MAGMA_DEBUG
}
} // namespace magma
