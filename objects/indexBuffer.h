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
#include <vector>
#include "buffer.h"
#include "../shared.h"

namespace magma
{
    class SrcTransferBuffer;
    class CommandBuffer;

    /* An array of vertex indices. */

    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        IndexBuffer(std::shared_ptr<Device> device,
            const void *data, VkDeviceSize size,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            std::shared_ptr<SrcTransferBuffer> srcBuffer,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        template<typename IndexType>
        IndexBuffer(std::shared_ptr<Device> device,
            const std::vector<IndexType>& indices,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        template<typename IndexType>
        IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const std::vector<IndexType>& indices,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        VkIndexType getIndexType() const { return indexType; }
        uint32_t getIndexCount() const noexcept;

    private:
        VkIndexType indexType;
    };
} // namespace magma

#include "indexBuffer.inl"

