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
    namespace vertexlayout
    {
        /* Vector types. */

        template<typename Type, int N>
        struct Vector
        {
            Type v[N];
        };

        typedef Vector<float, 2> Float2;
        typedef Vector<float, 3> Float3;
        typedef Vector<float, 4> Float4;
        typedef Vector<uint8_t, 4> Color;

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

        /* Typical vertex structures that are often used with basic primitive rasterization. */

        typedef Vertex<Float2, Float2> Pos2fTex2f;
        typedef Vertex<Float2, Float3> Pos2fColor3f;
        typedef Vertex<Float2, Float4> Pos2fColor4f;
        typedef Vertex<Float2, Color> Pos2fColor4b;

        typedef Vertex<Float3, Float2> Pos3fTex2f;
        typedef Vertex<Float3, Float3> Pos3fColor3f;
        typedef Vertex<Float3, Float4> Pos3fColor4f;
        typedef Vertex<Float3, Color> Pos3fColor4b;

        typedef Vertex<Float3, Float3> Pos3fNormal3f;

        typedef LitVertex<Float3, Float3, Float2> Pos3fNormal3fTex2f;
        typedef LitVertex<Float3, Float3, Float3> Pos3fNormal3fColor3f;
        typedef LitVertex<Float3, Float3, Float4> Pos3fNormal3fColor4f;
        typedef LitVertex<Float3, Float3, Color> Pos3fNormal3fColor4b;
    } // namespace vertexlayout
} // namespace magma
