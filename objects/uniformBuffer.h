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
#include <type_traits>
#include "buffer.h"
#include "device.h"
#include "physicalDevice.h"

namespace magma
{
    /* An array of uniform values that are used in various shader stages.
       It is host visible so can be mapped by user to write uniform values. */

    template<typename Type>
    class UniformBuffer : public Buffer
    {
    public:
        typedef Type UniformType;

        explicit UniformBuffer(std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator = nullptr,
            bool pinnedMemory = false,
            uint32_t arraySize = 1,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing()):
            Buffer(std::move(device), sizeof(Type) * arraySize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                (pinnedMemory ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0) | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                flags, sharing, std::move(allocator)),
            arraySize(arraySize)
        {
            static_assert(std::alignment_of<Type>() == 16, "uniform type should have 16-byte alignment");
        }

        Type *map(ZeroMemoryFunction zeroFn = nullptr) noexcept
        {
            if (memory)
            {
                if (void *data = memory->map(0, size))
                {
                    if (zeroFn)
                        zeroFn(data, static_cast<std::size_t>(size));
                    return reinterpret_cast<Type *>(data);
                }
            }
            return nullptr;
        }

        void unmap() noexcept
        {
            if (memory)
                memory->unmap();
        }

        virtual uint32_t getArraySize() const noexcept
        {
            return arraySize;
        }

    protected:
        const uint32_t arraySize;
    };

    /* An array of aligned uniform values that can be fetched from buffer with dynamic offset.
       Alignment is determined by hardware requirements. To access elements of a buffer,
       iterator of AlignedUniformArray should be used. */

    template<typename Type>
    class DynamicUniformBuffer : public UniformBuffer<Type>
    {
    public:
        typedef Type UniformType;

        explicit DynamicUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            std::shared_ptr<Allocator> allocator = nullptr,
            bool pinnedMemory = false,
            VkBufferCreateFlags flags = 0,
            const Resource::Sharing& sharing = Resource::Sharing()):
            UniformBuffer<Type>(device, std::move(allocator), pinnedMemory, alignedArraySize(device, arraySize), flags, sharing),
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

        VkDeviceSize getElementAlignment() const noexcept
        {
            return alignment;
        }

    private:
        static VkDeviceSize minOffsetAlignment(std::shared_ptr<Device> device) noexcept
        {   // Check hardware requirements
            std::shared_ptr<const PhysicalDevice> physicalDevice = std::move(device->getPhysicalDevice());
            const VkPhysicalDeviceProperties& properties = physicalDevice->getProperties();
            const VkPhysicalDeviceLimits& limits = properties.limits;
            return limits.minUniformBufferOffsetAlignment;
        }

        static uint32_t alignedArraySize(std::shared_ptr<Device> device, uint32_t arraySize) noexcept
        {
            const VkDeviceSize alignment = minOffsetAlignment(std::move(device));
            if (elementSize >= alignment)
                return arraySize;
            const VkDeviceSize multiplier = alignment/elementSize;
            return static_cast<uint32_t>(arraySize * multiplier);
        }

    private:
        static constexpr std::size_t elementSize = sizeof(Type);
        const VkDeviceSize alignment;
    };

    template<typename Type> using UniformBufferPtr = std::shared_ptr<UniformBuffer<Type>>;
    template<typename Type> using DynamicUniformBufferPtr = std::shared_ptr<DynamicUniformBuffer<Type>>;
} // namespace magma
