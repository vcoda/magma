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

    /* Base class of count buffer. Allows to read back
       counter value from device to host for debugging. */

    class BaseCountBuffer : public Buffer
    {
    public:
        VkPipelineStageFlags getStageMask() const noexcept { return stageMask; }
        void readback(std::shared_ptr<CommandBuffer> cmdBuffer) const;

    protected:
        BaseCountBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkPipelineStageFlags stageMask,
            std::shared_ptr<Allocator> allocator,
            const Sharing& sharing);
        const VkPipelineStageFlags stageMask;
        mutable std::shared_ptr<DstTransferBuffer> hostBuffer;
    };

    /* 32-bit unsigned integer atomic counter. */

    class CountBuffer : public BaseCountBuffer
    {
    public:
        explicit CountBuffer(std::shared_ptr<Device> device,
            VkPipelineStageFlags stageMask,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        void setValue(uint32_t value,
            std::shared_ptr<CommandBuffer> cmdBuffer) noexcept;
        uint32_t getValue() const noexcept;
    };

    /* Three 32-bit unsigned integer counters that can be used
       with vkCmdDispatchIndirect(X, Y, Z) call. */

    class DispatchCountBuffer : public BaseCountBuffer
    {
    public:
        explicit DispatchCountBuffer(std::shared_ptr<Device> device,
            VkPipelineStageFlags stageMask,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        void setValues(uint32_t x, uint32_t y, uint32_t z,
            std::shared_ptr<CommandBuffer> cmdBuffer) noexcept;
        std::array<uint32_t, 3> getValues() const noexcept;
    };
} // namespace magma
