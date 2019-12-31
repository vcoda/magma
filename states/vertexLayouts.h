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

namespace magma
{
    /* Half type definition to distinguish from uint16_t type. */

    struct half_t
    {
        uint16_t value;
    };

    namespace vertexlayout
    {
        /* Common vector types. */

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

        /* Templates for common vertex layouts. */

        template<typename Position,
            typename Attribute>
        struct Vertex
        {
            Position position;
            Attribute attrib;
        };

        template<typename Position,
            typename Normal,
            typename Attribute>
        struct LitVertex
        {
            Position position;
            Normal normal;
            Attribute attrib;
        };

        template<typename Position,
            typename Normal,
            typename Attribute>
        struct BumpVertex
        {
            Position position;
            Normal normal;
            Normal tangent;
            Normal bitangent;
            Attribute attrib;
        };

        /* Common vertex structures with half float type. */

        typedef Vertex<half2, half2> Pos2hTex2h;
        typedef Vertex<half2, half3> Pos2hColor3h;
        typedef Vertex<half2, half4> Pos2hColor4h;
        typedef Vertex<half2, ubyte4> Pos2hColor4b;

        typedef Vertex<half3, half2> Pos3hTex2h;
        typedef Vertex<half3, half3> Pos3hColor3h;
        typedef Vertex<half3, half4> Pos3hColor4h;
        typedef Vertex<half3, ubyte4> Pos3hColor4b;
        typedef Vertex<half3, half3> Pos3hNormal3h;

        typedef LitVertex<half3, half3, half2> Pos3hNormal3hTex2h;
        typedef LitVertex<half3, half3, half3> Pos3hNormal3hColor3h;
        typedef LitVertex<half3, half3, half4> Pos3hNormal3hColor4h;
        typedef LitVertex<half3, half3, ubyte4> Pos3hNormal3hColor4b;

        /* Typical vertex structures with float type. */

        typedef Vertex<float2, float2> Pos2fTex2f;
        typedef Vertex<float2, float3> Pos2fColor3f;
        typedef Vertex<float2, float4> Pos2fColor4f;
        typedef Vertex<float2, ubyte4> Pos2fColor4b;

        typedef Vertex<float3, float2> Pos3fTex2f;
        typedef Vertex<float3, float3> Pos3fColor3f;
        typedef Vertex<float3, float4> Pos3fColor4f;
        typedef Vertex<float3, ubyte4> Pos3fColor4b;
        typedef Vertex<float3, float3> Pos3fNormal3f;

        typedef LitVertex<float3, float3, float2> Pos3fNormal3fTex2f;
        typedef LitVertex<float3, float3, float3> Pos3fNormal3fColor3f;
        typedef LitVertex<float3, float3, float4> Pos3fNormal3fColor4f;
        typedef LitVertex<float3, float3, ubyte4> Pos3fNormal3fColor4b;
    } // namespace vertexlayout
} // namespace magma
