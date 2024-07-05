/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
    class DstTransferBuffer;

    /* 32-bit unsigned integer atomic counter. Allows to inspect
       counter value by host for debugging purposes. */

    class CountBuffer : public Buffer
    {
    public:
        explicit CountBuffer(std::shared_ptr<Device> device,
            VkPipelineStageFlags stageMask,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        void setValue(uint32_t value, std::shared_ptr<CommandBuffer> cmdBuffer) noexcept;
        void readback(std::shared_ptr<CommandBuffer> cmdBuffer) const;
        uint32_t getValue() const noexcept;

    private:
        const VkPipelineStageFlags stageMask;
        mutable std::shared_ptr<DstTransferBuffer> hostBuffer;
    };
} // namespace magma
