/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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

    /* Base index buffer class. Holds index type and count as helper for indexed draw calls. */

    class BaseIndexBuffer : public Buffer
    {
    public:
        VkIndexType getIndexType() const noexcept { return indexType; }
        std::size_t getIndexSize() const noexcept;
        uint32_t getIndexCount() const noexcept;

    protected:
        explicit BaseIndexBuffer(std::shared_ptr<Device> device,
            VkIndexType indexType,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            const Descriptor& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);

        const VkIndexType indexType;
    };

    /* An array of vertex indices. */

    class IndexBuffer : public BaseIndexBuffer
    {
    public:
        explicit IndexBuffer(std::shared_ptr<Device> device,
            VkIndexType indexType,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
        explicit IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkIndexType indexType,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit IndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkIndexType indexType,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
    };

    /* Dynamic index buffer for fast data transfer from host to device
       when using page-locked (or "pinned") memory. */

    class DynamicIndexBuffer : public BaseIndexBuffer
    {
    public:
        explicit DynamicIndexBuffer(std::shared_ptr<Device> device,
            VkIndexType indexType,
            VkDeviceSize size,
            bool pinnedMemory,
            std::shared_ptr<Allocator> allocator = nullptr,
            const void *initialData = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };

#ifdef VK_NV_ray_tracing
    class AccelerationStructureIndexBuffer : public BaseIndexBuffer
    {
    public:
        explicit AccelerationStructureIndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkIndexType indexType,
            VkDeviceSize size,
            const void *data = nullptr,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit AccelerationStructureIndexBuffer(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkIndexType indexType,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_NV_ray_tracing
} // namespace magma
