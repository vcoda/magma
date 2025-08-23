/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
    /* Base index buffer class. Holds index type and index count
       as a helper for indexed draw calls. */

    class BaseIndexBuffer : public Buffer
    {
    public:
        VkIndexType getIndexType() const noexcept { return indexType; }
        std::size_t getIndexSize() const noexcept;
        uint32_t getIndexCount() const noexcept;

    protected:
        BaseIndexBuffer(std::shared_ptr<Device> device,
            VkIndexType indexType,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            const Initializer& optional,
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
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit IndexBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            VkIndexType indexType,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
        explicit IndexBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            VkIndexType indexType,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };

    /* Template index buffer that uses an "array of known bound"
       parameter to determine VkIndexType and size of index array
       at compile-time to minimize count of constructor's arguments.

       Usage example:

       uint16_t indices[] = {0, 1, 2};
       auto indexBuffer = std::make_unique<magma::TIndexBuffer<uint16_t>>(
           cmdBuffer, indices); */

    template<class IndexType>
    class TIndexBuffer : public IndexBuffer
    {
        static_assert(
        #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
            std::is_same<IndexType, uint8_t>::value ||
        #endif
            std::is_same<IndexType, uint16_t>::value ||
            std::is_same<IndexType, uint32_t>::value,
        #if defined(VK_KHR_index_type_uint8) || defined(VK_EXT_index_type_uint8)
            "index buffer must contain indices of unsigned char, short or int type"
        #else
            "index buffer must contain indices of unsigned short or int type"
        #endif
            );
    public:
        template<std::size_t IndexCount>
        explicit TIndexBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            const IndexType (&indices)[IndexCount],
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
    };

    /* Major GPU vendors expose a 256MiB-ish staging buffer
       with the DEVICE_LOCAL | HOST_VISIBLE | HOST_COHERENT
       flags where the GPU and CPU can both write into shared
       memory visible to each other. This limit correlates with
       the 256MiB PCIE-specified BAR limit that defines the size
       of the VRAM window that the host can access. */

    class DynamicIndexBuffer : public BaseIndexBuffer
    {
    public:
        explicit DynamicIndexBuffer(std::shared_ptr<Device> device,
            VkIndexType indexType,
            VkDeviceSize size,
            bool stagedPool,
            std::shared_ptr<Allocator> allocator = nullptr,
            const void *initialData = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
    };

    template<class T>
    constexpr VkIndexType getIndexType() noexcept;
} // namespace magma

#include "indexBuffer.inl"
