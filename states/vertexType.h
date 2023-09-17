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

namespace magma
{
    /* Half type definition to distinguish from integer uint16_t type. */

    struct half_t
    {
        uint16_t value;
    };

    /* Scalar or vector GPU type. <Unsigned> and <Normalized>
       parameters are used to distinguish Vulkan formats with
       UNORM, SNORM, UINT and SINT modificators. */

    template<class PodType, size_t Components, bool Unsigned, bool Normalized = false>
    struct AttributeType
    {
        PodType v[Components];
        constexpr static bool unsigned_() noexcept { return Unsigned; }
        constexpr static bool normalized() noexcept { return Normalized; }
    };

    /* Scalar vertex input types. */

    typedef AttributeType<uint8_t, 1, true, true> UByteNorm;
    typedef AttributeType<int8_t, 1, false, true> ByteNorm;
    typedef AttributeType<uint8_t, 1, true> UByte;
    typedef AttributeType<int8_t, 1, false> Byte;
    typedef AttributeType<uint16_t, 1, true, true> UShortNorm;
    typedef AttributeType<int16_t, 1, false, true> ShortNorm;
    typedef AttributeType<uint16_t, 1, true> UShort;
    typedef AttributeType<int16_t, 1, false> Short;
    typedef AttributeType<half_t, 1, false> Half;
    typedef AttributeType<uint32_t, 1, true> UInt;
    typedef AttributeType<int32_t, 1, false> Int;
    typedef AttributeType<float, 1, false> Float;
    typedef AttributeType<uint64_t, 1, true> LargeUInt;
    typedef AttributeType<int64_t, 1, false> LargeInt;
    typedef AttributeType<double, 1, false> Double;

    /* Vectorized vertex input types. */

    typedef AttributeType<uint8_t, 2, true, true> UByteNorm2;
    typedef AttributeType<int8_t, 2, false, true> ByteNorm2;
    typedef AttributeType<uint8_t, 2, true> UByte2;
    typedef AttributeType<int8_t, 2, false> Byte2;
    typedef AttributeType<uint8_t, 4, true, true> UByteNorm4;
    typedef AttributeType<int8_t, 4, false, true> ByteNorm4;
    typedef AttributeType<uint8_t, 4, true> UByte4;
    typedef AttributeType<int8_t, 4, false> Byte4;

    typedef AttributeType<uint16_t, 2, true, true> UShortNorm2;
    typedef AttributeType<int16_t, 2, false, true> ShortNorm2;
    typedef AttributeType<uint16_t, 2, true> UShort2;
    typedef AttributeType<int16_t, 2, false> Short2;
    typedef AttributeType<uint16_t, 3, true, true> UShortNorm3;
    typedef AttributeType<int16_t, 3, false, true> ShortNorm3;
    typedef AttributeType<uint16_t, 3, true> UShort3;
    typedef AttributeType<int16_t, 3, false> Short3;
    typedef AttributeType<uint16_t, 4, true, true> UShortNorm4;
    typedef AttributeType<int16_t, 4, false, true> ShortNorm4;
    typedef AttributeType<uint16_t, 4, true> UShort4;
    typedef AttributeType<int16_t, 4, false> Short4;

    typedef AttributeType<uint32_t, 2, true> UInt2;
    typedef AttributeType<int32_t, 2, false> Int2;
    typedef AttributeType<uint32_t, 3, true> UInt3;
    typedef AttributeType<int32_t, 3, false> Int3;
    typedef AttributeType<uint32_t, 4, true> UInt4;
    typedef AttributeType<int32_t, 4, false> Int4;

    typedef AttributeType<uint64_t, 2, true> LargeUInt2;
    typedef AttributeType<int64_t, 2, false> LargeInt2;
    typedef AttributeType<uint64_t, 3, true> LargeUInt3;
    typedef AttributeType<int64_t, 3, false> LargeInt3;
    typedef AttributeType<uint64_t, 4, true> LargeUInt4;
    typedef AttributeType<int64_t, 4, false> LargeInt4;

    typedef AttributeType<half_t, 2, false> Half2;
    typedef AttributeType<half_t, 3, false> Half3;
    typedef AttributeType<half_t, 4, false> Half4;
    typedef AttributeType<float, 2, false> Float2;
    typedef AttributeType<float, 3, false> Float3;
    typedef AttributeType<float, 4, false> Float4;
    typedef AttributeType<double, 2, false> Double2;
    typedef AttributeType<double, 3, false> Double3;
    typedef AttributeType<double, 4, false> Double4;
} // namespace magma
