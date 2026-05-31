/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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

namespace magma
{
    /* Structure specifying vertex input attribute description.
       Usage example of non-constexpr attribute definition:

        {0, &Vertex::position},
        {1, &Vertex::normal},
        {2, &Vertex::texCoord} */

    struct VertexInputAttribute : VkVertexInputAttributeDescription
    {
        constexpr VertexInputAttribute(uint32_t location,
            uint32_t binding,
            VkFormat format,
            uint32_t offset) noexcept;
        template<class Vertex, class Type>
        VertexInputAttribute(uint32_t location,
            uint32_t binding,
            Type Vertex::*attribute) noexcept;
        constexpr uint32_t size() const noexcept;
        constexpr hash_t hash() const noexcept;
    };

    /* Applications specify vertex input attribute and vertex
       input binding descriptions as part of graphics pipeline. */

    struct VertexInputState : VkPipelineVertexInputStateCreateInfo
    {
        constexpr VertexInputState() noexcept;
        constexpr VertexInputState(uint32_t binding,
            uint32_t stride,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept;
    #ifdef VK_EXT_vertex_attribute_divisor
        constexpr VertexInputState(uint32_t binding,
            uint32_t stride,
            uint32_t divisor) noexcept;
    #endif // VK_EXT_vertex_attribute_divisor
        constexpr VertexInputState(const VertexInputState&) noexcept;
        constexpr hash_t hash() const noexcept;

        const VkVertexInputBindingDescription vertexBinding;
        // Allows to specify individual divisor value for binding.
        // Divisor is the number of successive instances that will
        // use the same value of the vertex attribute when instanced
        // rendering is enabled. For example, if the divisor is N,
        // the same vertex attribute will be applied to N successive
        // instances before moving on to the next vertex attribute.
    #ifdef VK_EXT_vertex_attribute_divisor
        const VkVertexInputBindingDivisorDescriptionEXT vertexBindingDivisor;
        const VkPipelineVertexInputDivisorStateCreateInfoEXT vertexInputDivisorInfo;
    #endif // VK_EXT_vertex_attribute_divisor
    };
} // namespace magma

#define MAGMA_VERTEX_ATTRIBUTE(Vertex, attribute, location)\
magma::VertexInputAttribute{location, 0,\
    magma::specialization::VertexAttribute<std::remove_cv_t<decltype(Vertex::attribute)>>::getFormat(),\
    offsetof(Vertex, attribute)\
}

#define MAGMA_VERTEX_ATTRIBUTE_BINDING(Vertex, attribute, location, binding)\
magma::VertexInputAttribute{location, binding,\
    magma::specialization::VertexAttribute<std::remove_cv_t<decltype(Vertex::attribute)>>::getFormat(),\
    0\
}

#include "vertexInputState.inl"
