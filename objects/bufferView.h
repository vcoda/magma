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
#include "handle.h"

namespace magma
{
    class Device;
    class Buffer;

    class BufferView : public NonDispatchable<VkBufferView>
    {
    public:
        BufferView(std::shared_ptr<const Buffer> buffer,
            VkFormat format,
            VkDeviceSize offset = 0,
            VkDeviceSize range = VK_WHOLE_SIZE);
        ~BufferView();
        std::shared_ptr<const Buffer> getBuffer() const { return buffer; }

    private:
        std::shared_ptr<const Buffer> buffer;
    };
} // namespace magma
