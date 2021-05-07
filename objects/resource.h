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

    /* Base resource class. */

    class Resource
    {
    public:
        class Sharing;
        virtual void onDefragmentation() = 0;
    };

    /* Buffer and image objects are created with a sharing mode
       controlling how they can be accessed from queues. */

    class Resource::Sharing
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

    /* Vulkan supports two primary resource types: buffers and images.
       Resources are views of memory with associated formatting and dimensionality. */

    template<typename Child, typename Type>
    class NonDispatchableResource :
        public NonDispatchable<Type>,
        public Resource,
        public std::enable_shared_from_this<Child>
    {
    public:
        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        std::shared_ptr<DeviceMemory> getMemory() noexcept { return memory; }
        std::shared_ptr<const DeviceMemory> getMemory() const noexcept { return memory; }
        std::shared_ptr<IDeviceMemoryAllocator> getDeviceAllocator() const noexcept { return deviceAllocator; }

    protected:
        explicit NonDispatchableResource(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept:
            NonDispatchable<Type>(objectType, std::move(device), std::move(allocator)),
            size(0),
            offset(0)
        {}

        explicit NonDispatchableResource(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator) noexcept:
            NonDispatchable<Type>(objectType, std::move(device), MAGMA_HOST_ALLOCATOR(allocator)),
            size(0),
            offset(0),
            deviceAllocator(MAGMA_DEVICE_ALLOCATOR(allocator))
        {}

        VkDeviceSize size;
        VkDeviceSize offset;
        std::shared_ptr<DeviceMemory> memory;
        std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator;
    };
} // namespace magma
