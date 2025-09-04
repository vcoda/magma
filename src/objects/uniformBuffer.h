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
#include "../helpers/uniformArray.h"
#include "../exceptions/exception.h"

namespace magma
{
    /* Base template of uniform buffer. */

    template<class Type>
    class TBaseUniformBuffer : public BaseUniformBuffer
    {
        static constexpr uint32_t Alignment = 16;

    public:
        typedef Type UniformType;

        VkDeviceSize getAlignment() const noexcept override { return std::alignment_of<Type>(); }
        bool dynamic() const noexcept override { return false; }

        helpers::UniformArray<Type> mapToArray(uint32_t firstIndex = 0,
            uint32_t arraySize = std::numeric_limits<uint32_t>::max())
        {
            void *data = mapRange(firstIndex, arraySize);
            if (!data)
                MAGMA_ERROR("failed to map range of uniform buffer");
            return helpers::UniformArray<Type>(data, arraySize);
        }

    protected:
        TBaseUniformBuffer(std::shared_ptr<Device> device,
            uint32_t arraySize,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator,
            bool mappedPersistently):
            BaseUniformBuffer(std::move(device), arraySize * sizeof(Type), sizeof(Type), arraySize, usage, memoryFlags,
                optional, sharing, std::move(allocator), mappedPersistently)
            {
                static_assert(std::alignment_of<Type>() == Alignment,
                    "uniform type should have 16-byte alignment");
            }
    };

    /* An array of uniform values that are used in various
       shader stages. It is host visible so can be mapped
       by user to write uniform values. */

    template<class Type>
    class UniformBuffer : public TBaseUniformBuffer<Type>
    {
    public:
        explicit UniformBuffer(std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            UniformBuffer(std::move(device), false, false, 1, std::move(allocator), optional, sharing)
        {}

        explicit UniformBuffer(std::shared_ptr<Device> device,
            bool stagedPool,
            bool mappedPersistently,
            uint32_t arraySize,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            TBaseUniformBuffer<Type>(std::move(device), arraySize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // Make it compatible with vkCmdUpdateBuffer
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                    (stagedPool ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0),
                optional, sharing, std::move(allocator), mappedPersistently)
        {}
    };

    /* Host visible but non-coherent uniform buffer.
       Requires to flush mapped memory range after data upload.
       Mapping and unmapping uniform buffer have a CPU cost.
       Therefore, if updated frequently, user may define it
       as persistenly mapped. */

    template<class Type>
    class NonCoherentUniformBuffer : public TBaseUniformBuffer<Type>
    {
    public:
        explicit NonCoherentUniformBuffer(std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            NonCoherentUniformBuffer(std::move(device), false, 1, std::move(allocator), optional, sharing)
        {}

        explicit NonCoherentUniformBuffer(std::shared_ptr<Device> device,
            bool mappedPersistently,
            uint32_t arraySize,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Buffer::Initializer& optional = Buffer::Initializer(),
            const Sharing& sharing = Sharing()):
            TBaseUniformBuffer<Type>(std::move(device), arraySize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                optional, sharing, std::move(allocator), mappedPersistently)
        {}

        bool flushMappedRange(const helpers::UniformArray<Type>& array) noexcept
        {
            if (!Buffer::memory->mapped())
                return false;
            const VkDeviceSize alignment = BaseUniformBuffer::getAlignment();
            const VkDeviceSize nonCoherentAtomSize = BaseUniformBuffer::getNonCoherentAtomSize();
            VkDeviceSize offset = array.getFirstIndex() * alignment;
            if (offset % nonCoherentAtomSize)
                offset = offset / nonCoherentAtomSize * nonCoherentAtomSize;
            VkDeviceSize size = array.getUpdatedRange() * alignment;
            const VkDeviceSize minFlushSize = std::min(Buffer::memory->getSize(), nonCoherentAtomSize);
            size = std::max(size, minFlushSize);
            return Buffer::memory->flushMappedRange(offset + BaseUniformBuffer::getMapOffset(), size);
        }
    };

    template<class T> using UniformBufferUPtr = std::unique_ptr<UniformBuffer<T>>;
    template<class T> using UniformBufferSPtr = std::shared_ptr<UniformBuffer<T>>;
    template<class T> using NonCoherentUniformBufferUPtr = std::unique_ptr<NonCoherentUniformBuffer<T>>;
    template<class T> using NonCoherentUniformBufferSPtr = std::shared_ptr<NonCoherentUniformBuffer<T>>;
} // namespace magma
