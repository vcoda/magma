/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "fullScreenExclusiveSwapchain.h"
#include "device.h"
#include "surface.h"
#include "debugReportCallback.h"
#include "debugUtilsMessenger.h"
#include "../allocator/allocator.h"
#include "../misc/deviceFeatures.h"
#include "../misc/extension.h"
#include "../helpers/stringize.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#if defined(VK_KHR_swapchain) && defined(VK_EXT_full_screen_exclusive)
FullScreenExclusiveSwapchain::FullScreenExclusiveSwapchain(std::shared_ptr<Device> device, std::shared_ptr<const Surface> surface,
    uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& extent, uint32_t arrayLayers,
    VkImageUsageFlags imageUsage, VkSurfaceTransformFlagBitsKHR preTransform, VkCompositeAlphaFlagBitsKHR compositeAlpha,
    VkPresentModeKHR presentMode, VkSwapchainCreateFlagsKHR flags, VkFullScreenExclusiveEXT fullScreenExclusive,
#ifdef VK_KHR_win32_surface
    HMONITOR hMonitor /* NULL */,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<Swapchain> oldSwapchain /* nullptr */,
#ifdef VK_EXT_debug_report
    std::shared_ptr<DebugReportCallback> debugReportCallback /* nullptr */,
#endif
#ifdef VK_EXT_debug_utils
    std::shared_ptr<DebugUtilsMessenger> debugUtilsMessenger /* nullptr */,
#endif
    const Sharing& sharing /* default */,
    const StructureChain& extendedInfo /* default */):
    Swapchain(std::move(device), surfaceFormat, extent, arrayLayers, imageUsage, flags, sharing, oldSwapchain, std::move(allocator)),
    fullScreenExclusive(false)
#ifdef VK_KHR_win32_surface
   ,hMonitor(hMonitor)
