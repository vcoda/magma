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
#include "../objects/vertexBuffer.h"
#include "../objects/indexBuffer.h"
#include "../objects/accelerationStructureInputBuffer.h"

namespace magma
{
    namespace helpers
    {
        template<class Vertex, std::size_t Size>
        std::shared_ptr<VertexBuffer> makeVertexBuffer(const Vertex (&vertices)[Size],
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
        template<class Vertex>
        std::shared_ptr<VertexBuffer> makeVertexBuffer(const std::vector<Vertex>& vertices,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
        template<class Index, std::size_t Size>
        std::shared_ptr<IndexBuffer> makeIndexBuffer(const Index (&indices)[Size],
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
        template<class Index>
        std::shared_ptr<IndexBuffer> makeIndexBuffer(const std::vector<Index>& indices,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
    #ifdef VK_KHR_acceleration_structure
        template<class Type>
        std::unique_ptr<AccelerationStructureInputBuffer> makeInputBuffer(const Type& element,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
        template<class Type, std::size_t Size>
        std::unique_ptr<AccelerationStructureInputBuffer> makeInputBuffer(const Type (&elements)[Size],
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
        template<class Type>
        std::unique_ptr<AccelerationStructureInputBuffer> makeInputBuffer(const std::vector<Type>& elements,
            std::shared_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
    #endif // VK_KHR_acceleration_structure
    } // namespace helpers
} // namespace magma

#include "bufferInitializers.inl"
