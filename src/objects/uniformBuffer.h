/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "buffer.h"
#include "device.h"
#include "physicalDevice.h"

namespace magma
{
    /* An array of uniform values that are used in various
       shader stages. It is host visible so can be mapped
       by user to write uniform values. */

    template<class Type>
    class UniformBuffer : public Buffer
    {
    public:
        typedef Type UniformType;

        explicit UniformBuffer(std::shared_ptr<Device> device,
            bool barStagedMemory = false,
            std::shared_ptr<Allocator> allocator = nullptr,
            uint32_t arraySize = 1,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing()):
            Buffer(std::move(device), sizeof(Type) * arraySize,
                0, // flags
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // Make it compatible with vkCmdUpdateBuffer
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

    /* An array of uniform values that are used in various
       shader stages. It is host visible so can be mapped
       by user to write uniform values. */

    template<class Type>
    class NonCoherentUniformBuffer : public Buffer
    {
    public:
        typedef Type UniformType;
        explicit NonCoherentUniformBuffer(std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator = nullptr,
            bool persistentlyMapped = false,
            uint32_t arraySize = 1,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing()):
            Buffer(std::move(device), sizeof(Type) * arraySize,
                0, // flags
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                optional, sharing, std::move(allocator)),
            persistent(persistentlyMapped),
            arraySize(arraySize)
        {
            static_assert(std::alignment_of<Type>() == 16, "uniform type should have 16-byte alignment");
            if (permanentlyMapped)
                memory->map();
        }

        ~NonCoherentUniformBuffer()
        {
            if (persistent)
                memory->unmap();
        }

        bool persistentlyMapped() const noexcept
        {
            return persistent;
        }

        void updateRange(VkDeviceSize offset, VkDeviceSize size, const void *srcData) noexcept
        {
            MAGMA_ASSERT(size % sizeof(Type) == 0);
            MAGMA_ASSERT(offset + size <= memory->getSize());
            if (persistent)
                memcpy((uint8_t *)memory->getMapPointer() + offset, srcData, size);
            else if (void *const mapData = memory->map(offset, size))
                memcpy(mapData, srcData, size);
            memory->flushMappedRange(offset, size);
            if (!persistent)
                memory->unmap();
        }

        void updateRange(const Type& element) noexcept
        {
            updateRange(0, sizeof(Type), &element);
        }

        virtual uint32_t getArraySize() const noexcept
        {
            return arraySize;
        }

    protected:
        const bool persistent;
        const uint32_t arraySize;
    };

    template<class Type> using UniformBufferPtr = std::shared_ptr<UniformBuffer<Type>>;
    template<class Type> using NonCoherentUniformBufferPtr = std::shared_ptr<NonCoherentUniformBuffer<Type>>;
} // namespace magma
