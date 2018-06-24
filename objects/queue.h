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
    class CommandBuffer;
    class Semaphore;
    class Fence;
    class Swapchain;

    class Queue : public Dispatchable<VkQueue>
    {
        Queue(VkQueue handle, std::shared_ptr<const Device> device,
            VkQueueFlagBits flags, uint32_t familyIndex, uint32_t index);
        friend Device;

    public:
        VkQueueFlagBits getFlags() const noexcept { return flags; }
        uint32_t getFamilyIndex() const noexcept { return familyIndex; }
        uint32_t getIndex() const noexcept { return index; }
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
        void present(std::shared_ptr<const Swapchain> swapchain, 
            uint32_t imageIndex,
            std::shared_ptr<const Semaphore> waitSemaphore = nullptr);
        void presentToDisplay(std::shared_ptr<const Swapchain> swapchain, 
            uint32_t imageIndex,
            const VkRect2D& srcRect, 
            const VkRect2D& dstRect, 
            bool persistent,
            std::shared_ptr<const Semaphore> waitSemaphore = nullptr);

    private:
        void present(std::shared_ptr<const Swapchain> swapchain, 
            uint32_t imageIndex,
            const VkDisplayPresentInfoKHR *displayPresentInfo,
            std::shared_ptr<const Semaphore> waitSemaphore);

    private:
        VkQueueFlagBits flags;
        uint32_t familyIndex;
        uint32_t index;
    };
} // namespace magma
