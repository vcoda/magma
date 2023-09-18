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
#include "vertexAttributeType.h"

namespace magma
{
    namespace fvf
    {
        /* Flexible vertex formats for basic vertex shading. */

        template<class Pos>
        struct Vertex
        {
            Pos pos;
        };

        template<class Pos, class Color>
        struct ColorVertex
        {
            Pos pos;
            Color color;
        };

        template<class Pos, class TexCoord>
        struct TexVertex
        {
            Pos pos;
            TexCoord uv;
        };

        template<class Pos, class Normal>
        struct LitVertex
        {
            Pos pos;
            Normal normal;
        };

        template<class Pos, class Normal, class TexCoord>
        struct TexLitVertex
        {
            Pos pos;
            Normal normal;
            TexCoord uv;
        };

        template<class Pos, class Vector, class TexCoord>
        struct BumpVertex
        {
            Pos pos;
            Vector normal;
            Vector tangent;
            Vector bitangent;
            TexCoord uv;
        };

        typedef Vertex<Half2> Pos2h;
        typedef Vertex<Float2> Pos2f;
        typedef Vertex<Double2> Pos2d;
        typedef Vertex<Half3> Pos3h;
        typedef Vertex<Float3> Pos3f;
        typedef Vertex<Double3> Pos3d;
        typedef Vertex<Half4> Pos4h;
        typedef Vertex<Float4> Pos4f;
        typedef Vertex<Double4> Pos4d;

        typedef ColorVertex<Half2, UByteNorm4> Pos2hColor4ub;
        typedef ColorVertex<Float2, UByteNorm4> Pos2fColor4ub;
        typedef ColorVertex<Double2, UByteNorm4> Pos2dColor4ub;
        typedef ColorVertex<Half3, UByteNorm4> Pos3hColor4ub;
        typedef ColorVertex<Float3, UByteNorm4> Pos3fColor4ub;
        typedef ColorVertex<Double3, UByteNorm4> Pos3dColor4ub;
        typedef ColorVertex<Half4, UByteNorm4> Pos4hColor4ub;
        typedef ColorVertex<Float4, UByteNorm4> Pos4fColor4ub;
        typedef ColorVertex<Double4, UByteNorm4> Pos4dColor4ub;

        typedef TexVertex<Half2, UShortNorm2> Pos2hTex2us;
        typedef TexVertex<Half2, Half2> Pos2hTex2h;
        typedef TexVertex<Half2, Float2> Pos2hTex2f;
        typedef TexVertex<Float2, UShortNorm2> Pos2fTex2us;
        typedef TexVertex<Float2, Half2> Pos2fTex2h;
        typedef TexVertex<Float2, Float2> Pos2fTex2f;
        typedef TexVertex<Double2, UShortNorm2> Pos2dTex2us;
        typedef TexVertex<Double2, Half2> Pos2dTex2h;
        typedef TexVertex<Double2, Float2> Pos2dTex2f;
        typedef TexVertex<Half3, UShortNorm2> Pos3hTex2us;
        typedef TexVertex<Half3, Half2> Pos3hTex2h;
        typedef TexVertex<Half3, Float2> Pos3hTex2f;
        typedef TexVertex<Float3, UShortNorm2> Pos3fTex2us;
        typedef TexVertex<Float3, Half2> Pos3fTex2h;
        typedef TexVertex<Float3, Float2> Pos3fTex2f;
        typedef TexVertex<Double3, UShortNorm2> Pos3dTex2us;
        typedef TexVertex<Double3, Half2> Pos3dTex2h;
        typedef TexVertex<Double3, Float2> Pos3dTex2f;

        typedef LitVertex<Half3, Half3> Pos3hNormal3h;
        typedef LitVertex<Half3, UByteNorm4> Pos3hNormal4ub;
        typedef LitVertex<Float3, Half3> Pos3fNormal3h;
        typedef LitVertex<Float3, Float3> Pos3fNormal3f;
        typedef LitVertex<Float3, UByteNorm4> Pos3fNormal4ub;
        typedef LitVertex<Double3, Half3> Pos3dNormal3h;
        typedef LitVertex<Double3, Float3> Pos3dNormal3f;
        typedef LitVertex<Double3, UByteNorm4> Pos3dNormal4ub;
        typedef LitVertex<Half4, Half3> Pos4hNormal3h;
        typedef LitVertex<Half4, UByteNorm4> Pos4hNormal4ub;
        typedef LitVertex<Float4, Half3> Pos4fNormal3h;
        typedef LitVertex<Float4, Float3> Pos4fNormal3f;
        typedef LitVertex<Float4, UByteNorm4> Pos4fNormal4ub;
        typedef LitVertex<Double4, Half3> Pos4dNormal3h;
        typedef LitVertex<Double4, Float3> Pos4dNormal3f;
        typedef LitVertex<Double4, UByteNorm4> Pos4dNormal4ub;

        typedef BumpVertex<Half3, UByteNorm4, Half2> Pos3hTBN4ubTex2h;
        typedef BumpVertex<Float3, UByteNorm4, Half2> Pos3fTBN4ubTex2h;
        typedef BumpVertex<Float3, UByteNorm4, Float2> Pos3fTBN4ubTex2f;
        typedef BumpVertex<Double3, UByteNorm4, Half2> Pos3dTBN4ubTex2h;
        typedef BumpVertex<Double3, UByteNorm4, Float2> Pos3dTBN4ubTex2f;
        typedef BumpVertex<Half4, UByteNorm4, Half2> Pos4hTBN4ubTex2h;
        typedef BumpVertex<Float4, UByteNorm4, Half2> Pos4fTBN4ubTex2h;
        typedef BumpVertex<Float4, UByteNorm4, Float2> Pos4fTBN4ubTex2f;
        typedef BumpVertex<Double4, UByteNorm4, Half2> Pos4dTBN4ubTex2h;
        typedef BumpVertex<Double4, UByteNorm4, Float2> Pos4dTBN4ubTex2f;
    } // namespace fvf
} // namespace magma
