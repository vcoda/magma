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

namespace magma
{
    class SrcTransferBuffer;
    class CommandBuffer;

    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkIndexType indexType,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        IndexBuffer(std::shared_ptr<const Device> device,
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
        IndexBuffer(std::shared_ptr<const Device> device,
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

    template<typename IndexType>
    inline IndexBuffer::IndexBuffer(std::shared_ptr<const Device> device,
        const std::vector<IndexType>& indices,
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */,
        CopyMemoryFunction copyFn /* nullptr */):
        IndexBuffer(device, indices.data(), static_cast<VkDeviceSize>(sizeof(IndexType) * indices.size()),
            sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32,
            flags, std::move(allocator), std::move(copyFn))
    {}

    template<typename IndexType>
    inline IndexBuffer::IndexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
        const std::vector<IndexType>& indices,
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */,
        CopyMemoryFunction copyFn /* nullptr */):
        IndexBuffer(copyCmdBuffer, indices.data(), static_cast<VkDeviceSize>(sizeof(IndexType) * indices.size()),
            sizeof(IndexType) == sizeof(uint16_t) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32,
            flags, std::move(allocator), std::move(copyFn))
    {}

    inline uint32_t IndexBuffer::getIndexCount() const noexcept
    {
        if (VK_INDEX_TYPE_UINT16 == indexType)
            return static_cast<uint32_t>(size / sizeof(uint16_t));
        return static_cast<uint32_t>(size / sizeof(uint32_t));
    }
} // namespace magma
