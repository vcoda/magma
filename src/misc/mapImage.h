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
#include "../objects/image.h"

namespace magma
{
template<class Type>
inline void mapImageRange(lent_ptr<Image> image, VkDeviceSize offset, VkDeviceSize size,
    std::function<void(Type *data)> mapFn)
{
    MAGMA_ASSERT(image);
    MAGMA_ASSERT(image->getTiling() == VK_IMAGE_TILING_LINEAR);
    MAGMA_ASSERT(offset + (VK_WHOLE_SIZE == size ? 0 : size) <= image->getSize());
    auto& imageMemory = image->getMemory();
    if (imageMemory)
    {
        void *data = imageMemory->map(offset, size);
        MAGMA_ASSERT(data);
        if (data) try
        {
            MAGMA_ASSERT(mapFn);
            mapFn(static_cast<Type *>(data));
            imageMemory->unmap();
        }
        catch (...)
        {
            imageMemory->unmap();
            MAGMA_THROW;
        }
    }
}

template<class Type>
inline void mapImage(lent_ptr<Image> image, std::function<void(Type *data)> mapFn)
{
    mapImageRange(std::move(image), 0, VK_WHOLE_SIZE, std::move(mapFn));
}
} // namespace magma
