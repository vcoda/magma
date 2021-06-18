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
#include "nondispatchable.h"

namespace magma
{
    class Surface;
    class SwapchainColorAttachment;
    class Queue;
    class Fence;
    class Semaphore;
    class DebugReportCallback;

    /* Swapchain provides the ability to present rendering results to a surface.
       It is an abstraction for an array of presentable images that are associated with a surface.
       One image (which can be an array image for multiview/stereoscopic-3D surfaces)
       is displayed at a time, but multiple images can be queued for presentation.
       An application renders to the image, and then queues the image for presentation to the surface. */

    class Swapchain : public NonDispatchable<VkSwapchainKHR>
    {
    public:
        explicit Swapchain(std::shared_ptr<Device> device,
            std::shared_ptr<const Surface> surface,
            uint32_t minImageCount,
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& extent,
            VkImageUsageFlags usage,
            VkSurfaceTransformFlagBitsKHR preTransform,
            VkCompositeAlphaFlagBitsKHR compositeAlpha,
            VkPresentModeKHR presentMode,
            VkSwapchainCreateFlagsKHR flags,
            std::shared_ptr<IAllocator> allocator = nullptr,
            std::shared_ptr<const DebugReportCallback> debugReportCallback = nullptr);
        ~Swapchain();
        const VkExtent2D& getExtent() const noexcept { return extent; }
        uint32_t getImageIndex() const noexcept { return imageIndex; }
        uint32_t getImageCount() const;
        std::vector<std::shared_ptr<SwapchainColorAttachment>> getImages() const;
        uint32_t acquireNextImage(std::shared_ptr<const Semaphore> semaphore,
            std::shared_ptr<const Fence> fence,
            uint64_t timeout = UINT64_MAX);

    private:
        const VkSurfaceFormatKHR surfaceFormat;
        const VkExtent2D extent;
        uint32_t imageIndex;
    };
} // namespace magma
