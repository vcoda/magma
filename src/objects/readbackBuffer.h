/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#include "dstTransferBuffer.h"
#include "../barriers/bufferMemoryBarrier.h"

namespace magma
{
    /* A readback buffer provides device-local storage for typed
       data used by a graphics or compute pipeline, and allows
       its contents to be initialized and read back by the host. */

    template<class Type>
    class ReadbackBuffer : public Buffer
    {
    public:
        static_assert(sizeof(Type) % sizeof(uint32_t) == 0,
            "size of readback type must be a multiple of 4 bytes");
        static_assert(alignof(Type) >= alignof(uint32_t),
            "readback type should be at least 4-byte aligned");
        static_assert(std::is_trivially_copyable_v<Type>,
            "readback type should be trivially copyable");

        explicit ReadbackBuffer(std::shared_ptr<Device> device,
            VkPipelineStageFlags stageMask,
            std::shared_ptr<Allocator> allocator = nullptr,
            uint32_t count = 1,
            bool indirectUsage = false,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        VkPipelineStageFlags getStageMask() const noexcept { return stageMask; }
        uint32_t getCount() const noexcept { return count; }
        void readback(lent_ptr<CommandBuffer> cmdBuffer) const;
        void setValue(const Type& value,
            lent_ptr<CommandBuffer> cmdBuffer) noexcept;
        bool getValue(Type *dst) const noexcept;

    protected:
        const VkPipelineStageFlags stageMask;
        const uint32_t count;
        std::unique_ptr<DstTransferBuffer> staging;
    };

    template<class T> using ReadbackBufferUPtr = std::unique_ptr<ReadbackBuffer<T>>;
    template<class T> using ReadbackBufferSPtr = std::shared_ptr<ReadbackBuffer<T>>;
} // namespace magma

#include "readbackBuffer.inl"
