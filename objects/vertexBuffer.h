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
        void setVertexCount(uint32_t count) noexcept { vertexCount = count; }
        uint32_t getVertexCount() const noexcept 
        { 
            MAGMA_ASSERT(vertexCount);
            return vertexCount;
        }

    protected:
        explicit BaseVertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            VkBufferCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);

        uint32_t vertexCount;
    };

    /* An array of vertex buffer attributes like position, normal, texture coordinates etc. */

    class VertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing());
    };

    /* Dynamic vertex buffer for fast data transfer from host to device 
       when using page-locked (or "pinned") memory. */

    class DynamicVertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit DynamicVertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            bool pciPinnedMemory,
            std::shared_ptr<Allocator> allocator = nullptr,
            const void *initial = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };

#ifdef VK_NV_ray_tracing
    class AccelerationStructureVertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit AccelerationStructureVertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit AccelerationStructureVertexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing());
    };
#endif // VK_NV_ray_tracing
} // namespace magma
