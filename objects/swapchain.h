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
#pragma once
#include <vector>
#include "handle.h"

namespace magma
{
    class Device;
    class Surface;
    class Image;
    class Queue;
    class Semaphore;
    class Fence;

    class Swapchain : public NonDispatchable<VkSwapchainKHR>
    {
    public:
	    Swapchain(std::shared_ptr<const Device> device, 
            std::shared_ptr<const Surface> surface, 
            uint32_t minImageCount, 
            VkSurfaceFormatKHR surfaceFormat,
            const VkExtent2D& imageExtent,
		    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR, 
		    VkSwapchainCreateFlagsKHR flags = 0);
	    ~Swapchain();
        uint32_t acquireNextImage(
            std::shared_ptr<const Semaphore> semaphore, 
            std::shared_ptr<const Fence> fence) noexcept;
        uint32_t getImageCount() const noexcept;
        const std::vector<std::shared_ptr<Image>>& getImages() const noexcept;

    private:
        VkSurfaceFormatKHR surfaceFormat;
        mutable std::vector<std::shared_ptr<Image>> images;
    };
} // namespace magma
