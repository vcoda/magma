/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

namespace magma::vt
{
    /* Generic vertex formats for basic vertex shading.
       Note that vertex attributes should be 4-byte aligned,
       so (u)short3 and half3 types are omitted. */

    /* Vertex as single 2D, 3D or 4D position. */

    template<class V>
    struct Vertex
    {
        V pos;
    };

    /* Vertex with position and color (r,g,b,a) values to
       draw triangles with per-vertex color interpolation. */

    template<class V, class C>
    struct ColorVertex
    {
        V pos;
        C color;
    };

    /* Vertex with position and texture (u,v) coordinates. */

    template<class V, class T>
    struct TexVertex
    {
        V pos;
        T uv;
    };

    /* Vertex with position and normal (x,y,z) vector
       to implement simple diffuse and specular lighting. */

    template<class V, class N>
    struct LitVertex
    {
        V pos;
        N normal;
    };

    /* Vertex with position, normal and texture coordinates
       to implement simple diffuse and specular lighting
       multiplied with diffuse texture. */

    template<class V, class N, class T>
    struct LitTexVertex
    {
        V pos;
        N normal;
        T uv;
    };

    /* Vertex with position, normal, tangent, bitangent
       vectors and texture coordinates to implement per-
       pixel bump mapping. */

    template<class V, class N, class T>
    struct BumpVertex
    {
        V pos;
        N normal;
        N tangent;
        N bitangent;
        T uv;
    };

    typedef Vertex<Half2> Pos2h;
    typedef Vertex<Float2> Pos2f;
    typedef Vertex<Double2> Pos2d;
    typedef Vertex<Float3> Pos3f;
    typedef Vertex<Double3> Pos3d;
    typedef Vertex<Half4> Pos4h;
    typedef Vertex<Float4> Pos4f;
    typedef Vertex<Double4> Pos4d;

    typedef ColorVertex<Half2, UByteNorm4> Pos2hColor4ub;
    typedef ColorVertex<Float2, UByteNorm4> Pos2fColor4ub;
    typedef ColorVertex<Double2, UByteNorm4> Pos2dColor4ub;
    typedef ColorVertex<Half4, UByteNorm4> Pos4hColor4ub;
    typedef ColorVertex<Float3, UByteNorm4> Pos3fColor4ub;
    typedef ColorVertex<Double3, UByteNorm4> Pos3dColor4ub;

    typedef TexVertex<Half2, UShortNorm2> Pos2hTex2us;
    typedef TexVertex<Half2, Half2> Pos2hTex2h;
    typedef TexVertex<Half2, Float2> Pos2hTex2f;
    typedef TexVertex<Float2, UShortNorm2> Pos2fTex2us;
    typedef TexVertex<Float2, Half2> Pos2fTex2h;
    typedef TexVertex<Float2, Float2> Pos2fTex2f;
    typedef TexVertex<Double2, UShortNorm2> Pos2dTex2us;
    typedef TexVertex<Double2, Half2> Pos2dTex2h;
    typedef TexVertex<Double2, Float2> Pos2dTex2f;
    typedef TexVertex<Half4, UShortNorm2> Pos4hTex2us;
    typedef TexVertex<Half4, Half2> Pos4hTex2h;
    typedef TexVertex<Half4, Float2> Pos4hTex2f;
    typedef TexVertex<Float3, UShortNorm2> Pos3fTex2us;
    typedef TexVertex<Float3, Half2> Pos3fTex2h;
    typedef TexVertex<Float3, Float2> Pos3fTex2f;
    typedef TexVertex<Double3, UShortNorm2> Pos3dTex2us;
    typedef TexVertex<Double3, Half2> Pos3dTex2h;
    typedef TexVertex<Double3, Float2> Pos3dTex2f;

