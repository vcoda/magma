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
#include "uniformBufferAligned.h"

namespace magma
{
    /* An array of aligned uniform values that can be fetched
       from buffer with dynamic offset. Alignment is determined
       by hardware requirements. To access elements of a buffer,
       iterator of AlignedUniformArray should be used. */

    template<class Type>
    class DynamicUniformBuffer : public UniformBuffer<Type>,
        public AlignedUniformBuffer<Type>
    {
    public:
        typedef Type UniformType;
        explicit DynamicUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            bool barStagedMemory = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            UniformBuffer<Type>(std::move(device), barStagedMemory, std::move(allocator), getAlignedArraySize(device, arraySize), optional, sharing),
            AlignedUniformBuffer<Type>(this->device)
        {}

        VkDescriptorBufferInfo getDescriptor() const noexcept override
        {
            return getBufferDescriptor(UniformBuffer<Type>::handle);
        }

        uint32_t getArraySize() const noexcept override
        {
            return getAlignedArraySize(UniformBuffer<Type>::arraySize);
        }
    };

    /* Host visible non-coherent uniform buffer whose values
       can be fetched with dynamic offset. Alignment is
       determined by hardware requirements. To access elements
       of a buffer, iterator of AlignedUniformArray should be used.
       Requires to flush mapped memory range after data upload.
       Mapping and unmapping uniform buffer have a CPU cost,
       so it is persistenly mapped. */

    template<class Type>
    class NonCoherentDynamicUniformBuffer : public NonCoherentUniformBuffer<Type>,
        public AlignedUniformBuffer<Type>
    {
    public:
        explicit NonCoherentDynamicUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            NonCoherentUniformBuffer(device, std::move(allocator), true, getAlignedArraySize(device, arraySize), optional, sharing),
            AlignedUniformBuffer<Type>(std::move(device))
        {}

        VkDescriptorBufferInfo getDescriptor() const noexcept override
        {
            return getBufferDescriptor(NonCoherentUniformBuffer<Type>::handle);
        }

        uint32_t getArraySize() const noexcept override
        {
            return getAlignedArraySize(NonCoherentUniformBuffer<Type>::arraySize);
        }
    };

    template<class Type> using DynamicUniformBufferPtr = std::shared_ptr<DynamicUniformBuffer<Type>>;
    template<class Type> using NonCoherentDynamicUniformBufferPtr = std::shared_ptr<NonCoherentDynamicUniformBuffer<Type>>;
} // namespace magma
