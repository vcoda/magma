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
            bool barStagedMemory = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            uint32_t arraySize = 1,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing()):
            Buffer(std::move(device), sizeof(Type) * arraySize,
                0, // flags
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                (barStagedMemory ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0) | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                optional, sharing, std::move(allocator)),
            arraySize(arraySize)
        {
            static_assert(std::alignment_of<Type>() == 16, "uniform type should have 16-byte alignment");
        }

        Type *map(bool zeroMemory = false) noexcept
        {
            if (memory)
            {
                if (void *data = memory->map(0, VK_WHOLE_SIZE))
                {
                    if (zeroMemory)
                        memset(data, 0, static_cast<std::size_t>(size));
                    return reinterpret_cast<Type *>(data);
                }
            }
            return nullptr;
        }

        void unmap() noexcept
        {
            if (memory)
            {
                if (memory->mapped())
                    memory->unmap();
            }
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
            bool barStagedMemory = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Descriptor& optional = Buffer::Descriptor(),
            const Sharing& sharing = Sharing()):
            UniformBuffer<Type>(device, barStagedMemory, std::move(allocator), alignedArraySize(device, arraySize), optional, sharing),
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
            std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
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
