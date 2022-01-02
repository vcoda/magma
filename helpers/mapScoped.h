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
#include "../core/zeroMemory.h"

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
            std::shared_ptr<DeviceMemory> memory(buffer->getMemory());
            if (memory)
            {
                void *const data = memory->map();
                if (data)
                {
                    callbackFn(static_cast<Type *>(data));
                    memory->unmap();
                }
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<UniformBuffer<Type>>& buffer,
            std::function<void(typename UniformBuffer<Type>::UniformType *data)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            Type *const data = buffer->map();
            if (data)
            {
                callbackFn(data);
                buffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScopedMemzero(const std::shared_ptr<UniformBuffer<Type>>& buffer,
            std::function<void(typename UniformBuffer<Type>::UniformType *data)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            Type *const data = buffer->map(core::zeroMemory);
            if (data)
            {
                callbackFn(data);
                buffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<UniformBuffer<Type>>& buffer,
            std::function<void(UniformArray<Type>& array)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            Type *const data = buffer->map();
            if (data)
            {
                UniformArray<Type> array(data, buffer->getArraySize());
                callbackFn(array);
                buffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScopedMemzero(const std::shared_ptr<UniformBuffer<Type>>& buffer,
            std::function<void(UniformArray<Type>& array)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            Type *const data = buffer->map(core::zeroMemory);
            if (data)
            {
                UniformArray<Type> array(data, buffer->getArraySize());
                callbackFn(array);
                buffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<DynamicUniformBuffer<Type>>& buffer,
            std::function<void(AlignedUniformArray<Type>& array)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            Type *const data = buffer->map();
            if (data)
            {
                AlignedUniformArray<Type> array(data,
                    buffer->getArraySize(),
                    buffer->getElementAlignment());
                callbackFn(array);
                buffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScopedMemzero(const std::shared_ptr<DynamicUniformBuffer<Type>>& buffer,
            std::function<void(AlignedUniformArray<Type>& array)> callbackFn)
        {
            MAGMA_ASSERT(buffer);
            MAGMA_ASSERT(callbackFn);
            Type *const data = buffer->map(core::zeroMemory);
            if (data)
            {
                AlignedUniformArray<Type> array(data,
                    buffer->getArraySize(),
                    buffer->getElementAlignment());
                callbackFn(array);
                buffer->unmap();
            }
        }

        template<typename Type>
        inline void mapScoped(const std::shared_ptr<Image>& image,
            VkDeviceSize offset,
            std::function<void(Type *data)> callbackFn)
        {
            MAGMA_ASSERT(image);
            MAGMA_ASSERT(callbackFn);
            std::shared_ptr<DeviceMemory> memory(image->getMemory());
            if (memory)
            {
                void *const data = memory->map(offset);
                if (data)
                {
                    callbackFn(static_cast<Type *>(data));
                    memory->unmap();
                }
            }
        }
    } // namespace helpers
} // namespace magma
