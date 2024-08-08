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
#include "../misc/format.h"

namespace magma
{
    /* Half type definition to distinguish from integer uint16_t type. */

    struct half_t
    {
        uint16_t value;
    };

    /* https://registry.khronos.org/vulkan/specs/1.1/html/vkspec.html#_identification_of_formats
       Table 34. Interpretation of Numeric Format */

    enum class Numeric : uint8_t
    {
        UNorm, SNorm, UScaled, SScaled, UInt, SInt, UFloat, SFloat
    };

    /* Scalar or vector GPU type. <Normalized> and <Scaled>
       parameters are used to distinguish numeric formats such as
       UNORM/SNORM, USCALED/SSCALED, UINT/SINT and SFLOAT. */

    template<class PodType, int N,
        bool Normalized = false,
        bool Scaled = false>
    struct VertexAttributeType
    {
        PodType v[N];
        constexpr static std::size_t size() noexcept { return sizeof(PodType) * N; }
        constexpr static Numeric numeric() noexcept;
    };

    /* Each vertex attribute type should be associated
       with corresponding Vulkan format. */

    template<VkFormat Format>
    struct VertexAttributeFormat
    {
        constexpr static VkFormat format() noexcept;
    };

    /* Scalar vertex input types. */

    typedef VertexAttributeType<uint8_t, 1, true> UByteNorm;
    typedef VertexAttributeType<int8_t, 1, true> ByteNorm;
    typedef VertexAttributeType<uint8_t, 1> UByte;
    typedef VertexAttributeType<int8_t, 1> Byte;
    typedef VertexAttributeType<uint16_t, 1, true> UShortNorm;
    typedef VertexAttributeType<int16_t, 1, true> ShortNorm;
    typedef VertexAttributeType<uint16_t, 1> UShort;
    typedef VertexAttributeType<int16_t, 1> Short;
    typedef VertexAttributeType<half_t, 1> Half;
    typedef VertexAttributeType<uint32_t, 1> UInt;
    typedef VertexAttributeType<int32_t, 1> Int;
    typedef VertexAttributeType<float, 1> Float;
    typedef VertexAttributeType<uint64_t, 1> LargeUInt;
    typedef VertexAttributeType<int64_t, 1> LargeInt;
    typedef VertexAttributeType<double, 1> Double;

    /* Vectorized vertex input types.
       (Keep in mind that vertex attributes should be 4-byte aligned,
       so (U)Short3 and Half3 types may not be supported). */

    typedef VertexAttributeType<uint8_t, 2, true> UByteNorm2;
    typedef VertexAttributeType<int8_t, 2, true> ByteNorm2;
    typedef VertexAttributeType<uint8_t, 2> UByte2;
    typedef VertexAttributeType<int8_t, 2> Byte2;
    typedef VertexAttributeType<uint8_t, 4, true> UByteNorm4;
    typedef VertexAttributeType<int8_t, 4, true> ByteNorm4;
    typedef VertexAttributeType<uint8_t, 4> UByte4;
    typedef VertexAttributeType<int8_t, 4> Byte4;

    typedef VertexAttributeType<uint16_t, 2, true> UShortNorm2;
    typedef VertexAttributeType<int16_t, 2, true> ShortNorm2;
    typedef VertexAttributeType<uint16_t, 2> UShort2;
    typedef VertexAttributeType<int16_t, 2> Short2;
    typedef VertexAttributeType<uint16_t, 3, true> UShortNorm3;
    typedef VertexAttributeType<int16_t, 3, true> ShortNorm3;
    typedef VertexAttributeType<uint16_t, 3> UShort3;
    typedef VertexAttributeType<int16_t, 3> Short3;
    typedef VertexAttributeType<uint16_t, 4, true> UShortNorm4;
    typedef VertexAttributeType<int16_t, 4, true> ShortNorm4;
    typedef VertexAttributeType<uint16_t, 4> UShort4;
    typedef VertexAttributeType<int16_t, 4> Short4;

    typedef VertexAttributeType<uint32_t, 2> UInt2;
    typedef VertexAttributeType<int32_t, 2> Int2;
    typedef VertexAttributeType<uint32_t, 3> UInt3;
    typedef VertexAttributeType<int32_t, 3> Int3;
    typedef VertexAttributeType<uint32_t, 4> UInt4;
    typedef VertexAttributeType<int32_t, 4> Int4;

    typedef VertexAttributeType<uint64_t, 2> LargeUInt2;
    typedef VertexAttributeType<int64_t, 2> LargeInt2;
    typedef VertexAttributeType<uint64_t, 3> LargeUInt3;
    typedef VertexAttributeType<int64_t, 3> LargeInt3;
    typedef VertexAttributeType<uint64_t, 4> LargeUInt4;
    typedef VertexAttributeType<int64_t, 4> LargeInt4;

    typedef VertexAttributeType<half_t, 2> Half2;
    typedef VertexAttributeType<half_t, 3> Half3;
    typedef VertexAttributeType<half_t, 4> Half4;
    typedef VertexAttributeType<float, 2> Float2;
    typedef VertexAttributeType<float, 3> Float3;
    typedef VertexAttributeType<float, 4> Float4;
    typedef VertexAttributeType<double, 2> Double2;
    typedef VertexAttributeType<double, 3> Double3;
    typedef VertexAttributeType<double, 4> Double4;
} // namespace magma

#include "vertexAttributeType.inl"
