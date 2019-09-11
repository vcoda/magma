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
    namespace vertexdecl
    {
        template<typename Type, int N>
        struct Attribute
        {
            Type v[N];
        };

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

        template<int N>
        using Float = Attribute<float, N>;
        using Color = Attribute<uint8_t, 4>;

        /* Typical vertex structures that are often used with basic primitive rasterization. */

        typedef Vertex<Float<2>, Float<2>> Pos2fTex2f;
        typedef Vertex<Float<2>, Float<3>> Pos2fColor3f;
        typedef Vertex<Float<2>, Float<4>> Pos2fColor4f;
        typedef Vertex<Float<2>, Color> Pos2fColor4b;

        typedef Vertex<Float<3>, Float<2>> Pos3fTex2f;
        typedef Vertex<Float<3>, Float<3>> Pos3fColor3f;
        typedef Vertex<Float<3>, Float<4>> Pos3fColor4f;
        typedef Vertex<Float<3>, Color> Pos3fColor4b;
        typedef Vertex<Float<3>, Float<3>> Pos3fNormal3f;

        typedef LitVertex<Float<3>, Float<3>, Float<2>> Pos3fNormal3fTex2f;
        typedef LitVertex<Float<3>, Float<3>, Float<3>> Pos3fNormal3fColor3f;
        typedef LitVertex<Float<3>, Float<3>, Float<4>> Pos3fNormal3fColor4f;
        typedef LitVertex<Float<3>, Float<3>, Color> Pos3fNormal3fColor4b;
    } // namespace vertexdecl

    namespace specialization
    {
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(vertexdecl::Float<2>, false, VK_FORMAT_R32G32_SFLOAT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(vertexdecl::Float<3>, false, VK_FORMAT_R32G32B32_SFLOAT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(vertexdecl::Float<4>, false, VK_FORMAT_R32G32B32A32_SFLOAT)
        MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(vertexdecl::Color, true, VK_FORMAT_R8G8B8A8_UNORM)
    } // namespace specialization
} // namespace magma
