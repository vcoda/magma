/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_EXT_full_screen_exclusive
class SurfaceFullScreenExclusiveCreateInfo : public CreateInfo
{
public:
    SurfaceFullScreenExclusiveCreateInfo(VkFullScreenExclusiveEXT fullScreenExclusive,
        const CreateInfo& swapchainInfoEx = CreateInfo()) noexcept
    {
        fullScreenExclusiveInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
        fullScreenExclusiveInfo.pNext = (void *)swapchainInfoEx.getNode(); // API mistake
        fullScreenExclusiveInfo.fullScreenExclusive = fullScreenExclusive;
    }

    const void *getNode() const noexcept override
    {
        return &fullScreenExclusiveInfo;
    }

private:
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveInfo;
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
    std::shared_ptr<const DebugReportCallback> debugReportCallback /* nullptr */,
    const CreateInfo& swapchainInfoEx /* default */):
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
        std::move(debugReportCallback),
        SurfaceFullScreenExclusiveCreateInfo(fullScreenExclusive, swapchainInfoEx)),
    fullScreenExlusive(false)
{}

void FullScreenExclusiveSwapchain::acquireFullScreenExclusiveMode()
{
    // To omit window compositor (which may result in better performance),
    // your application should create borderless window that spans to entire monitor.
    // For example, set window style as WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS and call
    // SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW).
    MAGMA_DEVICE_EXTENSION(vkAcquireFullScreenExclusiveModeEXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkAcquireFullScreenExclusiveModeEXT(MAGMA_HANDLE(device), handle);
    const exception::source_location location{__FILE__, __LINE__, __FUNCTION__};
    switch (result)
    {
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        throw exception::OutOfHostMemory("failed to aquire full screen exclusive mode", location);
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        throw exception::OutOfDeviceMemory("failed to aquire full screen exclusive mode", location);
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("failed to aquire full screen exclusive mode");
    case VK_ERROR_SURFACE_LOST_KHR:
        throw exception::SurfaceLost("failed to aquire full screen exclusive mode");
    }
    fullScreenExlusive = true;
}

void FullScreenExclusiveSwapchain::releaseFullScreenExclusiveMode()
{
    MAGMA_DEVICE_EXTENSION(vkReleaseFullScreenExclusiveModeEXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkReleaseFullScreenExclusiveModeEXT(MAGMA_HANDLE(device), handle);
    MAGMA_THROW_FAILURE(result, "failed to release full screen exclusive mode");
    fullScreenExlusive = false;
}
#endif // VK_EXT_full_screen_exclusive
} // namespace magma
