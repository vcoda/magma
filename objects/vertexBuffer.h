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
    class CommandBuffer;

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(std::shared_ptr<const Device> device,
            const void *data, VkDeviceSize size,
            uint32_t vertexCount,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            uint32_t vertexCount,  
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        template<typename VertexType>
        VertexBuffer(std::shared_ptr<const Device> device,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        template<typename VertexType>
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getVertexCount() const { return vertexCount; }

    protected:
        VertexBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags,
            std::shared_ptr<IAllocator> allocator,
            VkMemoryPropertyFlags memoryFlags);

    private:
        uint32_t vertexCount;
    };

    template<typename VertexType>
    inline VertexBuffer::VertexBuffer(std::shared_ptr<const Device> device,
        const std::vector<VertexType>& vertices,
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */):
        VertexBuffer(device, vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
            static_cast<uint32_t>(vertices.size()), flags, allocator)
    {}

    template<typename VertexType>
    VertexBuffer::VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
        const std::vector<VertexType>& vertices,
        VkBufferCreateFlags flags /* 0 */,
        std::shared_ptr<IAllocator> allocator /* nullptr */):
        VertexBuffer(copyCmdBuffer, vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()),
            static_cast<uint32_t>(vertices.size()), flags, allocator)
    {}
} // namespace magma
