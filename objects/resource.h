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
#pragma once
#include "nondispatchable.h"
#include "../allocator/allocator.h"
#include "../misc/sharing.h"
#include "../exceptions/exception.h"

namespace magma
{
    class IDeviceMemory;
    class PhysicalDevice;
    class CommandBuffer;
    class Queue;

    /* Vulkan supports two primary resource types: buffers and images.
       Resources are views of memory with associated formatting and
       dimensionality. Buffers and images are created with a sharing mode
       controlling how they can be accessed from queues. */

    class Resource : core::NonCopyable
    {
    public:
        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        const Sharing& getSharing() const noexcept { return sharing; }
        const std::shared_ptr<IDeviceMemory>& getMemory() const noexcept { return memory; }
        const std::shared_ptr<IDeviceMemoryAllocator>& getDeviceAllocator() const noexcept { return deviceAllocator; }
        virtual void bindMemory(std::shared_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) = 0;
    #ifdef VK_KHR_device_group
        virtual void bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) = 0;
    #endif // VK_KHR_device_group
        virtual void onDefragment() = 0;

    protected:
        explicit Resource(const Sharing& sharing,
            std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator) noexcept;
        void commitAndWait(std::shared_ptr<CommandBuffer> cmdBuffer);

        VkDeviceSize size;
        VkDeviceSize offset;
        const Sharing sharing;
        std::shared_ptr<IDeviceMemory> memory;
        std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator;
    };

    /* Non-dispatchable resource object (buffer, image,
       acceleration structure etc.) that has template
       declaration to handle different resource types. */

    template<class Self, class Type>
    class NonDispatchableResource :
        public NonDispatchable<Type>,
        public Resource,
        public std::enable_shared_from_this<Self>
    {
    protected:
        explicit NonDispatchableResource(VkObjectType objectType,
            std::shared_ptr<Device> device,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator) noexcept:
            NonDispatchable<Type>(objectType, std::move(device), MAGMA_HOST_ALLOCATOR(allocator)),
            Resource(sharing, MAGMA_DEVICE_ALLOCATOR(allocator))
        {}
    };
} // namespace magma
