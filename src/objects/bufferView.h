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
#include "nondispatchable.h"

namespace magma
{
    class Buffer;

    /* A buffer view represents a contiguous range of a buffer
       and a specific format to be used to interpret the data.
       Buffer views are used to enable shaders to access buffer
       contents interpreted as formatted data. */

    class BufferView : public NonDispatchable<VkBufferView>
    {
    public:
        explicit BufferView(std::shared_ptr<Buffer> resource,
            VkFormat format,
            VkDeviceSize offset = 0,
            VkDeviceSize range = VK_WHOLE_SIZE,
            const StructureChain& extendedInfo = StructureChain());
        ~BufferView();
        const std::shared_ptr<Buffer>& getBuffer() const noexcept { return buffer; }
        VkFormat getFormat() const noexcept { return format; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        VkDeviceSize getRange() const noexcept { return range; }

    private:
        std::shared_ptr<Buffer> buffer;
        const VkFormat format;
        const VkDeviceSize offset;
        const VkDeviceSize range;
    };
} // namespace magma
