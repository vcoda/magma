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
#include "vertexFormat.h"
#include "vertexInputState.h"

namespace magma
{
    /* Vertex input structure template. */

    template<class Vertex, std::size_t VertexAttributeCount>
    class VertexInputStructure : public VertexInputState
    {
        static_assert(sizeof(Vertex) >= sizeof(uint32_t),
            "invalid size of vertex structure");
        static_assert(sizeof(Vertex) % sizeof(uint32_t) == 0,
            "size of vertex structure should be a multiple of 4 bytes");
        static_assert(VertexAttributeCount > 0, "invalid count of vertex attributes");

    public:
        constexpr VertexInputStructure() noexcept;
        constexpr VertexInputStructure(const std::array<VertexInputAttribute, VertexAttributeCount>& attributes,
            VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX) noexcept;
        template<std::size_t VertexInputBindingCount>
        constexpr VertexInputStructure(const std::array<VertexInputAttribute, VertexAttributeCount>& attributes,
            const std::array<VkVertexInputRate, VertexInputBindingCount>& inputRates) noexcept;
    #ifdef VK_EXT_vertex_attribute_divisor
        template<std::size_t VertexBindingDivisorCount>
        constexpr VertexInputStructure(const std::array<VertexInputAttribute, VertexAttributeCount>& attributes,
            const std::array<uint32_t, VertexBindingDivisorCount>& divisors) noexcept;
    #endif // VK_EXT_vertex_attribute_divisor

    private:
        VkVertexInputBindingDescription vertexBindings[VertexAttributeCount];
        VkVertexInputAttributeDescription vertexInputAttributes[VertexAttributeCount];
    #ifdef VK_EXT_vertex_attribute_divisor
        VkVertexInputBindingDivisorDescriptionEXT vertexBindingDivisors[VertexAttributeCount];
    #endif
    };

    namespace specialization
    {
        /* User have to specialize this template for concrete
           type and corresponding Vulkan format. */

        template<class T>
        struct VertexAttribute : AttributeFormat<VK_FORMAT_UNDEFINED>
        {
            constexpr static std::size_t size() noexcept { return sizeof(T); }
        };
    } // namespace specialization
} // namespace magma

#include "vertexInputStructure.inl"

/* GCC complains about extra qualification inside the same namespace
   and specialization of type in different namespace, so hence we
   break up the namespace to define this macro properly. See:
   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480*/

#if defined(__GNUC__)
#define MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(Type, Format)\
namespace magma { namespace specialization {\
template<> struct VertexAttribute<Type> : AttributeFormat<Format> {};\
}}
#else
#define MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(Type, Format)\
template<> struct magma::specialization::VertexAttribute<Type> :\
    magma::AttributeFormat<Format> {};
#endif // __GNUC__

#define MAGMA_VERTEX_ATTRIBUTE(Vertex, attribute, location)\
magma::VertexInputAttribute{location, 0,\
    magma::specialization::VertexAttribute<std::remove_cv_t<decltype(Vertex::attribute)>>::format(),\
    offsetof(Vertex, attribute)\
}

#define MAGMA_VERTEX_STREAM_ATTRIBUTE(Vertex, attribute, location, binding)\
magma::VertexInputAttribute{location, binding,\
    magma::specialization::VertexAttribute<std::remove_cv_t<decltype(Vertex::attribute)>>::format(),\
    0\
}

/* Built-in specializations for scalar vertex input types. */

MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UByteNorm, VK_FORMAT_R8_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ByteNorm, VK_FORMAT_R8_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UByte, VK_FORMAT_R8_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Byte, VK_FORMAT_R8_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShortNorm, VK_FORMAT_R16_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ShortNorm, VK_FORMAT_R16_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShort, VK_FORMAT_R16_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Short, VK_FORMAT_R16_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Half, VK_FORMAT_R16_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UInt, VK_FORMAT_R32_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Int, VK_FORMAT_R32_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Float, VK_FORMAT_R32_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeUInt, VK_FORMAT_R64_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeInt, VK_FORMAT_R64_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Double, VK_FORMAT_R64_SFLOAT)

/* Built-in specializations for vectorized vertex input types. */

MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UByteNorm2, VK_FORMAT_R8G8_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ByteNorm2, VK_FORMAT_R8G8_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UByte2, VK_FORMAT_R8G8_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Byte2, VK_FORMAT_R8G8_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UByteNorm4, VK_FORMAT_R8G8B8A8_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ByteNorm4, VK_FORMAT_R8G8B8A8_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UByte4, VK_FORMAT_R8G8B8A8_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Byte4, VK_FORMAT_R8G8B8A8_SINT)

MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShortNorm2, VK_FORMAT_R16G16_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ShortNorm2, VK_FORMAT_R16G16_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShort2, VK_FORMAT_R16G16_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Short2, VK_FORMAT_R16G16_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShortNorm3, VK_FORMAT_R16G16B16_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ShortNorm3, VK_FORMAT_R16G16B16_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShort3, VK_FORMAT_R16G16B16_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Short3, VK_FORMAT_R16G16B16_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShortNorm4, VK_FORMAT_R16G16B16A16_UNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::ShortNorm4, VK_FORMAT_R16G16B16A16_SNORM)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UShort4, VK_FORMAT_R16G16B16A16_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Short4, VK_FORMAT_R16G16B16A16_SINT)

MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UInt2, VK_FORMAT_R32G32_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Int2, VK_FORMAT_R32G32_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UInt3, VK_FORMAT_R32G32B32_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Int3, VK_FORMAT_R32G32B32_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::UInt4, VK_FORMAT_R32G32B32A32_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Int4, VK_FORMAT_R32G32B32A32_SINT)

MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeUInt2, VK_FORMAT_R64G64_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeInt2, VK_FORMAT_R64G64_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeUInt3, VK_FORMAT_R64G64B64_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeInt3, VK_FORMAT_R64G64B64_SINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeUInt4, VK_FORMAT_R64G64B64A64_UINT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::LargeInt4, VK_FORMAT_R64G64B64A64_SINT)

MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Half2, VK_FORMAT_R16G16_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Half3, VK_FORMAT_R16G16B16_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Half4, VK_FORMAT_R16G16B16A16_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Float2, VK_FORMAT_R32G32_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Float3, VK_FORMAT_R32G32B32_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Float4, VK_FORMAT_R32G32B32A32_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Double2, VK_FORMAT_R64G64_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Double3, VK_FORMAT_R64G64B64_SFLOAT)
MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(magma::Double4, VK_FORMAT_R64G64B64A64_SFLOAT)

#define MAGMA_VERTEX(Vertex, format, name)\
constexpr magma::VertexInputState name(\
    {0, 0, format, 0}, sizeof(Vertex)\
);

#define MAGMA_COLOR_VERTEX(Vertex, name)\
constexpr magma::VertexInputStructure<Vertex, 2> name(\
{\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, pos, 0),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, color, 1)\
});

#define MAGMA_TEX_VERTEX(Vertex, name)\
constexpr magma::VertexInputStructure<Vertex, 2> name(\
{\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, pos, 0),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, uv, 1)\
});

#define MAGMA_LIT_VERTEX(Vertex, name)\
constexpr magma::VertexInputStructure<Vertex, 2> name(\
{\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, pos, 0),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, normal, 1)\
});

#define MAGMA_LIT_TEX_VERTEX(Vertex, name)\
constexpr magma::VertexInputStructure<Vertex, 3> name(\
{\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, pos, 0),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, normal, 1),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, uv, 2)\
});

#define MAGMA_BUMP_VERTEX(Vertex, name)\
constexpr magma::VertexInputStructure<Vertex, 5> name(\
{\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, pos, 0),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, normal, 1),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, tangent, 2),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, bitangent, 3),\
    MAGMA_VERTEX_ATTRIBUTE(Vertex, uv, 4)\
});

namespace magma::renderstate
{
    /* No input vertex data. */

    constexpr VertexInputStructure<int, 1> nullVertexInput;

    /* Vertex position only. */

    MAGMA_VERTEX(vt::Pos2h, VK_FORMAT_R16G16_SFLOAT, pos2h);
    MAGMA_VERTEX(vt::Pos2f, VK_FORMAT_R32G32_SFLOAT, pos2f);
    MAGMA_VERTEX(vt::Pos2d, VK_FORMAT_R64G64_SFLOAT, pos2d);
    MAGMA_VERTEX(vt::Pos3f, VK_FORMAT_R32G32B32_SFLOAT, pos3f);
    MAGMA_VERTEX(vt::Pos3d, VK_FORMAT_R64G64B64_SFLOAT, pos3d);
    MAGMA_VERTEX(vt::Pos4h, VK_FORMAT_R16G16B16A16_SFLOAT, pos4h);
    MAGMA_VERTEX(vt::Pos4f, VK_FORMAT_R32G32B32A32_SFLOAT, pos4f);
    MAGMA_VERTEX(vt::Pos4d, VK_FORMAT_R64G64B64A64_SFLOAT, pos4d);

    /* Vertex position and 32-bit (r,g,b,a) color. */

    MAGMA_COLOR_VERTEX(vt::Pos2hColor4ub, pos2hColor4ub)
    MAGMA_COLOR_VERTEX(vt::Pos2fColor4ub, pos2fColor4ub)
    MAGMA_COLOR_VERTEX(vt::Pos2dColor4ub, pos2dColor4ub)
    MAGMA_COLOR_VERTEX(vt::Pos4hColor4ub, pos4hColor4ub)
    MAGMA_COLOR_VERTEX(vt::Pos3fColor4ub, pos3fColor4ub)
    MAGMA_COLOR_VERTEX(vt::Pos3dColor4ub, pos3dColor4ub)