    typedef LitVertex<Half4, ByteNorm4> Pos4hNormal4b;
    typedef LitVertex<Half4, UByteNorm4> Pos4hNormal4ub;
    typedef LitVertex<Half4, ShortNorm4> Pos4hNormal4s;
    typedef LitVertex<Half4, UShortNorm4> Pos4hNormal4us;
    typedef LitVertex<Half4, Half4> Pos4hNormal4h;
    typedef LitVertex<Half4, Float3> Pos4hNormal3f;
    typedef LitVertex<Float3, ByteNorm4> Pos3fNormal4b;
    typedef LitVertex<Float3, UByteNorm4> Pos3fNormal4ub;
    typedef LitVertex<Float3, ShortNorm4> Pos3fNormal4s;
    typedef LitVertex<Float3, UShortNorm4> Pos3fNormal4us;
    typedef LitVertex<Float3, Half4> Pos3fNormal4h;
    typedef LitVertex<Float3, Float3> Pos3fNormal3f;
    typedef LitVertex<Double3, ByteNorm4> Pos3dNormal4b;
    typedef LitVertex<Double3, UByteNorm4> Pos3dNormal4ub;
    typedef LitVertex<Double3, ShortNorm4> Pos3dNormal4s;
    typedef LitVertex<Double3, UShortNorm4> Pos3dNormal4us;
    typedef LitVertex<Double3, Half4> Pos3dNormal4h;
    typedef LitVertex<Double3, Float3> Pos3dNormal3f;

    typedef LitTexVertex<Half4, ByteNorm4, UShortNorm2> Pos4hNormal4bTex2us;
    typedef LitTexVertex<Half4, ByteNorm4, Half2> Pos4hNormal4bTex2h;
    typedef LitTexVertex<Half4, ByteNorm4, Float2> Pos4hNormal4bTex2f;
    typedef LitTexVertex<Half4, UByteNorm4, UShortNorm2> Pos4hNormal4ubTex2us;
    typedef LitTexVertex<Half4, UByteNorm4, Half2> Pos4hNormal4ubTex2h;
    typedef LitTexVertex<Half4, UByteNorm4, Float2> Pos4hNormal4ubTex2f;
    typedef LitTexVertex<Half4, ShortNorm4, UShortNorm2> Pos4hNormal4sTex2us;
    typedef LitTexVertex<Half4, ShortNorm4, Half2> Pos4hNormal4sTex2h;
    typedef LitTexVertex<Half4, ShortNorm4, Float2> Pos4hNormal4sTex2f;
    typedef LitTexVertex<Half4, UShortNorm4, UShortNorm2> Pos4hNormal4usTex2us;
    typedef LitTexVertex<Half4, UShortNorm4, Half2> Pos4hNormal4usTex2h;
    typedef LitTexVertex<Half4, UShortNorm4, Float2> Pos4hNormal4usTex2f;
    typedef LitTexVertex<Half4, Half4, UShortNorm2> Pos4hNormal4hTex2us;
    typedef LitTexVertex<Half4, Half4, Half2> Pos4hNormal4hTex2h;
    typedef LitTexVertex<Half4, Half4, Float2> Pos4hNormal4hTex2f;
    typedef LitTexVertex<Half4, Float3, UShortNorm2> Pos4hNormal3fTex2us;
    typedef LitTexVertex<Half4, Float3, Half2> Pos4hNormal3fTex2h;
    typedef LitTexVertex<Half4, Float3, Float2> Pos4hNormal3fTex2f;
    typedef LitTexVertex<Float3, ByteNorm4, UShortNorm2> Pos3fNormal4bTex2us;
    typedef LitTexVertex<Float3, ByteNorm4, Half2> Pos3fNormal4bTex2h;
    typedef LitTexVertex<Float3, ByteNorm4, Float2> Pos3fNormal4bTex2f;
    typedef LitTexVertex<Float3, UByteNorm4, UShortNorm2> Pos3fNormal4ubTex2us;
    typedef LitTexVertex<Float3, UByteNorm4, Half2> Pos3fNormal4ubTex2h;
    typedef LitTexVertex<Float3, UByteNorm4, Float2> Pos3fNormal4ubTex2f;
    typedef LitTexVertex<Float3, ShortNorm4, UShortNorm2> Pos3fNormal4sTex2us;
    typedef LitTexVertex<Float3, ShortNorm4, Half2> Pos3fNormal4sTex2h;
    typedef LitTexVertex<Float3, ShortNorm4, Float2> Pos3fNormal4sTex2f;
    typedef LitTexVertex<Float3, UShortNorm4, UShortNorm2> Pos3fNormal4usTex2us;
    typedef LitTexVertex<Float3, UShortNorm4, Half2> Pos3fNormal4usTex2h;
    typedef LitTexVertex<Float3, UShortNorm4, Float2> Pos3fNormal4usTex2f;
    typedef LitTexVertex<Float3, Half4, UShortNorm2> Pos3fNormal4hTex2us;
    typedef LitTexVertex<Float3, Half4, Half2> Pos3fNormal4hTex2h;
    typedef LitTexVertex<Float3, Half4, Float2> Pos3fNormal4hTex2f;
    typedef LitTexVertex<Float3, Float3, UShortNorm2> Pos3fNormal3fTex2us;
    typedef LitTexVertex<Float3, Float3, Half2> Pos3fNormal3fTex2h;
    typedef LitTexVertex<Float3, Float3, Float2> Pos3fNormal3fTex2f;
    typedef LitTexVertex<Double3, ByteNorm4, UShortNorm2> Pos3dNormal4bTex2us;
    typedef LitTexVertex<Double3, ByteNorm4, Half2> Pos3dNormal4bTex2h;
    typedef LitTexVertex<Double3, ByteNorm4, Float2> Pos3dNormal4bTex2f;
    typedef LitTexVertex<Double3, UByteNorm4, UShortNorm2> Pos3dNormal4ubTex2us;
    typedef LitTexVertex<Double3, UByteNorm4, Half2> Pos3dNormal4ubTex2h;
    typedef LitTexVertex<Double3, UByteNorm4, Float2> Pos3dNormal4ubTex2f;
    typedef LitTexVertex<Double3, ShortNorm4, UShortNorm2> Pos3dNormal4sTex2us;
    typedef LitTexVertex<Double3, ShortNorm4, Half2> Pos3dNormal4sTex2h;
    typedef LitTexVertex<Double3, ShortNorm4, Float2> Pos3dNormal4sTex2f;
    typedef LitTexVertex<Double3, UShortNorm4, UShortNorm2> Pos3dNormal4usTex2us;
    typedef LitTexVertex<Double3, UShortNorm4, Half2> Pos3dNormal4usTex2h;
    typedef LitTexVertex<Double3, UShortNorm4, Float2> Pos3dNormal4usTex2f;
    typedef LitTexVertex<Double3, Half4, UShortNorm2> Pos3dNormal4hTex2us;
    typedef LitTexVertex<Double3, Half4, Half2> Pos3dNormal4hTex2h;
    typedef LitTexVertex<Double3, Half4, Float2> Pos3dNormal4hTex2f;
    typedef LitTexVertex<Double3, Float3, UShortNorm2> Pos3dNormal3fTex2us;
    typedef LitTexVertex<Double3, Float3, Half2> Pos3dNormal3fTex2h;
    typedef LitTexVertex<Double3, Float3, Float2> Pos3dNormal3fTex2f;

