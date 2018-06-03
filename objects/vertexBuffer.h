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
#include "device.h"

namespace magma
{
    class SrcTransferBuffer;
    class CommandBuffer;

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        VertexBuffer(std::shared_ptr<const Device> device,
            const void *data, VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            std::shared_ptr<SrcTransferBuffer> srcBuffer,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        template<typename VertexType>
        VertexBuffer(std::shared_ptr<const Device> device,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        template<typename VertexType>
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        uint32_t getVertexCount() const noexcept;

    private:
        uint32_t vertexCount = 0;
    };

    template<typename VertexType>
    inline VertexBuffer::VertexBuffer(std::shared_ptr<const Device> device,
        const std::vector<VertexType>& vertices,
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */,
        CopyMemoryFunction copyFn /* nullptr */):
        VertexBuffer(std::move(device), vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
            flags, std::move(allocator), std::move(copyFn))
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
    }

    template<typename VertexType>
    inline VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
        const std::vector<VertexType>& vertices,
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */,
        CopyMemoryFunction copyFn /* nullptr */):
        VertexBuffer(copyCmdBuffer, vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
            flags, std::move(allocator), std::move(copyFn))
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
    }
} // namespace magma
