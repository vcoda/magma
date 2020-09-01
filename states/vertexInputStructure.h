/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "vertexLayouts.h"
#include "../core/copy.h"

namespace magma
{
    /* Template vertex input structure. */

    template<typename Vertex>
    class VertexInputStructure : public VertexInputState
    {
    public:
        VertexInputStructure() = default;
        explicit VertexInputStructure(uint32_t binding,
            const VertexInputAttribute& attribute,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
        explicit VertexInputStructure(uint32_t binding,
            const std::initializer_list<VertexInputAttribute>& attributes,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
        template<uint32_t vertexAttributeCount>
        explicit VertexInputStructure(uint32_t binding,
            const VertexInputAttribute(&attributes)[vertexAttributeCount],
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);
        VertexInputStructure(const VertexInputStructure& other) noexcept;
        VertexInputStructure& operator=(const VertexInputStructure& other) noexcept;
        virtual uint32_t stride(uint32_t binding) const noexcept override;
    };

    namespace specialization
    {
        /* This structure is used for attribute format deduction. */

        template<VkFormat format>
        struct AttributeFormat
        {
            constexpr VkFormat getFormat() const { return format; }
        };

        /* User have to specialize this template for concrete type.
           Optional normalization flag is used to distinct normalized integer formats
           (that come into the shader in the range [0,1]) from plain integers. */

        template<typename Type, bool normalized = false>
        struct VertexAttribute : AttributeFormat<VK_FORMAT_UNDEFINED>
        {
            constexpr VertexAttribute();
            constexpr std::size_t getSize() const { return sizeof(Type); }
            constexpr bool hasNormalization() const { return normalized; }
        };

        /* Built-in specializations for basic machine types. */

        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint8_t, true, VK_FORMAT_R8_UNORM);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int8_t, true, VK_FORMAT_R8_SNORM);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint8_t, false, VK_FORMAT_R8_UINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int8_t, false, VK_FORMAT_R8_SINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint16_t, true, VK_FORMAT_R16_UNORM);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int16_t, true, VK_FORMAT_R16_SNORM);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint16_t, false, VK_FORMAT_R16_UINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int16_t, false, VK_FORMAT_R16_SINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(half_t, false, VK_FORMAT_R16_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint32_t, false, VK_FORMAT_R32_UINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int32_t, false, VK_FORMAT_R32_SINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(float, false, VK_FORMAT_R32_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(uint64_t, false, VK_FORMAT_R64_UINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(int64_t, false, VK_FORMAT_R64_SINT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(double, false, VK_FORMAT_R64_SFLOAT);

        /* Built-in specializations for our vector types. */

        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::ubyte4, true, VK_FORMAT_R8G8B8A8_UNORM);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::half2, false, VK_FORMAT_R16G16_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::half3, false, VK_FORMAT_R16G16B16_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::half4, false, VK_FORMAT_R16G16B16A16_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::float2, false, VK_FORMAT_R32G32_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::float3, false, VK_FORMAT_R32G32B32_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::float4, false, VK_FORMAT_R32G32B32A32_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::double2, false, VK_FORMAT_R64G64_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::double3, false, VK_FORMAT_R64G64B64_SFLOAT);
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(core::double4, false, VK_FORMAT_R64G64B64A64_SFLOAT);
    } // namespace specialization
} // namespace magma

namespace magma
{
    namespace renderstates
    {
        extern const VertexInputStructure<int> nullVertexInput;

        /* Vertex input states with half float type. */

        extern const VertexInputStructure<core::half2> pos2h;
        extern const VertexInputStructure<core::half3> pos3h;
        extern const VertexInputStructure<core::half4> pos4h;

        extern const VertexInputStructure<vertexlayout::Pos2hTex2h> pos2hTex2h;
        extern const VertexInputStructure<vertexlayout::Pos2hColor3h> pos2hColor3h;
        extern const VertexInputStructure<vertexlayout::Pos2hColor4h> pos2hColor4h;
        extern const VertexInputStructure<vertexlayout::Pos2hColor4b> pos2hColor4b;

        extern const VertexInputStructure<vertexlayout::Pos3hTex2h> pos3hTex2h;
        extern const VertexInputStructure<vertexlayout::Pos3hColor3h> pos3hColor3h;
        extern const VertexInputStructure<vertexlayout::Pos3hColor4h> pos3hColor4h;
        extern const VertexInputStructure<vertexlayout::Pos3hColor4b> pos3hColor4b;
        extern const VertexInputStructure<vertexlayout::Pos3hNormal3h> pos3hNormal3h;

        extern const VertexInputStructure<vertexlayout::Pos3hNormal3hTex2h> pos3hNormal3hTex2h;
        extern const VertexInputStructure<vertexlayout::Pos3hNormal3hColor3h> pos3hNormal3hColor3h;
        extern const VertexInputStructure<vertexlayout::Pos3hNormal3hColor4h> pos3hNormal3hColor4h;
        extern const VertexInputStructure<vertexlayout::Pos3hNormal3hColor4b> pos3hNormal3hColor4b;

        /* Vertex input states with float type. */

        extern const VertexInputStructure<core::float2> pos2f;
        extern const VertexInputStructure<core::float3> pos3f;
        extern const VertexInputStructure<core::float4> pos4f;

        extern const VertexInputStructure<vertexlayout::Pos2fTex2f> pos2fTex2f;
        extern const VertexInputStructure<vertexlayout::Pos2fColor3f> pos2fColor3f;
        extern const VertexInputStructure<vertexlayout::Pos2fColor4f> pos2fColor4f;
        extern const VertexInputStructure<vertexlayout::Pos2fColor4b> pos2fColor4b;

        extern const VertexInputStructure<vertexlayout::Pos3fTex2f> pos3fTex2f;
        extern const VertexInputStructure<vertexlayout::Pos3fColor3f> pos3fColor3f;
        extern const VertexInputStructure<vertexlayout::Pos3fColor4f> pos3fColor4f;
        extern const VertexInputStructure<vertexlayout::Pos3fColor4b> pos3fColor4b;
        extern const VertexInputStructure<vertexlayout::Pos3fNormal3f> pos3fNormal3f;

        extern const VertexInputStructure<vertexlayout::Pos3fNormal3fTex2f> pos3fNormal3fTex2f;
        extern const VertexInputStructure<vertexlayout::Pos3fNormal3fColor3f> pos3fNormal3fColor3f;
        extern const VertexInputStructure<vertexlayout::Pos3fNormal3fColor4f> pos3fNormal3fColor4f;
        extern const VertexInputStructure<vertexlayout::Pos3fNormal3fColor4b> pos3fNormal3fColor4b;
    } // namespace renderstates
} // namespace magma

#include "vertexInputStructure.inl"
