/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
    /* Base index buffer class. Holds index type and count as helper for indexed draw calls. */

    class BaseIndexBuffer : public Buffer
    {
    public:
        VkIndexType getIndexType() const noexcept;
        uint32_t getIndexCount() const noexcept;

    protected:
        explicit BaseIndexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkIndexType indexType,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memFlags,
            VkBufferCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);

        VkIndexType indexType;
    };

    /* An array of vertex indices. */

    class IndexBuffer : public BaseIndexBuffer
    {
    public:
        explicit IndexBuffer(std::shared_ptr<CommandBuffer> copyCmd,
            VkDeviceSize size,
            const void *data,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        template<typename IndexType>
        explicit IndexBuffer(std::shared_ptr<CommandBuffer> copyCmd,
            const std::vector<IndexType>& indices,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit IndexBuffer(std::shared_ptr<CommandBuffer> copyCmd,
            std::shared_ptr<SrcTransferBuffer> srcBuffer,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* Dynamic index buffer that can be mapped for host access. */

    class DynamicIndexBuffer : public BaseIndexBuffer
    {
    public:
        DynamicIndexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkIndexType indexType,
            const void *initial = nullptr,
            VkBufferCreateFlags flags = 0 ,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma

#include "indexBuffer.inl"