    typedef BumpVertex<Half4, ByteNorm4, UShortNorm2> Pos4hTbn4bTex2us;
    typedef BumpVertex<Half4, ByteNorm4, Half2> Pos4hTbn4bTex2h;
    typedef BumpVertex<Half4, ByteNorm4, Float2> Pos4hTbn4bTex2f;
    typedef BumpVertex<Half4, UByteNorm4, UShortNorm2> Pos4hTbn4ubTex2us;
    typedef BumpVertex<Half4, UByteNorm4, Half2> Pos4hTbn4ubTex2h;
    typedef BumpVertex<Half4, UByteNorm4, Float2> Pos4hTbn4ubTex2f;
    typedef BumpVertex<Half4, ShortNorm4, UShortNorm2> Pos4hTbn4sTex2us;
    typedef BumpVertex<Half4, ShortNorm4, Half2> Pos4hTbn4sTex2h;
    typedef BumpVertex<Half4, ShortNorm4, Float2> Pos4hTbn4sTex2f;
    typedef BumpVertex<Half4, UShortNorm4, UShortNorm2> Pos4hTbn4usTex2us;
    typedef BumpVertex<Half4, UShortNorm4, Half2> Pos4hTbn4usTex2h;
    typedef BumpVertex<Half4, UShortNorm4, Float2> Pos4hTbn4usTex2f;
    typedef BumpVertex<Half4, Half4, UShortNorm2> Pos4hTbn4hTex2us;
    typedef BumpVertex<Half4, Half4, Half2> Pos4hTbn4hTex2h;
    typedef BumpVertex<Half4, Half4, Float2> Pos4hTbn4hTex2f;
    typedef BumpVertex<Half4, Float3, UShortNorm2> Pos4hTbn3fTex2us;
    typedef BumpVertex<Half4, Float3, Half2> Pos4hTbn3fTex2h;
    typedef BumpVertex<Half4, Float3, Float2> Pos4hTbn3fTex2f;
    typedef BumpVertex<Float3, ByteNorm4, UShortNorm2> Pos3fTbn4bTex2us;
    typedef BumpVertex<Float3, ByteNorm4, Half2> Pos3fTbn4bTex2h;
    typedef BumpVertex<Float3, ByteNorm4, Float2> Pos3fTbn4bTex2f;
    typedef BumpVertex<Float3, UByteNorm4, UShortNorm2> Pos3fTbn4ubTex2us;
    typedef BumpVertex<Float3, UByteNorm4, Half2> Pos3fTbn4ubTex2h;
    typedef BumpVertex<Float3, UByteNorm4, Float2> Pos3fTbn4ubTex2f;
    typedef BumpVertex<Float3, ShortNorm4, UShortNorm2> Pos3fTbn4sTex2us;
    typedef BumpVertex<Float3, ShortNorm4, Half2> Pos3fTbn4sTex2h;
    typedef BumpVertex<Float3, ShortNorm4, Float2> Pos3fTbn4sTex2f;
    typedef BumpVertex<Float3, UShortNorm4, UShortNorm2> Pos3fTbn4usTex2us;
    typedef BumpVertex<Float3, UShortNorm4, Half2> Pos3fTbn4usTex2h;
    typedef BumpVertex<Float3, UShortNorm4, Float2> Pos3fTbn4usTex2f;
    typedef BumpVertex<Float3, Half4, UShortNorm2> Pos3fTbn4hTex2us;
    typedef BumpVertex<Float3, Half4, Half2> Pos3fTbn4hTex2h;
    typedef BumpVertex<Float3, Half4, Float2> Pos3fTbn4hTex2f;
    typedef BumpVertex<Float3, Float3, UShortNorm2> Pos3fTbn3fTex2us;
    typedef BumpVertex<Float3, Float3, Half2> Pos3fTbn3fTex2h;
    typedef BumpVertex<Float3, Float3, Float2> Pos3fTbn3fTex2f;
    typedef BumpVertex<Double3, ByteNorm4, UShortNorm2> Pos3dTbn4bTex2us;
    typedef BumpVertex<Double3, ByteNorm4, Half2> Pos3dTbn4bTex2h;
    typedef BumpVertex<Double3, ByteNorm4, Float2> Pos3dTbn4bTex2f;
    typedef BumpVertex<Double3, UByteNorm4, UShortNorm2> Pos3dTbn4ubTex2us;
    typedef BumpVertex<Double3, UByteNorm4, Half2> Pos3dTbn4ubTex2h;
    typedef BumpVertex<Double3, UByteNorm4, Float2> Pos3dTbn4ubTex2f;
    typedef BumpVertex<Double3, ShortNorm4, UShortNorm2> Pos3dTbn4sTex2us;
    typedef BumpVertex<Double3, ShortNorm4, Half2> Pos3dTbn4sTex2h;
    typedef BumpVertex<Double3, ShortNorm4, Float2> Pos3dTbn4sTex2f;
    typedef BumpVertex<Double3, UShortNorm4, UShortNorm2> Pos3dTbn4usTex2us;
    typedef BumpVertex<Double3, UShortNorm4, Half2> Pos3dTbn4usTex2h;
    typedef BumpVertex<Double3, UShortNorm4, Float2> Pos3dTbn4usTex2f;
    typedef BumpVertex<Double3, Half4, UShortNorm2> Pos3dTbn4hTex2us;
    typedef BumpVertex<Double3, Half4, Half2> Pos3dTbn4hTex2h;
    typedef BumpVertex<Double3, Half4, Float2> Pos3dTbn4hTex2f;
    typedef BumpVertex<Double3, Float3, UShortNorm2> Pos3dTbn3fTex2us;
    typedef BumpVertex<Double3, Float3, Half2> Pos3dTbn3fTex2h;
    typedef BumpVertex<Double3, Float3, Float2> Pos3dTbn3fTex2f;
} // namespace magma::vt
