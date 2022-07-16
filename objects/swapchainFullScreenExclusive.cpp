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
#include "swapchainFullScreenExclusive.h"
#include "device.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#if defined(VK_KHR_swapchain) && defined(VK_EXT_full_screen_exclusive)
class SurfaceFullScreenExclusiveInfo : public CreateInfo
{
public:
    SurfaceFullScreenExclusiveInfo(VkFullScreenExclusiveEXT fullScreenExclusive,
        const CreateInfo& chainedInfo = CreateInfo()) noexcept
    {
        fullScreenExclusiveSurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
        fullScreenExclusiveSurfaceInfo.pNext = (void *)chainedInfo.getNode(); // API mistake
        fullScreenExclusiveSurfaceInfo.fullScreenExclusive = fullScreenExclusive;
    }

    const void *getNode() const noexcept override
    {
        return &fullScreenExclusiveSurfaceInfo;
    }

private:
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveSurfaceInfo;
};

FullScreenExclusiveSwapchain::FullScreenExclusiveSwapchain(std::shared_ptr<Device> device, std::shared_ptr<const Surface> surface,
    uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& extent,
    VkImageUsageFlags usage,
    VkSurfaceTransformFlagBitsKHR preTransform,
    VkCompositeAlphaFlagBitsKHR compositeAlpha,
    VkPresentModeKHR presentMode,
    VkSwapchainCreateFlagsKHR flags,
    VkFullScreenExclusiveEXT fullScreenExclusive,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<Swapchain> oldSwapchain /* nullptr */,
    std::shared_ptr<const DebugReportCallback> debugReportCallback /* nullptr */,
    const CreateInfo& chainedInfo /* default */):
    Swapchain(std::move(device),
        std::move(surface),
        minImageCount,
        surfaceFormat,
        extent,
        usage,
        preTransform,
        compositeAlpha,
        presentMode,
        flags,
        std::move(allocator),
        std::move(oldSwapchain),
        std::move(debugReportCallback),
        SurfaceFullScreenExclusiveInfo(fullScreenExclusive, chainedInfo)),
    fullScreenExlusive(false)
{}

void FullScreenExclusiveSwapchain::acquireFullScreenExclusiveMode()
{
    // To omit window compositor (which may result in better performance),
    // your application should create borderless window that spans to entire monitor.
    // For example, set window style as WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS and call
    // SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW).
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkAcquireFullScreenExclusiveModeEXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkAcquireFullScreenExclusiveModeEXT(MAGMA_HANDLE(device), handle);
    handleError(result, "failed to acquire full-screen exclusive mode");
    fullScreenExlusive = true;
}

void FullScreenExclusiveSwapchain::releaseFullScreenExclusiveMode()
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkReleaseFullScreenExclusiveModeEXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkReleaseFullScreenExclusiveModeEXT(MAGMA_HANDLE(device), handle);
    handleError(result, "failed to release full-screen exclusive mode");
    fullScreenExlusive = false;
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
class SurfaceFullScreenExclusiveWin32Info : public CreateInfo
{
public:
    SurfaceFullScreenExclusiveWin32Info(HMONITOR hMonitor,
        const CreateInfo& chainedInfo = CreateInfo()) noexcept
    {
        fullScreenExclusiveWin32SurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
        fullScreenExclusiveWin32SurfaceInfo.pNext = chainedInfo.getNode();
        fullScreenExclusiveWin32SurfaceInfo.hmonitor = hMonitor;
    }

    const void *getNode() const noexcept override
    {
        return &fullScreenExclusiveWin32SurfaceInfo;
    }

private:
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32SurfaceInfo;
};

FullScreenExclusiveSwapchainWin32::FullScreenExclusiveSwapchainWin32(std::shared_ptr<Device> device, std::shared_ptr<const Surface> surface,
    uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& extent,
    VkImageUsageFlags usage,
    VkSurfaceTransformFlagBitsKHR preTransform,
    VkCompositeAlphaFlagBitsKHR compositeAlpha,
    VkPresentModeKHR presentMode,
    VkSwapchainCreateFlagsKHR flags,
    VkFullScreenExclusiveEXT fullScreenExclusive,
    HMONITOR hMonitor,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<Swapchain> oldSwapchain /* nullptr */,
    std::shared_ptr<const DebugReportCallback> debugReportCallback /* nullptr */,
    const CreateInfo& chainedInfo /* default */):
    FullScreenExclusiveSwapchain(std::move(device),
        std::move(surface),
        minImageCount,
        surfaceFormat,
        extent,
        usage,
        preTransform,
        compositeAlpha,
        presentMode,
        flags,
        fullScreenExclusive,
        std::move(allocator),
        std::move(oldSwapchain),
        std::move(debugReportCallback),
        SurfaceFullScreenExclusiveWin32Info(hMonitor, chainedInfo)),
    hMonitor(hMonitor)
{}
#endif // VK_USE_PLATFORM_WIN32_KHR
#endif // VK_KHR_swapchain && VK_EXT_full_screen_exclusive
} // namespace magma
