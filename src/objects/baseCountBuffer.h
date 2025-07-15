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
    /* Base class of count buffer. Allows to read back
       counter value(s) from device to host for debugging. */

    class BaseCountBuffer : public Buffer
    {
    public:
        VkPipelineStageFlags getStageMask() const noexcept { return stageMask; }
        void readback(lent_ptr<CommandBuffer> cmdBuffer) const;

    protected:
        BaseCountBuffer(std::shared_ptr<Device> device,
            uint32_t elementCount,
            VkPipelineStageFlags stageMask,
            std::shared_ptr<Allocator> allocator,
            const Sharing& sharing);

        const VkPipelineStageFlags stageMask;
        std::unique_ptr<Buffer> stagingBuffer;
    };
} // namespace magma
