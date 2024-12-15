/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    /* Base vertex buffer class. Holds vertex count
       as a helper for non-indexed draw calls. */

    class BaseVertexBuffer : public Buffer
    {
    public:
        void setVertexCount(uint32_t count) noexcept { vertexCount = count; }
        uint32_t getVertexCount() const noexcept { return vertexCount; }

    protected:
        BaseVertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);

        uint32_t vertexCount;
    };

    /* An array of vertex buffer attributes like vertices, normals,
       colors, texture coordinates etc. */

    class VertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit VertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit VertexBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit VertexBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };

    /* Major GPU vendors expose a 256MiB-ish staging buffer
       with the DEVICE_LOCAL | HOST_VISIBLE | HOST_COHERENT
       flags where the GPU and CPU can both write into shared
       memory visible to each other. This 256MiB limit
       correlates with the 256MiB PCIE-specified BAR-size limit
       that defines the size of the 256MiB aperture/window of
       VRAM that the host can access. */

    class DynamicVertexBuffer : public BaseVertexBuffer
    {
    public:
        explicit DynamicVertexBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            bool stagedPool,
            std::shared_ptr<Allocator> allocator = nullptr,
            const void *initialData = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma
