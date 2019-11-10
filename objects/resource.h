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
#include "nondispatchable.h"

namespace magma
{
    class DeviceMemory;
    class PhysicalDevice;

    /* Base resource class. */

    class Resource
    {
    public:
        class Sharing
        {
        public:
            Sharing() = default;
            Sharing(const std::vector<uint32_t>& queueFamilyIndices);
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
        std::shared_ptr<DeviceMemory> getMemory() noexcept { return memory; }
        std::shared_ptr<const DeviceMemory> getMemory() const noexcept { return memory; }

    protected:
        Resource(VkDeviceSize size):
            size(size), offset(0) {}

        VkDeviceSize size;
        VkDeviceSize offset;
        std::shared_ptr<DeviceMemory> memory;
    };

    /* Vulkan supports two primary resource types: buffers and images.
       Resources are views of memory with associated formatting and dimensionality. */

    template<typename Child, typename Object>
    class NonDispatchableResource : public NonDispatchable<Object>, public Resource,
        public std::enable_shared_from_this<Child>
    {
    protected:
        explicit NonDispatchableResource(VkObjectType objectType,
            VkDeviceSize size,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator):
            NonDispatchable(objectType, std::move(device), std::move(allocator)),
            Resource(size)
        {}
    };
} // namespace magma
