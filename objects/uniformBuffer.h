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
#include <algorithm>
#include <functional>
#include "buffer.h"
#include "device.h"
#include "physicalDevice.h"
#include "../mem/typedefs.h"

namespace magma
{
    template<typename Block>
    class UniformBuffer : public Buffer
    {
    public:
        UniformBuffer(std::shared_ptr<const Device> device,
            uint32_t arraySize = 1,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr):
            Buffer(device, sizeof(Block) * arraySize, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                flags, allocator,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
            arraySize(arraySize)
        {
            static_assert(sizeof(Block)%16 == 0, "uniform block should have 16-byte alignment");
        }
        Block *map(ZeroMemoryFunction zeroFn = nullptr) noexcept
        {
            if (void *block = memory->map(0, size))
            {
                if (zeroFn)
                    zeroFn(block, static_cast<size_t>(size));
                return reinterpret_cast<Block *>(block);
            }
            return nullptr;
        }
        void unmap() noexcept
        { 
            memory->unmap();
        }
        virtual uint32_t getArraySize() const { return arraySize; }

    protected:
        const uint32_t arraySize;
    };

    template<typename Type>
    class DynamicUniformBuffer : public UniformBuffer<Type>
    {
    public:
        DynamicUniformBuffer(std::shared_ptr<const Device> device,
            uint32_t arraySize,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr):
            UniformBuffer<Type>(device, alignedArraySize(device, arraySize), flags, allocator),
            alignment(std::max(
                minOffsetAlignment(device),
                static_cast<VkDeviceSize>(elementSize)
            ))
        {}
        virtual uint32_t getArraySize() const override
        { 
            if (elementSize >= alignment)
                return UniformBuffer<Type>::arraySize;
            const VkDeviceSize divisor = alignment/elementSize;
            return static_cast<uint32_t>(UniformBuffer<Type>::arraySize/divisor);
        }
        uint32_t getDynamicOffset(uint32_t index) const
        {
            return static_cast<uint32_t>(index * alignment);
        }
        VkDeviceSize getElementAlignment() const { return alignment; }

    private:
        VkDeviceSize minOffsetAlignment(std::shared_ptr<const Device> device) const
        {   // Check hardware requirements
            std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
            const VkPhysicalDeviceProperties& properties = physicalDevice->getProperties();
            const VkPhysicalDeviceLimits& limits = properties.limits;
            return limits.minUniformBufferOffsetAlignment;
        }
        uint32_t alignedArraySize(std::shared_ptr<const Device> device, uint32_t arraySize) const
        {
            const VkDeviceSize alignment = minOffsetAlignment(device);
            if (elementSize >= alignment)
                return arraySize;
            const VkDeviceSize multiplier = alignment/elementSize;
            return static_cast<uint32_t>(arraySize * multiplier);
        }

    private:
        static constexpr size_t elementSize = sizeof(Type);
        const VkDeviceSize alignment;
    };
} // namespace magma
