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
#include "model/nondispatchable.h"

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
        ~BufferView();
        virtual Buffer *getBuffer() const noexcept = 0;
        VkFormat getFormat() const noexcept { return format; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        VkDeviceSize getRange() const noexcept { return range; }

    protected:
        BufferView(const Buffer *buffer,
            VkFormat format,
            VkDeviceSize offset,
            VkDeviceSize range,
            const StructureChain& extendedInfo);

    private:
        const VkFormat format;
        const VkDeviceSize offset;
        const VkDeviceSize range;
    };

    /* Buffer view may have various resource ownership strategy.
       If buffer has a single interpretation, unique ownership is
       preferred. Sometimes a few different interpretations of
       the same buffer may be necessary, so a shared ownership of
       resource is used. */

    template<class Pointer>
    class TBufferView : public BufferView
    {
    public:
        explicit TBufferView(Pointer buffer,
            VkFormat format,
            VkDeviceSize offset = 0,
            VkDeviceSize range = VK_WHOLE_SIZE,
            const StructureChain& extendedInfo = StructureChain());
        Buffer *getBuffer() const noexcept override { return buffer.get(); }
        const Pointer& getBufferPointer() const noexcept { return buffer; }

    private:
        const Pointer buffer;
    };

    typedef TBufferView<std::unique_ptr<Buffer>> UniqueBufferView;
    typedef TBufferView<std::shared_ptr<Buffer>> SharedBufferView;
} // namespace magma

#include "bufferView.inl"
