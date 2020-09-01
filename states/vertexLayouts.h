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
#include "../core/vectors.h"

namespace magma
{
    namespace vertexlayout
    {
        /* Templates for common vertex layouts. */

        template<typename Position, typename Attribute>
        struct Vertex
        {
            Position position;
            Attribute attrib;
        };

        template<typename Position, typename Normal, typename Attribute>
        struct LitVertex
        {
            Position position;
            Normal normal;
            Attribute attrib;
        };

        template<typename Position, typename Normal, typename Attribute>
        struct BumpVertex
        {
            Position position;
            Normal tangent;
            Normal bitangent;
            Normal normal;
            Attribute attrib;
        };

        /* Common vertex structures with half float type. */

        typedef Vertex<core::half2, core::half2> Pos2hTex2h;
        typedef Vertex<core::half2, core::half3> Pos2hColor3h;
        typedef Vertex<core::half2, core::half4> Pos2hColor4h;
        typedef Vertex<core::half2, core::ubyte4> Pos2hColor4b;

        typedef Vertex<core::half3, core::half2> Pos3hTex2h;
        typedef Vertex<core::half3, core::half3> Pos3hColor3h;
        typedef Vertex<core::half3, core::half4> Pos3hColor4h;
        typedef Vertex<core::half3, core::ubyte4> Pos3hColor4b;
        typedef Vertex<core::half3, core::half3> Pos3hNormal3h;

        typedef LitVertex<core::half3, core::half3, core::half2> Pos3hNormal3hTex2h;
        typedef LitVertex<core::half3, core::half3, core::half3> Pos3hNormal3hColor3h;
        typedef LitVertex<core::half3, core::half3, core::half4> Pos3hNormal3hColor4h;
        typedef LitVertex<core::half3, core::half3, core::ubyte4> Pos3hNormal3hColor4b;

        typedef BumpVertex<core::half3, core::half3, core::half2> Pos3hTBN3hTex2h;

        /* Common vertex structures with float type. */

        typedef Vertex<core::float2, core::float2> Pos2fTex2f;
        typedef Vertex<core::float2, core::float3> Pos2fColor3f;
        typedef Vertex<core::float2, core::float4> Pos2fColor4f;
        typedef Vertex<core::float2, core::ubyte4> Pos2fColor4b;

        typedef Vertex<core::float3, core::float2> Pos3fTex2f;
        typedef Vertex<core::float3, core::float3> Pos3fColor3f;
        typedef Vertex<core::float3, core::float4> Pos3fColor4f;
        typedef Vertex<core::float3, core::ubyte4> Pos3fColor4b;
        typedef Vertex<core::float3, core::float3> Pos3fNormal3f;

        typedef LitVertex<core::float3, core::float3, core::float2> Pos3fNormal3fTex2f;
        typedef LitVertex<core::float3, core::float3, core::float3> Pos3fNormal3fColor3f;
        typedef LitVertex<core::float3, core::float3, core::float4> Pos3fNormal3fColor4f;
        typedef LitVertex<core::float3, core::float3, core::ubyte4> Pos3fNormal3fColor4b;

        typedef BumpVertex<core::float3, core::float3, core::float2> Pos3fTBN3fTex2f;
    } // namespace vertexlayout
} // namespace magma
