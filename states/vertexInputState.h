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

namespace magma
{
    /* Structure specifying vertex input binding description. */

    struct VertexInputBinding : VkVertexInputBindingDescription
    {
        constexpr VertexInputBinding(uint32_t binding,
            uint32_t stride,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept;
        constexpr hash_t hash() const noexcept;
    };

    /* Allows to specify individual divisor value for binding.
       Divisor is the number of successive instances that will
       use the same value of the vertex attribute when instanced
       rendering is enabled. For example, if the divisor is N,
       the same vertex attribute will be applied to N successive
       instances before moving on to the next vertex attribute. */

    struct VertexInputBindingDivisor : VertexInputBinding
    {
        uint32_t divisor;
        constexpr VertexInputBindingDivisor(uint32_t binding,
            uint32_t stride,
            uint32_t divisor,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept;
        constexpr hash_t hash() const noexcept;
    };

    /* Structure specifying vertex input attribute description. */

    struct VertexInputAttribute : VkVertexInputAttributeDescription
    {
        VertexInputAttribute() = default;
        constexpr VertexInputAttribute(uint32_t location,
            uint32_t binding,
            VkFormat format,
            uint32_t offset) noexcept;
        template<class Vertex, class Type>
        constexpr VertexInputAttribute(uint32_t location,
            Type Vertex::*attrib) noexcept;
        constexpr hash_t hash() const noexcept;
    };

    /* Applications specify vertex input attribute and vertex
       input binding descriptions as part of graphics pipeline. */

    class VertexInputState : public VkPipelineVertexInputStateCreateInfo
    {
    public:
        VertexInputState() noexcept;
        explicit VertexInputState(const VertexInputBinding& binding,
            const std::initializer_list<VertexInputAttribute>& attributes) noexcept;
        explicit VertexInputState(const std::initializer_list<VertexInputBinding>& bindings,
            const std::initializer_list<VertexInputAttribute>& attributes) noexcept;
    #ifdef VK_EXT_vertex_attribute_divisor
        explicit VertexInputState(const VertexInputBindingDivisor& binding,
            const std::initializer_list<VertexInputAttribute>& attributes) noexcept;
        explicit VertexInputState(const std::initializer_list<VertexInputBindingDivisor>& bindings,
            const std::initializer_list<VertexInputAttribute>& attributes) noexcept;
    #endif // VK_EXT_vertex_attribute_divisor
        VertexInputState(const VertexInputState&) noexcept;
        VertexInputState& operator=(const VertexInputState&) noexcept;
        virtual ~VertexInputState();
        virtual uint32_t stride(uint32_t binding) const noexcept;
        hash_t hash() const noexcept;
        bool operator==(const VertexInputState&) const noexcept;

    #ifdef VK_EXT_vertex_attribute_divisor
        VkPipelineVertexInputDivisorStateCreateInfoEXT vertexInputDivisorInfo;
    #endif
    };
} // namespace magma

#include "vertexInputState.inl"
