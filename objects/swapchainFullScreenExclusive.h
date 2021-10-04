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
#pragma once
#include "swapchain.h"

namespace magma
{
    /* Allows applications to set the policy for swapchain creation and presentation mechanisms
       relating to full-screen access. Implementations may be able to acquire exclusive access
       to a particular display for an application window that covers the whole screen.
       This can increase performance on some systems by bypassing composition, however it can also result
       in disruptive or expensive transitions in the underlying windowing system when a change occurs. */

#ifdef VK_EXT_full_screen_exclusive
    class FullScreenExclusiveSwapchain : public Swapchain
    {
    public:
        explicit FullScreenExclusiveSwapchain(std::shared_ptr<Device> device,
            std::shared_ptr<const Surface> surface,
            uint32_t minImageCount,
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& extent,
            VkImageUsageFlags usage,
            VkSurfaceTransformFlagBitsKHR preTransform,
            VkCompositeAlphaFlagBitsKHR compositeAlpha,
            VkPresentModeKHR presentMode,
            VkSwapchainCreateFlagsKHR flags,
            VkFullScreenExclusiveEXT fullScreenExclusive,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<const DebugReportCallback> debugReportCallback = nullptr,
            const CreateInfo& swapchainInfoEx = CreateInfo());
        void acquireFullScreenExclusiveMode();
        void releaseFullScreenExclusiveMode();
        bool hasFullScreenExclusiveMode() const noexcept { return fullScreenExlusive; }

    private:
        bool fullScreenExlusive;
    };

    /* Allows to specify full-screen exclusive mode for physical display
       that is represented by a monitor handle of type HMONITOR. See:
       https://docs.microsoft.com/en-us/windows/win32/gdi/hmonitor-and-the-device-context */

#ifdef VK_USE_PLATFORM_WIN32_KHR
    class FullScreenExclusiveSwapchainWin32 : public FullScreenExclusiveSwapchain
    {
    public:
        explicit FullScreenExclusiveSwapchainWin32(std::shared_ptr<Device> device,
            std::shared_ptr<const Surface> surface,
            uint32_t minImageCount,
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& extent,
            VkImageUsageFlags usage,
            VkSurfaceTransformFlagBitsKHR preTransform,
            VkCompositeAlphaFlagBitsKHR compositeAlpha,
            VkPresentModeKHR presentMode,
            VkSwapchainCreateFlagsKHR flags,
            VkFullScreenExclusiveEXT fullScreenExclusive,
            HMONITOR hMonitor,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<const DebugReportCallback> debugReportCallback = nullptr,
            const CreateInfo& swapchainInfoEx = CreateInfo());
        HMONITOR getMonitorHandle() const noexcept { return hMonitor; }

    private:
        HMONITOR hMonitor;
    };
#endif // VK_USE_PLATFORM_WIN32_KHR
#endif // VK_EXT_full_screen_exclusive
} // namespace magma
