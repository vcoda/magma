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
namespace magma
{
namespace helpers
{
inline void ImmediateRender::setVertexShader(const VertexShaderStage& shader)
{
    MAGMA_ASSERT(!insidePrimitive);
    vertexShader = shader;
}

inline void ImmediateRender::setFragmentShader(const FragmentShaderStage& shader)
{
    MAGMA_ASSERT(!insidePrimitive);
    fragmentShader = shader;
}

inline void ImmediateRender::setRasterizationState(const RasterizationState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    rasterizationState = state;
}

inline void ImmediateRender::setMultisampleState(const MultisampleState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    multisampleState = state;
}

inline void ImmediateRender::setDepthStencilState(const DepthStencilState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    depthStencilState = state;
}

inline void ImmediateRender::setColorBlendState(const ColorBlendState& state)
{
    MAGMA_ASSERT(!insidePrimitive);
    colorBlendState = state;
}

inline void ImmediateRender::setIdentity()
{
    MAGMA_ASSERT(!insidePrimitive);
    transform = Transform();
}

inline void ImmediateRender::setTransform(const float transform[16])
{
    MAGMA_ASSERT(!insidePrimitive);
    memcpy(this->transform.m, transform, sizeof(float) * 16);
}

inline void ImmediateRender::normal(float x, float y, float z)
{
    curr.nx = x;
    curr.ny = y;
    curr.nz = z;
}

inline void ImmediateRender::normal(const float n[3])
{
    normal(n[0], n[1], n[2]);
}

inline void ImmediateRender::color(float r, float g, float b, float a /* 1 */)
{
    curr.r = r;
    curr.g = g;
    curr.b = b;
    curr.a = a;
}

inline void ImmediateRender::color(const float c[4])
{
    color(c[0], c[1], c[2], c[3]);
}

inline void ImmediateRender::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a /* 255 */)
{
    color(r/255.f, g/255.f, b/255.f, a/255.f);
}

inline void ImmediateRender::color(const uint8_t c[4])
{
    color(c[0], c[1], c[2], c[3]);
}

inline void ImmediateRender::texCoord(float u, float v)
{
    curr.u = u;
    curr.v = v;
}

inline void ImmediateRender::texCoord(const float tc[2])
{
    texCoord(tc[0], tc[1]);
}

inline void ImmediateRender::pointSize(float size)
{
    MAGMA_ASSERT(size >= 1.f);
    curr.psize = size;
}

inline void ImmediateRender::vertex(float x, float y, float z /* 0 */, float w /* 1 */)
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

inline void ImmediateRender::vertex(const float v[4])
{
    vertex(v[0], v[1], v[2], v[3]);
}

template<typename Vertex2>
inline void ImmediateRender::vertex2(const Vertex2& v)
{
    vertex(v[0], v[1]);
}

template<typename Vertex3>
inline void ImmediateRender::vertex3(const Vertex3& v)
{
    vertex(v[0], v[1], v[2]);
}

template<typename Vertex4>
inline void ImmediateRender::vertex4(const Vertex4& v)
{
    vertex(v[0], v[1], v[2], v[3]);
}
} // namespace helpers
} // namespace magma
