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
#include "buffer.h"

namespace magma
{
    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(std::shared_ptr<const Device> device,
            const void *data, VkDeviceSize size,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0);

        template <typename IndexType, size_t ArraySize>
        IndexBuffer(std::shared_ptr<const Device> device,
            const IndexType(&indices)[ArraySize],
            VkBufferCreateFlags flags = 0):
            Buffer(device, sizeof(IndexType) * ArraySize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, flags,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
            indexType(sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32)
        {
            if (void *buffer = memory->map(0, size))
            {
                memcpy(buffer, indices, static_cast<size_t>(size));
                memory->unmap();
            }
        }

        template <typename IndexType>
        IndexBuffer(std::shared_ptr<const Device> device,
            const std::vector<IndexType>& indices,
            VkBufferCreateFlags flags = 0):
            Buffer(device, sizeof(IndexType) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, flags,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
            indexType(sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32)
        {
            if (void *buffer = memory->map(0, size))
            {
                memcpy(buffer, indices.data(), static_cast<size_t>(size));
                memory->unmap();
            }
        }

        VkIndexType getIndexType() const { return indexType; }
        uint32_t getIndexCount() const 
        { 
            if (VK_INDEX_TYPE_UINT16 == indexType)
                return static_cast<uint32_t>(size / sizeof(uint16_t));
            return static_cast<uint32_t>(size / sizeof(uint32_t));
        }

    protected:
        IndexBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags,
            VkIndexType indexType,
            VkMemoryPropertyFlags memoryFlags);

    private:
        VkIndexType indexType;
    };

    class LocalIndexBuffer : public IndexBuffer
    {
    public:
        LocalIndexBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0);
    };
} // namespace magma
