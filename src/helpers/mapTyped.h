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
#include "../objects/deviceMemory.h"
#include "../objects/buffer.h"

namespace magma::helpers
{
template<class Type>
inline Type *map(lent_ptr<Buffer> buffer,
    VkDeviceSize offset = 0,
    VkDeviceSize size = VK_WHOLE_SIZE,
    VkMemoryMapFlags flags = 0) noexcept
{
    MAGMA_ASSERT(sizeof(Type) <= buffer->getSize());
    if (sizeof(Type) > buffer->getSize())
        return nullptr;
    if ((size != VK_WHOLE_SIZE) && (sizeof(Type) > size))
        return nullptr;
    void *ptr = buffer->getMemory()->map(offset, size, flags);
    MAGMA_ASSERT(ptr);
    return reinterpret_cast<Type *>(ptr);
}

inline bool unmap(lent_ptr<Buffer> buffer) noexcept
{
    auto& memory = buffer->getMemory();
    if (!memory->mapped() || memory->persistentlyMapped())
        return false;
    memory->unmap();
    return true;
}
} // namespace magma::helpers
