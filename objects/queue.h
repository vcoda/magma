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
    class CommandBuffer;
    class Semaphore;
    class Fence;
    class Swapchain;

    class Queue : public Handle<VkQueue>
    {
    public:
        Queue(std::shared_ptr<const Device> device, 
            uint32_t queueFamilyIndex = 0, 
            uint32_t queueIndex = 0);
        bool submit(const std::vector<std::shared_ptr<const CommandBuffer>>& commandBuffers,
            VkPipelineStageFlags waitStageMask,
            const std::vector<std::shared_ptr<const Semaphore>>& waitSemaphores,
            const std::vector<std::shared_ptr<const Semaphore>>& signalSemaphores,
            std::shared_ptr<const Fence> fence = nullptr) noexcept;
        bool submit(std::shared_ptr<const CommandBuffer> commandBuffer, 
            VkPipelineStageFlags waitStageMask = 0,
            std::shared_ptr<const Semaphore> waitSemaphore = nullptr,
            std::shared_ptr<const Semaphore> signalSemaphore = nullptr,
            std::shared_ptr<const Fence> fence = nullptr) noexcept;
        bool waitIdle() noexcept;
        bool present(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
            std::shared_ptr<const Semaphore> waitSemaphore = nullptr) noexcept;
    };
} // namespace magma
