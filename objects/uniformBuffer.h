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
#include "../typedefs.h"

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
            Buffer(std::move(device), sizeof(Block) * arraySize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                flags, std::move(allocator),
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
            arraySize(arraySize)
        {
            static_assert(sizeof(Block)%16 == 0, "uniform block should have 16-byte alignment");
        }
        Block *map(ZeroMemoryFunction zeroFn = nullptr) noexcept
        {
            if (memory)
            {
                if (void *block = memory->map(0, size))
                {
                    if (zeroFn)
                        zeroFn(block, static_cast<size_t>(size));
                    return reinterpret_cast<Block *>(block);
                }
            }
            return nullptr;
        }
        void unmap() noexcept
        {
            if (memory)
                memory->unmap();
        }
        virtual uint32_t getArraySize() const noexcept { return arraySize; }

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
            UniformBuffer<Type>(device, alignedArraySize(device, arraySize), flags, std::move(allocator)),
            alignment(std::max(
                minOffsetAlignment(device),
                static_cast<VkDeviceSize>(elementSize)
            ))
        {}
        virtual uint32_t getArraySize() const noexcept override
        {
            if (elementSize >= alignment)
                return UniformBuffer<Type>::arraySize;
            const VkDeviceSize divisor = alignment/elementSize;
            return static_cast<uint32_t>(UniformBuffer<Type>::arraySize/divisor);
        }
        uint32_t getDynamicOffset(uint32_t index) const noexcept
        {
            return static_cast<uint32_t>(index * alignment);
        }
        VkDeviceSize getElementAlignment() const noexcept { return alignment; }

    private:
        VkDeviceSize minOffsetAlignment(std::shared_ptr<const Device> device) const noexcept
        {   // Check hardware requirements
            std::shared_ptr<const PhysicalDevice> physicalDevice = std::move(device->getPhysicalDevice());
            const VkPhysicalDeviceProperties& properties = physicalDevice->getProperties();
            const VkPhysicalDeviceLimits& limits = properties.limits;
            return limits.minUniformBufferOffsetAlignment;
        }
        uint32_t alignedArraySize(std::shared_ptr<const Device> device, uint32_t arraySize) const noexcept
        {
            const VkDeviceSize alignment = minOffsetAlignment(std::move(device));
            if (elementSize >= alignment)
                return arraySize;
            const VkDeviceSize multiplier = alignment/elementSize;
            return static_cast<uint32_t>(arraySize * multiplier);
        }

    private:
        static constexpr size_t elementSize = sizeof(Type);
        const VkDeviceSize alignment;
    };

    template<typename Block> using UniformBufferPtr = std::shared_ptr<UniformBuffer<Block>>;
    template<typename Block> using DynamicUniformBufferPtr = std::shared_ptr<DynamicUniformBuffer<Block>>;
} // namespace magma