    /* Vertex position and texture (u,v) coordinates. */

    MAGMA_TEX_VERTEX(vt::Pos2hTex2us, pos2hTex2us)
    MAGMA_TEX_VERTEX(vt::Pos2hTex2h, pos2hTex2h)
    MAGMA_TEX_VERTEX(vt::Pos2hTex2f, pos2hTex2f)
    MAGMA_TEX_VERTEX(vt::Pos2fTex2us, pos2fTex2us)
    MAGMA_TEX_VERTEX(vt::Pos2fTex2h, pos2fTex2h)
    MAGMA_TEX_VERTEX(vt::Pos2fTex2f, pos2fTex2f)
    MAGMA_TEX_VERTEX(vt::Pos2dTex2us, pos2dTex2us)
    MAGMA_TEX_VERTEX(vt::Pos2dTex2h, pos2dTex2h)
    MAGMA_TEX_VERTEX(vt::Pos2dTex2f, pos2dTex2f)
    MAGMA_TEX_VERTEX(vt::Pos4hTex2us, pos4hTex2us)
    MAGMA_TEX_VERTEX(vt::Pos4hTex2h, pos4hTex2h)
    MAGMA_TEX_VERTEX(vt::Pos4hTex2f, pos4hTex2f)
    MAGMA_TEX_VERTEX(vt::Pos3fTex2us, pos3fTex2us)
    MAGMA_TEX_VERTEX(vt::Pos3fTex2h, pos3fTex2h)
    MAGMA_TEX_VERTEX(vt::Pos3fTex2f, pos3fTex2f)
    MAGMA_TEX_VERTEX(vt::Pos3dTex2us, pos3dTex2us)
    MAGMA_TEX_VERTEX(vt::Pos3dTex2h, pos3dTex2h)
    MAGMA_TEX_VERTEX(vt::Pos3dTex2f, pos3dTex2f)

    /* Vertex position and normal. */

    MAGMA_LIT_VERTEX(vt::Pos4hNormal4b, pos4hNormal4b)
    MAGMA_LIT_VERTEX(vt::Pos4hNormal4ub, pos4hNormal4ub)
    MAGMA_LIT_VERTEX(vt::Pos4hNormal4s, pos4hNormal4s)
    MAGMA_LIT_VERTEX(vt::Pos4hNormal4us, pos4hNormal4us)
    MAGMA_LIT_VERTEX(vt::Pos4hNormal4h, pos4hNormal4h)
    MAGMA_LIT_VERTEX(vt::Pos4hNormal3f, pos4hNormal3f)
    MAGMA_LIT_VERTEX(vt::Pos3fNormal4b, pos3fNormal4b)
    MAGMA_LIT_VERTEX(vt::Pos3fNormal4ub, pos3fNormal4ub)
    MAGMA_LIT_VERTEX(vt::Pos3fNormal4s, pos3fNormal4s)
    MAGMA_LIT_VERTEX(vt::Pos3fNormal4us, pos3fNormal4us)
    MAGMA_LIT_VERTEX(vt::Pos3fNormal4h, pos3fNormal4h)
    MAGMA_LIT_VERTEX(vt::Pos3fNormal3f, pos3fNormal3f)
    MAGMA_LIT_VERTEX(vt::Pos3dNormal4b, pos3dNormal4b)
    MAGMA_LIT_VERTEX(vt::Pos3dNormal4ub, pos3dNormal4ub)
    MAGMA_LIT_VERTEX(vt::Pos3dNormal4s, pos3dNormal4s)
    MAGMA_LIT_VERTEX(vt::Pos3dNormal4us, pos3dNormal4us)
    MAGMA_LIT_VERTEX(vt::Pos3dNormal4h, pos3dNormal4h)
    MAGMA_LIT_VERTEX(vt::Pos3dNormal3f, pos3dNormal3f)

