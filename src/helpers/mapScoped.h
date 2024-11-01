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
#include "../objects/uniformBuffer.h"
#include "../objects/dynamicUniformBuffer.h"
#include "../objects/image.h"

namespace magma
{
    namespace helpers
    {
        template<class Type>
        void mapScopedRange(const Buffer *buffer, VkDeviceSize offset, VkDeviceSize size,
            std::function<void(Type *data)> mapFn);
        template<class Type>
        void mapScoped(const std::unique_ptr<Buffer>& buffer,
            std::function<void(Type *data)> mapFn);
        template<class Type>
        void mapScoped(const std::shared_ptr<Buffer>& buffer,
            std::function<void(Type *data)> mapFn);
        template<class Type>
        void mapScoped(std::shared_ptr<UniformBuffer<Type>> uniformBuffer,
            std::function<void(typename UniformBuffer<Type>::UniformType *data)> mapFn);
        template<class Type>
        void mapScoped(std::shared_ptr<UniformBuffer<Type>> uniformBuffer,
            std::function<void(UniformArray<Type>& array)> mapFn);
        template<class Type>
        void mapScoped(std::shared_ptr<DynamicUniformBuffer<Type>> uniformBuffer,
            std::function<void(AlignedUniformArray<Type>& array)> mapFn);
        template<class Type>
        void mapScoped(std::shared_ptr<NonCoherentUniformBuffer<Type>> uniformBuffer,
            std::function<void(UniformArray<Type>& array)> mapFn);
        template<class Type>
        void mapScoped(std::shared_ptr<NonCoherentDynamicUniformBuffer<Type>> uniformBuffer,
            std::function<void(AlignedUniformArray<Type>& array)> mapFn);
        template<class Type>
        void mapScopedRange(std::shared_ptr<Image> image, VkDeviceSize offset, VkDeviceSize size,
            std::function<void(Type *data)> mapFn);
        template<class Type>
        void mapScoped(std::shared_ptr<Image> image,
            std::function<void(Type *data)> mapFn);
    } // namespace helpers
} // namespace magma

#include "mapScoped.inl"
