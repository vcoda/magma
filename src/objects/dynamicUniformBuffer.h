/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "uniformBuffer.h"

namespace magma
{
    /* An array of aligned uniform values that can be fetched
       from buffer with dynamic offset. Alignment is determined
       by hardware requirements. To access elements of a buffer,
       iterator of AlignedUniformArray should be used. */

    template<class Type>
    class DynamicUniformBuffer : public UniformBuffer<Type>
    {
    public:
        typedef Type UniformType;

        explicit DynamicUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            bool barStagedMemory = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            UniformBuffer<Type>(device, barStagedMemory, std::move(allocator), getAlignedArraySize(device, arraySize), optional, sharing),
            alignment(std::max(
                getMinOffsetAlignment(device),
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
        static VkDeviceSize getMinOffsetAlignment(std::shared_ptr<Device>& device) noexcept
        {   // Check hardware requirements
            const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
            const VkPhysicalDeviceProperties& properties = physicalDevice->getProperties();
            const VkPhysicalDeviceLimits& limits = properties.limits;
            return limits.minUniformBufferOffsetAlignment;
        }

        static uint32_t getAlignedArraySize(std::shared_ptr<Device>& device, uint32_t arraySize) noexcept
        {
            const VkDeviceSize minOffsetAlignment = getMinOffsetAlignment(device);
            if (elementSize >= minOffsetAlignment)
                return arraySize;
            const VkDeviceSize multiplier = minOffsetAlignment/elementSize;
            return static_cast<uint32_t>(arraySize * multiplier);
        }

    private:
        static constexpr std::size_t elementSize = sizeof(Type);
        const VkDeviceSize alignment;
    };

    template<class Type> using DynamicUniformBufferPtr = std::shared_ptr<DynamicUniformBuffer<Type>>;
} // namespace magma