    /* Vertex position, normal and texture coordinates. */

    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4bTex2us, pos4hNormal4bTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4bTex2h, pos4hNormal4bTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4bTex2f, pos4hNormal4bTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4ubTex2us, pos4hNormal4ubTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4ubTex2h, pos4hNormal4ubTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4ubTex2f, pos4hNormal4ubTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4sTex2us, pos4hNormal4sTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4sTex2h, pos4hNormal4sTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4sTex2f, pos4hNormal4sTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4usTex2us, pos4hNormal4usTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4usTex2h, pos4hNormal4usTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4usTex2f, pos4hNormal4usTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4hTex2us, pos4hNormal4hTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4hTex2h, pos4hNormal4hTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal4hTex2f, pos4hNormal4hTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal3fTex2us, pos4hNormal3fTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal3fTex2h, pos4hNormal3fTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos4hNormal3fTex2f, pos4hNormal3fTex2f)

    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4bTex2us, pos3fNormal4bTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4bTex2h, pos3fNormal4bTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4bTex2f, pos3fNormal4bTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4ubTex2us, pos3fNormal4ubTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4ubTex2h, pos3fNormal4ubTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4ubTex2f, pos3fNormal4ubTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4sTex2us, pos3fNormal4sTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4sTex2h, pos3fNormal4sTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4sTex2f, pos3fNormal4sTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4usTex2us, pos3fNormal4usTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4usTex2h, pos3fNormal4usTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4usTex2f, pos3fNormal4usTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4hTex2us, pos3fNormal4hTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4hTex2h, pos3fNormal4hTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal4hTex2f, pos3fNormal4hTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal3fTex2us, pos3fNormal3fTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal3fTex2h, pos3fNormal3fTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3fNormal3fTex2f, pos3fNormal3fTex2f)

    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4bTex2us, pos3dNormal4bTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4bTex2h, pos3dNormal4bTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4bTex2f, pos3dNormal4bTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4ubTex2us, pos3dNormal4ubTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4ubTex2h, pos3dNormal4ubTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4ubTex2f, pos3dNormal4ubTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4sTex2us, pos3dNormal4sTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4sTex2h, pos3dNormal4sTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4sTex2f, pos3dNormal4sTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4usTex2us, pos3dNormal4usTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4usTex2h, pos3dNormal4usTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4usTex2f, pos3dNormal4usTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4hTex2us, pos3dNormal4hTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4hTex2h, pos3dNormal4hTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal4hTex2f, pos3dNormal4hTex2f)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal3fTex2us, pos3dNormal3fTex2us)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal3fTex2h, pos3dNormal3fTex2h)
    MAGMA_LIT_TEX_VERTEX(vt::Pos3dNormal3fTex2f, pos3dNormal3fTex2f)

    /* Vertex position, TBN basis and texture coordinates. */

    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4bTex2us, pos4hTbn4bTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4bTex2h, pos4hTbn4bTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4bTex2f, pos4hTbn4bTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4ubTex2us, pos4hTbn4ubTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4ubTex2h, pos4hTbn4ubTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4ubTex2f, pos4hTbn4ubTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4sTex2us, pos4hTbn4sTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4sTex2h, pos4hTbn4sTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4sTex2f, pos4hTbn4sTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4usTex2us, pos4hTbn4usTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4usTex2h, pos4hTbn4usTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4usTex2f, pos4hTbn4usTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4hTex2us, pos4hTbn4hTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4hTex2h, pos4hTbn4hTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn4hTex2f, pos4hTbn4hTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn3fTex2us, pos4hTbn3fTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn3fTex2h, pos4hTbn3fTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos4hTbn3fTex2f, pos4hTbn3fTex2f)

    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4bTex2us, pos3fTbn4bTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4bTex2h, pos3fTbn4bTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4bTex2f, pos3fTbn4bTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4ubTex2us, pos3fTbn4ubTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4ubTex2h, pos3fTbn4ubTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4ubTex2f, pos3fTbn4ubTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4sTex2us, pos3fTbn4sTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4sTex2h, pos3fTbn4sTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4sTex2f, pos3fTbn4sTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4usTex2us, pos3fTbn4usTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4usTex2h, pos3fTbn4usTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4usTex2f, pos3fTbn4usTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4hTex2us, pos3fTbn4hTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4hTex2h, pos3fTbn4hTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn4hTex2f, pos3fTbn4hTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn3fTex2us, pos3fTbn3fTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn3fTex2h, pos3fTbn3fTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3fTbn3fTex2f, pos3fTbn3fTex2f)

    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4bTex2us, pos3dTbn4bTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4bTex2h, pos3dTbn4bTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4bTex2f, pos3dTbn4bTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4ubTex2us, pos3dTbn4ubTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4ubTex2h, pos3dTbn4ubTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4ubTex2f, pos3dTbn4ubTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4sTex2us, pos3dTbn4sTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4sTex2h, pos3dTbn4sTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4sTex2f, pos3dTbn4sTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4usTex2us, pos3dTbn4usTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4usTex2h, pos3dTbn4usTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4usTex2f, pos3dTbn4usTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4hTex2us, pos3dTbn4hTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4hTex2h, pos3dTbn4hTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn4hTex2f, pos3dTbn4hTex2f)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn3fTex2us, pos3dTbn3fTex2us)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn3fTex2h, pos3dTbn3fTex2h)
    MAGMA_BUMP_VERTEX(vt::Pos3dTbn3fTex2f, pos3dTbn3fTex2f)
} // namespace magma::renderstate
