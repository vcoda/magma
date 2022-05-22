/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../objects/deviceMemory.h"
#include "../objects/buffer.h"
#include "../objects/uniformBuffer.h"
#include "../objects/image.h"

namespace magma
{
    namespace helpers
    {
        template<typename Type>
        inline void mapScoped(const std::shared_ptr<Buffer>& buffer,
            std::function<void(Type *data)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            std::shared_ptr<DeviceMemory> bufferMemory(buffer->getMemory());
            if (bufferMemory)
            {
                if (void *const data = bufferMemory->map())
                {
                    try
                    {
                        callbackFn(static_cast<Type *>(data));
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

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<UniformBuffer<Type>>& uniformBuffer,
            std::function<void(typename UniformBuffer<Type>::UniformType *data)> callbackFn)
        {
            MAGMA_ASSERT(uniformBuffer);
            MAGMA_ASSERT(callbackFn);
            if (Type *const data = uniformBuffer->map())
            {
                try
                {
                    callbackFn(data);
                }
                catch (...)
                {
                    uniformBuffer->unmap();
                    throw;
                }
                uniformBuffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<UniformBuffer<Type>>& uniformBuffer,
            std::function<void(UniformArray<Type>& array)> callbackFn)
        {
            MAGMA_ASSERT(uniformBuffer);
            MAGMA_ASSERT(callbackFn);
            if (Type *const data = uniformBuffer->map())
            {
                try
                {
                    UniformArray<Type> array(data, uniformBuffer->getArraySize());
                    callbackFn(array);
                }
                catch (...)
                {
                    uniformBuffer->unmap();
                    throw;
                }
                uniformBuffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<DynamicUniformBuffer<Type>>& uniformBuffer,
            std::function<void(AlignedUniformArray<Type>& array)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            if (Type *const data = uniformBuffer->map())
            {
                try
                {
                    AlignedUniformArray<Type> array(data,
                        uniformBuffer->getArraySize(),
                        uniformBuffer->getElementAlignment());
                    callbackFn(array);
                }
                catch (...)
                {
                    uniformBuffer->unmap();
                    throw;
                }
                uniformBuffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<Image>& image, VkDeviceSize offset,
            std::function<void(Type *data)> callbackFn)
        {
            MAGMA_ASSERT(image);
            MAGMA_ASSERT(callbackFn);
            std::shared_ptr<DeviceMemory> imageMemory(image->getMemory());
            if (imageMemory)
            {
                if (void *const data = imageMemory->map(offset))
                {
                    try
                    {
                        callbackFn(static_cast<Type *>(data));
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
    } // namespace helpers
} // namespace magma
