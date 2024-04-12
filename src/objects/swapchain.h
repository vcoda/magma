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
#include "nondispatchable.h"
#include "../misc/sharing.h"
#include "../misc/structureChain.h"

namespace magma
{
    class Surface;
    class SwapchainImage;
    class Queue;
    class Fence;
    class Semaphore;
#ifdef VK_EXT_debug_report
    class DebugReportCallback;
#endif
#ifdef VK_EXT_debug_utils
    class DebugUtilsMessenger;
#endif

    /* Swapchain provides the ability to present rendering
       results to a surface. It is an abstraction for an array
       of presentable images that are associated with a surface.
       One image (which can be an array image for multiview or
       stereoscopic 3D surfaces) is displayed at a time, but
       multiple images can be queued for presentation. An
       application renders to the image, and then queues the
       image for presentation to the surface. */

#ifdef VK_KHR_swapchain
    class Swapchain : public NonDispatchable<VkSwapchainKHR>
    {
    public:
        explicit Swapchain(std::shared_ptr<Device> device,
            std::shared_ptr<const Surface> surface,
            uint32_t minImageCount,
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            VkImageUsageFlags imageUsage,
            VkSurfaceTransformFlagBitsKHR preTransform,
            VkCompositeAlphaFlagBitsKHR compositeAlpha,
            VkPresentModeKHR presentMode,
        #ifdef VK_EXT_swapchain_maintenance1
            const std::vector<VkPresentModeKHR>& presentModes = {},
        #endif
        #ifdef VK_KHR_device_group
            VkDeviceGroupPresentModeFlagsKHR deviceGroupPresentModes = 0,
        #endif
            VkSwapchainCreateFlagsKHR flags = 0,
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
        ~Swapchain();
        const VkSurfaceFormatKHR& getSurfaceFormat() const noexcept { return surfaceFormat; }
        const VkExtent2D& getExtent() const noexcept { return extent; }
        uint32_t getArrayLayers() const noexcept { return arrayLayers; }
        VkImageUsageFlags getImageUsage() const noexcept { return imageUsage; }
        VkSwapchainCreateFlagsKHR getFlags() const noexcept { return flags; }
        VkPresentModeKHR getPresentMode() const noexcept { return presentMode; }
        const Sharing& getSharing() const noexcept { return sharing; }
        bool hadRetired() const noexcept { return retired; }
        uint32_t getImageIndex() const noexcept { return imageIndex; }
        uint32_t getImageCount() const;
        const std::vector<std::shared_ptr<SwapchainImage>>& getImages() const;
        uint32_t acquireNextImage(std::shared_ptr<const Semaphore> semaphore = nullptr,
            std::shared_ptr<const Fence> fence = nullptr,
            uint64_t timeout = std::numeric_limits<uint64_t>::max());
    #ifdef VK_KHR_device_group
        uint32_t acquireNextDeviceGroupImage(uint32_t deviceMask,
            std::shared_ptr<const Semaphore> semaphore = nullptr,
            std::shared_ptr<const Fence> fence = nullptr,
            uint64_t timeout = std::numeric_limits<uint64_t>::max());
    #endif // VK_KHR_device_group
    #ifdef VK_KHR_bind_memory2
        void bindImage(std::shared_ptr<SwapchainImage> image,
            uint32_t imageIndex);
        #ifdef VK_KHR_device_group
        void bindImage(std::shared_ptr<SwapchainImage> image,
            uint32_t imageIndex,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {});
        #endif // VK_KHR_device_group
    #endif // VK_KHR_bind_memory2
    #ifdef VK_AMD_display_native_hdr
        void setLocalDimming(bool enable) noexcept;
    #endif

    protected:
        Swapchain(std::shared_ptr<Device> device,
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& extent,
            uint32_t arrayLayers,
            VkImageUsageFlags imageUsage,
            VkPresentModeKHR presentMode,
            VkSwapchainCreateFlagsKHR flags,
            const Sharing& sharing,
            std::shared_ptr<Swapchain> oldSwapchain,
            std::shared_ptr<IAllocator> allocator);
        void addImage(std::shared_ptr<SwapchainImage> image,
            uint32_t imageIndex);
        void handleError(VkResult result,
            const char *message) const;

        const VkSurfaceFormatKHR surfaceFormat;
        const VkExtent2D extent;
        const uint32_t arrayLayers;
        const VkImageUsageFlags imageUsage;
        const VkPresentModeKHR presentMode;
        const VkSwapchainCreateFlagsKHR flags;
        const Sharing sharing;
        bool retired;
        mutable std::vector<std::shared_ptr<SwapchainImage>> bindedImages;
        uint32_t imageIndex;
        friend class FullScreenExclusiveSwapchain;
    };
#endif // VK_KHR_swapchain
} // namespace magma

#ifdef VK_KHR_swapchain
std::ostream& operator<<(std::ostream&, const VkSwapchainCreateInfoKHR&);
#ifdef VK_KHR_device_group
std::ostream& operator<<(std::ostream&, const VkDeviceGroupSwapchainCreateInfoKHR&);
#endif
#endif // VK_KHR_swapchain
