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
#include "../allocator/allocator.h"

namespace magma
{
    class DeviceMemory;
    class PhysicalDevice;

    /* Vulkan supports two primary resource types: buffers and images.
       Resources are views of memory with associated formatting and dimensionality.
       Buffers and images are created with a sharing mode
       controlling how they can be accessed from queues. */

    class Resource
    {
    public:
        class Sharing
        {
        public:
            Sharing() = default;
            Sharing(const std::vector<uint32_t>& queueFamilyIndices) noexcept;
            Sharing(std::shared_ptr<const PhysicalDevice> device,
                const std::initializer_list<VkQueueFlagBits>& queueTypes);
            VkSharingMode getMode() const noexcept;
            uint32_t getQueueFamiliesCount() const noexcept;
            const std::vector<uint32_t>& getQueueFamilyIndices() const noexcept { return queueFamilyIndices; }

        private:
            uint32_t chooseFamilyIndex(VkQueueFlagBits queueType,
                const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const noexcept;

            std::vector<uint32_t> queueFamilyIndices;
        };

        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        const Sharing& getSharing() const noexcept { return sharing; }
        std::shared_ptr<DeviceMemory> getMemory() noexcept { return memory; }
        std::shared_ptr<const DeviceMemory> getMemory() const noexcept { return memory; }
        std::shared_ptr<IDeviceMemoryAllocator> getDeviceAllocator() const noexcept { return deviceAllocator; }
        virtual void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0) = 0;
#ifdef VK_KHR_device_group
        virtual void bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            VkDeviceSize offset = 0) = 0;
#endif
        virtual void onDefragmented() = 0;

    protected:
        explicit Resource(const Sharing& sharing,
            std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator) noexcept;

        VkDeviceSize size;
        VkDeviceSize offset;
        const Sharing sharing;
        std::shared_ptr<DeviceMemory> memory;
        std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator;
    };

    /* Non-dispatchable resource object (buffer, image, acceleration structure etc.)
       that has template declaration to handle different resource types. */

    template<typename This, typename Type>
    class NonDispatchableResource :
        public NonDispatchable<Type>,
        public Resource,
        public std::enable_shared_from_this<This>
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
