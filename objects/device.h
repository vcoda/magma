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
    class PhysicalDevice;
    class Queue;
    class Fence;
    class IAllocator;

    class Device : public Dispatchable<VkDevice>,
        public std::enable_shared_from_this<Device>
    {
        Device(std::shared_ptr<const PhysicalDevice> physicalDevice,
            const VkDeviceCreateInfo& info,
            const std::vector<VkDeviceQueueCreateInfo>& queues,
            std::shared_ptr<IAllocator> allocator = nullptr);
        friend PhysicalDevice;

    public:
        ~Device();
        std::shared_ptr<const PhysicalDevice> getPhysicalDevice() const { return physicalDevice; }
        std::shared_ptr<Queue> getQueue(VkQueueFlagBits flags, uint32_t queueIndex) const;
        bool waitIdle() const noexcept;
        bool resetFences(std::vector<std::shared_ptr<const Fence>>& fences) const noexcept;
        bool waitForFences(std::vector<std::shared_ptr<const Fence>>& fences, 
            bool waitAll, uint64_t timeout = UINT64_MAX) const noexcept;

    private:
        std::shared_ptr<const PhysicalDevice> physicalDevice;
        std::vector<VkDeviceQueueCreateInfo> queues;
    };
} // namespace magma
