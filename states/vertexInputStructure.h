/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "vertexInputState.h"
#include "../internal/copy.h"

namespace magma
{
    /* Template vertex input structure. */

    template<typename Vertex>
    class VertexInputStructure : public VertexInputState
    {
    public:
        VertexInputStructure() = default;
        explicit VertexInputStructure(uint32_t binding,
            const std::initializer_list<VertexInputAttribute> attributes,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
        template<uint32_t vertexAttributeCount>
        explicit VertexInputStructure(uint32_t binding,
            const VertexInputAttribute(&attributes)[vertexAttributeCount],
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
        VertexInputStructure(const VertexInputStructure& other);
        VertexInputStructure& VertexInputStructure::operator=(const VertexInputStructure& other);
        virtual uint32_t stride(uint32_t binding) const override;
    };

    namespace specialization
    {
        /* This structure is used for attribute format deduction. */

        template<VkFormat format = VK_FORMAT_UNDEFINED>
        struct AttributeFormat
        {
            constexpr operator VkFormat() const { return format; }
        };

        /* User have to specialize this template for concrete type.
           Optional normalization flag is used to distinct normalized integer formats
           (that come into the shader in the range [0,1]) from plain integers. */

        template<typename Type, bool normalized = false>
        struct VertexAttribute : AttributeFormat<VK_FORMAT_UNDEFINED>
        {
            VertexAttribute() noexcept
            {
                static_assert(false, "vertex attribute type not specialized");
            }
            constexpr std::size_t size() const { return sizeof(Type); }
        };

        /* Built-in specializations for basic machine types. */

        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint8_t, true, VK_FORMAT_R8_UNORM)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int8_t, true, VK_FORMAT_R8_SNORM)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint8_t, false, VK_FORMAT_R8_UINT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int8_t, false, VK_FORMAT_R8_SINT)

        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint16_t, true, VK_FORMAT_R16_UNORM)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int16_t, true, VK_FORMAT_R16_SNORM)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint16_t, false, VK_FORMAT_R16_UINT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int16_t, false, VK_FORMAT_R16_SINT)

        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint32_t, false, VK_FORMAT_R32_UINT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int32_t, false, VK_FORMAT_R32_SINT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(float, false, VK_FORMAT_R32_SFLOAT)

        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint64_t, false, VK_FORMAT_R64_UINT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int64_t, false, VK_FORMAT_R64_SINT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(double, false, VK_FORMAT_R64_SFLOAT)
    } // namespace specialization
} // namespace magma

#include "vertexDecl.h"

namespace magma
{
    namespace renderstates
    {
        extern const VertexInputStructure<int> nullVertexInput;

        extern const VertexInputStructure<vertexdecl::Pos2fTex2f> pos2fTex2f;
        extern const VertexInputStructure<vertexdecl::Pos2fColor3f> pos2fColor3f;
        extern const VertexInputStructure<vertexdecl::Pos2fColor4f> pos2fColor4f;
        extern const VertexInputStructure<vertexdecl::Pos2fColor4b> pos2fColor4b;

        extern const VertexInputStructure<vertexdecl::Pos3fTex2f> pos3fTex2f;
        extern const VertexInputStructure<vertexdecl::Pos3fColor3f> pos3fColor3f;
        extern const VertexInputStructure<vertexdecl::Pos3fColor4f> pos3fColor4f;
        extern const VertexInputStructure<vertexdecl::Pos3fColor4b> pos3fColor4b;
        extern const VertexInputStructure<vertexdecl::Pos3fNormal3f> pos3fNormal3f;

        extern const VertexInputStructure<vertexdecl::Pos3fNormal3fTex2f> pos3fNormal3fTex2f;
        extern const VertexInputStructure<vertexdecl::Pos3fNormal3fColor3f> pos3fNormal3fColor3f;
        extern const VertexInputStructure<vertexdecl::Pos3fNormal3fColor4f> pos3fNormal3fColor4f;
        extern const VertexInputStructure<vertexdecl::Pos3fNormal3fColor4b> pos3fNormal3fColor4b;
    } // namespace renderstates
} // namespace magma

#include "vertexInputStructure.inl"
