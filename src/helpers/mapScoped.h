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
#include "uniformArray.h"
#include "alignedUniformArray.h"
#include "../objects/dynamicUniformBuffer.h"
#include "../objects/image.h"

namespace magma
{
namespace helpers
{
template<class Type>
inline void mapRangeScoped(std::shared_ptr<Buffer> buffer,
    VkDeviceSize offset,
    VkDeviceSize size,
    std::function<void(Type *data)> mapFn)
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(mapFn);
    MAGMA_ASSERT(offset + (VK_WHOLE_SIZE == size ? 0 : size) <= buffer->getSize());
    IDeviceMemory *bufferMemory = buffer->getMemory().get();
    if (bufferMemory)
    {
        if (void *const data = bufferMemory->map(offset, size))
        {
            try
            {
                mapFn(static_cast<Type *>(data));
            }
            catch (...)
            {
                bufferMemory->unmap();
                throw;
            }
            bufferMemory->unmap();
        }
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<Buffer> buffer,
    std::function<void(Type *data)> mapFn)
{
    mapRangeScoped(std::move(buffer), 0, VK_WHOLE_SIZE, std::move(mapFn));
}

template<class Type>
inline void mapScoped(std::shared_ptr<UniformBuffer<Type>> uniformBuffer,
    std::function<void(typename UniformBuffer<Type>::UniformType *data)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    if (Type *const data = uniformBuffer->map())
    {
        try
        {
            mapFn(data);
        }
        catch (...)
        {
            uniformBuffer->unmap();
            throw;
        }
        uniformBuffer->unmap();
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<UniformBuffer<Type>> uniformBuffer,
    std::function<void(UniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    if (Type *const data = uniformBuffer->map())
    {
        try
        {
            UniformArray<Type> array(data, uniformBuffer->getArraySize());
            mapFn(array);
        }
        catch (...)
        {
            uniformBuffer->unmap();
            throw;
        }
        uniformBuffer->unmap();
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<DynamicUniformBuffer<Type>> uniformBuffer,
    std::function<void(AlignedUniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    if (Type *const data = uniformBuffer->map())
    {
        try
        {
            AlignedUniformArray<Type> array(data,
                uniformBuffer->getArraySize(),
                uniformBuffer->getAlignment());
            mapFn(array);
        }
        catch (...)
        {
            uniformBuffer->unmap();
            throw;
        }
        uniformBuffer->unmap();
    }
}

template<class Type>
inline void mapRangeScoped(std::shared_ptr<Image> image,
    VkDeviceSize offset,
    VkDeviceSize size,
    std::function<void(Type *data)> mapFn)
{
    MAGMA_ASSERT(image);
    MAGMA_ASSERT(mapFn);
    MAGMA_ASSERT(offset + (VK_WHOLE_SIZE == size ? 0 : size) <= image->getSize());
    IDeviceMemory *imageMemory = image->getMemory().get();
    if (imageMemory)
    {
        if (void *const data = imageMemory->map(offset, size))
        {
            try
            {
                mapFn(static_cast<Type *>(data));
            }
            catch (...)
            {
                imageMemory->unmap();
                throw;
            }
            imageMemory->unmap();
        }
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<Image> image,
    std::function<void(Type *data)> mapFn)
{
    mapRangeScoped(std::move(image), 0, VK_WHOLE_SIZE, std::move(mapFn));
}
} // namespace helpers
} // namespace magma
