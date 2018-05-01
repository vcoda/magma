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
#include "../shared.h"

namespace magma
{
namespace helpers
{
MAGMA_INLINE void ImmediateRender::setVertexShader(const VertexShaderStage& shader)
{
    MAGMA_ASSERT(!insidePrimitive);
    vertexShader = shader;
}

MAGMA_INLINE void ImmediateRender::setFragmentShader(const FragmentShaderStage& shader)
{
    MAGMA_ASSERT(!insidePrimitive);
    fragmentShader = shader;
}

MAGMA_INLINE void ImmediateRender::setRasterizationState(const RasterizationState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    rasterizationState = state;
}

MAGMA_INLINE void ImmediateRender::setMultisampleState(const MultisampleState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    multisampleState = state;
}

MAGMA_INLINE void ImmediateRender::setDepthStencilState(const DepthStencilState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    depthStencilState = state;
}

MAGMA_INLINE void ImmediateRender::setColorBlendState(const ColorBlendState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    colorBlendState = state;
}

MAGMA_INLINE void ImmediateRender::setIdentity()
{
    MAGMA_ASSERT(!insidePrimitive);
    transform = Transform();
}

MAGMA_INLINE void ImmediateRender::setTransform(const float transform[16])
{
    MAGMA_ASSERT(!insidePrimitive);
    memcpy(this->transform.m, transform, sizeof(float) * 16);
}

MAGMA_INLINE void ImmediateRender::normal(float x, float y, float z)
{
    curr.nx = x;
    curr.ny = y;
    curr.nz = z;
}

MAGMA_INLINE void ImmediateRender::normal(const float n[3])
{
    normal(n[0], n[1], n[2]);
}

MAGMA_INLINE void ImmediateRender::color(float r, float g, float b, float a /* 1 */)
{
    curr.r = r;
    curr.g = g;
    curr.b = b;
    curr.a = a;
}

MAGMA_INLINE void ImmediateRender::color(const float c[4])
{
    color(c[0], c[1], c[2], c[3]);
}

MAGMA_INLINE void ImmediateRender::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a /* UCHAR_MAX */)
{
    color(r/255.f, g/255.f, b/255.f, a/255.f);
}

MAGMA_INLINE void ImmediateRender::color(const uint8_t c[4])
{
    color(c[0], c[1], c[2], c[3]);
}

MAGMA_INLINE void ImmediateRender::texCoord(float u, float v)
{
    curr.u = u;
    curr.v = v;
}

MAGMA_INLINE void ImmediateRender::texCoord(const float tc[2])
{
    texCoord(tc[0], tc[1]);
}

MAGMA_INLINE void ImmediateRender::pointSize(float size)
{
    MAGMA_ASSERT(size >= 1.f);
    curr.psize = size;
}

MAGMA_INLINE void ImmediateRender::vertex(float x, float y, float z /* 0 */, float w /* 1 */)
{
    MAGMA_ASSERT(insidePrimitive);
    MAGMA_ASSERT(vertexCount < maxVertexCount);
    if (insidePrimitive && vertexCount < maxVertexCount)
    {   // Position
        vert->x = x;
        vert->y = y;
        vert->z = z;
        vert->w = w;
        // Normal
        vert->nx = curr.nx;
        vert->ny = curr.ny;
        vert->nz = curr.nz;
        // Color
        vert->r = curr.r;
        vert->g = curr.g;
        vert->b = curr.b;
        vert->a = curr.a;
        // Texture coords
        vert->u = curr.u;
        vert->v = curr.v;
        // Point size
        vert->psize = curr.psize;
        // Go to the next vertex
        ++vert;
        ++primitives.back().vertexCount;
        ++vertexCount;
    }
}

MAGMA_INLINE void ImmediateRender::vertex(const float v[4])
{
    vertex(v[0], v[1], v[2], v[3]);
}

template<typename Vertex2>
MAGMA_INLINE void ImmediateRender::vertex2(const Vertex2& v)
{
    vertex(v[0], v[1]);
}

template<typename Vertex3>
MAGMA_INLINE void ImmediateRender::vertex3(const Vertex3& v)
{
    vertex(v[0], v[1], v[2]);
}

template<typename Vertex4>
MAGMA_INLINE void ImmediateRender::vertex4(const Vertex4& v)
{
    vertex(v[0], v[1], v[2], v[3]);
}
} // namespace helpers
} // namespace magma