#endif
{
    VkSwapchainCreateInfoKHR swapchainInfo;
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveInfo;
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.pNext = &fullScreenExclusiveInfo;
    swapchainInfo.flags = flags;
    swapchainInfo.surface = *surface;
    swapchainInfo.minImageCount = minImageCount;
    swapchainInfo.imageFormat = surfaceFormat.format;
    swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainInfo.imageExtent = extent;
    swapchainInfo.imageArrayLayers = arrayLayers;
    swapchainInfo.imageUsage = imageUsage;
    swapchainInfo.imageSharingMode = sharing.getMode();
    swapchainInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    swapchainInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    swapchainInfo.preTransform = preTransform;
    swapchainInfo.compositeAlpha = compositeAlpha;
    swapchainInfo.presentMode = presentMode;
    swapchainInfo.clipped = (imageUsage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT); // Is pixels readback required?
    swapchainInfo.oldSwapchain = MAGMA_OPTIONAL_HANDLE(oldSwapchain);
    fullScreenExclusiveInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT,
    fullScreenExclusiveInfo.pNext = (void *)extendedInfo.chainNodes();
    fullScreenExclusiveInfo.fullScreenExclusive = fullScreenExclusive;
#ifdef VK_KHR_win32_surface
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32Info;
    if (hMonitor)
    {
        fullScreenExclusiveInfo.pNext = &fullScreenExclusiveWin32Info;
        fullScreenExclusiveWin32Info.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT,
        fullScreenExclusiveWin32Info.pNext = (void *)extendedInfo.chainNodes();
        fullScreenExclusiveWin32Info.hmonitor = hMonitor;
    }
#endif // VK_KHR_win32_surface
    if (!device->getDeviceFeatures()->checkImageUsageSupport(surface, swapchainInfo.imageUsage))
        MAGMA_ERROR("swapchain usage not supported by surface");
    VkResult result;
#if defined(VK_KHR_display_swapchain) && defined(VK_KHR_display_surface)
    if (std::dynamic_pointer_cast<const DisplaySurface>(surface))
    {
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateSharedSwapchainsKHR, VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME);
        result = vkCreateSharedSwapchainsKHR(MAGMA_HANDLE(device), 1, &swapchainInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    }
    else
#endif // VK_KHR_display_swapchain && VK_KHR_display_surface
    {
        result = vkCreateSwapchainKHR(MAGMA_HANDLE(device), &swapchainInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    }
    if (oldSwapchain)
        oldSwapchain->retired = true; // oldSwapchain is retired even if creation of the new swapchain fails
#if defined(VK_EXT_debug_report) || defined(VK_EXT_debug_utils)
    if (result != VK_SUCCESS)
    {
        char errorStr[MAGMA_MAX_STRING];
        sprintf_s(errorStr, MAGMA_MAX_STRING, "initialization of fullscreen exclusive swapchain failed with the following parameters:\n"
            "minImageCount: %u\nimageFormat: %s\nimageColorSpace: %s\nimageExtent: {%u, %u}\nimageArrayLayers: %u\n"
            "imageUsage: %s\nimageSharingMode: %s\npreTransform: %s\ncompositeAlpha: %s\npresentMode: %s\nclipped: %s\n"
        #ifdef MAGMA_X64
            "oldSwapchain: %p\n"
        #else
            "oldSwapchain: %llu\n"
        #endif
        #ifdef VK_KHR_win32_surface
            "hMonitor: %p\n"
        #endif
            "fullScreenExclusive: %s\n",
            swapchainInfo.minImageCount,
            helpers::stringize(swapchainInfo.imageFormat),
            helpers::stringize(swapchainInfo.imageColorSpace),
            swapchainInfo.imageExtent.width,
            swapchainInfo.imageExtent.height,
            swapchainInfo.imageArrayLayers,
            helpers::stringifyImageUsageFlags(imageUsage).c_str(),
            helpers::stringize(swapchainInfo.imageSharingMode),
            helpers::stringize(swapchainInfo.preTransform),
            helpers::stringize(swapchainInfo.compositeAlpha),
            helpers::stringize(swapchainInfo.presentMode),
            helpers::stringize(swapchainInfo.clipped),
            swapchainInfo.oldSwapchain,
        #ifdef VK_KHR_win32_surface
            hMonitor,
        #endif
            helpers::stringize(fullScreenExclusiveInfo.fullScreenExclusive));
    #ifdef VK_EXT_debug_report
        if (debugReportCallback)
        {
            debugReportCallback->message(VK_DEBUG_REPORT_ERROR_BIT_EXT,
                getObjectType(),
                0/*VK_NULL_HANDLE*/,
                0, 0, "magma", errorStr);
        }
    #endif // VK_EXT_debug_report
    #ifdef VK_EXT_debug_utils
        if (debugUtilsMessenger)
        {
            debugUtilsMessenger->message(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
                "magma", 0, errorStr);
        }
    #endif // VK_EXT_debug_utils
    }
#endif // VK_EXT_debug_report || VK_EXT_debug_utils
    handleError(result, "failed to create fullscreen exclusive swapchain");
}

void FullScreenExclusiveSwapchain::acquireFullScreenExclusiveMode()
{
    // To omit window compositor (which may result in better performance),
    // your application should create borderless window that spans to entire monitor.
    // For example, set window style as WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS and call
    // SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW).
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkAcquireFullScreenExclusiveModeEXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkAcquireFullScreenExclusiveModeEXT(MAGMA_HANDLE(device), handle);
    handleError(result, "failed to acquire full-screen exclusive mode");
    fullScreenExclusive = true;
}

void FullScreenExclusiveSwapchain::releaseFullScreenExclusiveMode()
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkReleaseFullScreenExclusiveModeEXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkReleaseFullScreenExclusiveModeEXT(MAGMA_HANDLE(device), handle);
    handleError(result, "failed to release full-screen exclusive mode");
    fullScreenExclusive = false;
}
#endif // VK_KHR_swapchain && VK_EXT_full_screen_exclusive
} // namespace magma
