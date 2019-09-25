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
#include "../misc/vectorTypes.h"

namespace magma
{
    namespace vertexlayout
    {
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

        /* Typical vertex structures with half float type. */

        typedef Vertex<vectors::half2, vectors::half2> Pos2hTex2h;
        typedef Vertex<vectors::half2, vectors::half3> Pos2hColor3h;
        typedef Vertex<vectors::half2, vectors::half4> Pos2hColor4h;
        typedef Vertex<vectors::half2, vectors::ubyte4> Pos2hColor4b;

        typedef Vertex<vectors::half3, vectors::half2> Pos3hTex2h;
        typedef Vertex<vectors::half3, vectors::half3> Pos3hColor3h;
        typedef Vertex<vectors::half3, vectors::half4> Pos3hColor4h;
        typedef Vertex<vectors::half3, vectors::ubyte4> Pos3hColor4b;
        typedef Vertex<vectors::half3, vectors::half3> Pos3hNormal3h;

        typedef LitVertex<vectors::half3, vectors::half3, vectors::half2> Pos3hNormal3hTex2h;
        typedef LitVertex<vectors::half3, vectors::half3, vectors::half3> Pos3hNormal3hColor3h;
        typedef LitVertex<vectors::half3, vectors::half3, vectors::half4> Pos3hNormal3hColor4h;
        typedef LitVertex<vectors::half3, vectors::half3, vectors::ubyte4> Pos3hNormal3hColor4b;

        /* Typical vertex structures with float type. */

        typedef Vertex<vectors::float2, vectors::float2> Pos2fTex2f;
        typedef Vertex<vectors::float2, vectors::float3> Pos2fColor3f;
        typedef Vertex<vectors::float2, vectors::float4> Pos2fColor4f;
        typedef Vertex<vectors::float2, vectors::ubyte4> Pos2fColor4b;

        typedef Vertex<vectors::float3, vectors::float2> Pos3fTex2f;
        typedef Vertex<vectors::float3, vectors::float3> Pos3fColor3f;
        typedef Vertex<vectors::float3, vectors::float4> Pos3fColor4f;
        typedef Vertex<vectors::float3, vectors::ubyte4> Pos3fColor4b;
        typedef Vertex<vectors::float3, vectors::float3> Pos3fNormal3f;

        typedef LitVertex<vectors::float3, vectors::float3, vectors::float2> Pos3fNormal3fTex2f;
        typedef LitVertex<vectors::float3, vectors::float3, vectors::float3> Pos3fNormal3fColor3f;
        typedef LitVertex<vectors::float3, vectors::float3, vectors::float4> Pos3fNormal3fColor4f;
        typedef LitVertex<vectors::float3, vectors::float3, vectors::ubyte4> Pos3fNormal3fColor4b;
    } // namespace vertexlayout
} // namespace magma
