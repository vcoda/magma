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
#include "../misc/resourceSharing.h"

namespace magma
{
    /* An array of vertex buffer attributes like position, normal, texture coordinates etc. */

    class VertexBuffer : public Buffer
    {
    public:
        explicit VertexBuffer(std::shared_ptr<Device> device,
            const void *data, VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            const ResourceSharing& sharing = ResourceSharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            VkBufferCreateFlags flags = 0,
            const ResourceSharing& sharing = ResourceSharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            std::shared_ptr<SrcTransferBuffer> srcBuffer,
            VkBufferCreateFlags flags = 0,
            const ResourceSharing& sharing = ResourceSharing(),
            std::shared_ptr<IAllocator> allocator = nullptr);
        template<typename VertexType>
        explicit VertexBuffer(std::shared_ptr<Device> device,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            const ResourceSharing& sharing = ResourceSharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        template<typename VertexType>
        explicit VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0,
            const ResourceSharing& sharing = ResourceSharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        void setVertexCount(uint32_t count) noexcept { vertexCount = count; }
        uint32_t getVertexCount() const noexcept;

    private:
        uint32_t vertexCount = 0;
    };
} // namespace magma

#include "vertexBuffer.inl"
