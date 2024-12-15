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
#include "../objects/buffer.h"

namespace magma
{
template<class Type>
inline void mapRange(lent_ptr<Buffer> buffer, VkDeviceSize offset, VkDeviceSize size,
    std::function<void(Type *data)> mapFn)
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(offset + (VK_WHOLE_SIZE == size ? 0 : size) <= buffer->getSize());
    auto& bufferMemory = buffer->getMemory();
    if (bufferMemory)
    {
        void *data = bufferMemory->map(offset, size);
        if (data) try
        {
            MAGMA_ASSERT(mapFn);
            mapFn(reinterpret_cast<Type *>(data));
            bufferMemory->unmap();
        }
        catch (...)
        {
            bufferMemory->unmap();
            MAGMA_THROW;
        }
    }
}

template<class Type>
inline void map(lent_ptr<Buffer> buffer, std::function<void(Type *data)> mapFn)
{
    mapRange(std::move(buffer), 0, VK_WHOLE_SIZE, std::move(mapFn));
}
} // namespace magma
