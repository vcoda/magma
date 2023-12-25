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
#pragma once
#include "swapchain.h"

namespace magma
{
    /* Allows applications to set the policy for swapchain creation
       and presentation mechanisms relating to full-screen access.
       Implementations may be able to acquire exclusive access to
       a particular display for an application window that covers
       the whole screen. This can increase performance on some
       systems by bypassing composition, however it can also result
       in disruptive or expensive transitions in the underlying
       windowing system when a change occurs. */

#if defined(VK_KHR_swapchain) && defined(VK_EXT_full_screen_exclusive)
    class FullScreenExclusiveSwapchain : public Swapchain
    {
    public:
        explicit FullScreenExclusiveSwapchain(std::shared_ptr<Device> device,
            std::shared_ptr<const Surface> surface,
            uint32_t minImageCount,
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            VkImageUsageFlags imageUsage,
            VkSurfaceTransformFlagBitsKHR preTransform,
            VkCompositeAlphaFlagBitsKHR compositeAlpha,
            VkPresentModeKHR presentMode,
            VkSwapchainCreateFlagsKHR flags,
            VkFullScreenExclusiveEXT fullScreenExclusive,
            // Allows to specify full-screen exclusive mode for physical display
            // that is represented by a monitor handle of type HMONITOR. See:
            // https://docs.microsoft.com/en-us/windows/win32/gdi/hmonitor-and-the-device-context
        #ifdef VK_KHR_win32_surface
            HMONITOR hMonitor = NULL,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<Swapchain> oldSwapchain = nullptr,
        #ifdef VK_EXT_debug_report
            std::shared_ptr<DebugReportCallback> debugReportCallback = nullptr,
        #endif
        #ifdef VK_EXT_debug_utils
            std::shared_ptr<DebugUtilsMessenger> debugUtilsMessenger = nullptr,
        #endif
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        void acquireFullScreenExclusiveMode();
        void releaseFullScreenExclusiveMode();
        bool fullScreenExclusiveMode() const noexcept { return fullScreenExclusive; }
    #ifdef VK_KHR_win32_surface
        HMONITOR getMonitorHandle() const noexcept { return hMonitor; }
    #endif

    private:
        bool fullScreenExclusive;
    #ifdef VK_KHR_win32_surface
        const HMONITOR hMonitor;
    #endif
    };
#endif // VK_KHR_swapchain && VK_EXT_full_screen_exclusive
} // namespace magma
