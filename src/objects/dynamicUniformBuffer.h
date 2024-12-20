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
#include "baseUniformBuffer.h"
#include "../helpers/alignedUniformArray.h"
#include "../exceptions/exception.h"

namespace magma
{
    /* Base class of dynamic uniform buffer. */

    class BaseDynamicUniformBuffer : public BaseUniformBuffer
    {
    public:
        VkDeviceSize getAlignment() const noexcept override { return alignment; }
        uint32_t getDynamicOffset(uint32_t index) const noexcept { return static_cast<uint32_t>(index * alignment); }
        VkDescriptorBufferInfo getDescriptor() const noexcept override;
        bool dynamic() const noexcept override { return true; }

    protected:
        BaseDynamicUniformBuffer(std::shared_ptr<Device> device,
            std::size_t typeSize,
            uint32_t arraySize,
            VkMemoryPropertyFlags memoryFlags,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator,
            bool mappedPersistently);

    private:
        static uint32_t calculateAlignedArraySize(const Device *device,
            std::size_t typeSize,
            uint32_t arraySize) noexcept;

        VkDeviceSize alignment;
    };

    /* An array of aligned uniform values that can be fetched
       from buffer with dynamic offset. Alignment is determined
       by hardware requirements. To access elements of a buffer,
       iterator of AlignedUniformArray should be used. */

    template<class Type>
    class DynamicUniformBuffer : public BaseDynamicUniformBuffer
    {
    public:
        typedef Type UniformType;
        explicit DynamicUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            bool stagedPool = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            BaseDynamicUniformBuffer(std::move(device), sizeof(Type), arraySize,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                    (stagedPool ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0),
                optional, sharing, std::move(allocator),
                false)
        {}

        helpers::AlignedUniformArray<Type> mapToArray(uint32_t firstIndex = 0,
            uint32_t arraySize = std::numeric_limits<uint32_t>::max())
        {
            void *data = mapRange(firstIndex, arraySize);
            if (!data)
                MAGMA_ERROR("failed to map range of dynamic uniform buffer");
            return helpers::AlignedUniformArray<Type>(data, arraySize, getAlignment());
        }
    };

    /* Host visible non-coherent uniform buffer whose values
       can be fetched with dynamic offset. Alignment is
       determined by hardware requirements. To access elements
       of a buffer, iterator of AlignedUniformArray should be used.
       Requires to flush mapped memory range after data upload.
       Mapping and unmapping uniform buffer have a CPU cost.
       Therefore, if updated frequently, user may define it
       as persistenly mapped. */

    template<class Type>
    class NonCoherentDynamicUniformBuffer : public BaseDynamicUniformBuffer
    {
    public:
        typedef Type UniformType;
        explicit NonCoherentDynamicUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            bool mappedPersistently = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            BaseDynamicUniformBuffer(std::move(device), sizeof(Type), arraySize,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                optional, sharing, std::move(allocator),
                mappedPersistently)
        {}

        helpers::AlignedUniformArray<Type> mapToArray(uint32_t firstIndex = 0,
            uint32_t arraySize = std::numeric_limits<uint32_t>::max())
        {
            void *data = mapRange(firstIndex, arraySize);
            if (!data)
                MAGMA_ERROR("failed to map range of non-coherent dynamic uniform buffer");
            return helpers::AlignedUniformArray<Type>(data, arraySize, getAlignment());
        }

        bool flushMappedRange(const helpers::AlignedUniformArray<Type>& array) noexcept
        {
            if (!memory->mapped())
                return false;
            VkDeviceSize offset = array.getFirstIndex() * getAlignment();
            const VkDeviceSize nonCoherentAtomSize = getNonCoherentAtomSize();
            if (offset % nonCoherentAtomSize)
                offset = offset / nonCoherentAtomSize * nonCoherentAtomSize;
            VkDeviceSize size = array.getUpdatedRange() * getAlignment();
            const VkDeviceSize minFlushSize = std::min(memory->getSize(), nonCoherentAtomSize);
            size = std::max(size, minFlushSize);
            return memory->flushMappedRange(offset + getMapOffset(), size);
        }
    };

    template<class Type> using DynamicUniformBufferUPtr = std::unique_ptr<DynamicUniformBuffer<Type>>;
    template<class Type> using DynamicUniformBufferSPtr = std::shared_ptr<DynamicUniformBuffer<Type>>;
    template<class Type> using NonCoherentDynamicUniformBufferUPtr = std::unique_ptr<NonCoherentDynamicUniformBuffer<Type>>;
    template<class Type> using NonCoherentDynamicUniformBufferSPtr = std::shared_ptr<NonCoherentDynamicUniformBuffer<Type>>;
} // namespace magma
