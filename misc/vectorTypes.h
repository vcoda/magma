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
#include <cstdint>

namespace magma
{
    /* Half format definition to avoid collision with uin16_t type. */

    struct half_t
    {
        uint16_t value;
    };

    namespace vectors
    {
        /* Template for common vector types. */

        template<typename T, int dimension>
        struct vector
        {
            T v[dimension];
        };

        typedef vector<uint8_t, 4> ubyte4;
        typedef vector<half_t, 2> half2;
        typedef vector<half_t, 3> half3;
        typedef vector<half_t, 4> half4;
        typedef vector<float, 2> float2;
        typedef vector<float, 3> float3;
        typedef vector<float, 4> float4;
        typedef vector<double, 2> double2;
        typedef vector<double, 3> double3;
        typedef vector<double, 4> double4;
    } // namespace vectors
} // namespace magma
