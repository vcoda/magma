/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
    class SrcTransferBuffer;

    /* Base vertex buffer class. Holds vertex count as helper for draw calls. */

    class BaseVertexBuffer : public Buffer
    {
    public:
        void setVertexCount(uint32_t count) noexcept;
        uint32_t getVertexCount() const noexcept;

    protected:
        explicit BaseVertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memFlags,
            VkBufferCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<IAllocator> allocator);

        uint32_t vertexCount;
    };

    /* An array of vertex buffer attributes like position, normal, texture coordinates etc. */

    class VertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
        // Templates
        template<typename VertexType, uint32_t vertexArraySize>
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            const VertexType (&vertices)[vertexArraySize],
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        template<typename VertexType>
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
    };

    /* Dynamic vertex buffer that can be mapped for host access. */

    class DynamicVertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit DynamicVertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            const void *initial = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
    };

#ifdef VK_NV_ray_tracing
    class AccelerationStructureVertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit AccelerationStructureVertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit AccelerationStructureVertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
#endif // VK_NV_ray_tracing
} // namespace magma

#include "vertexBuffer.inl"
