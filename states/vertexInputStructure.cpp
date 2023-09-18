/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "vertexInputStructure.h"

namespace magma
{
using namespace fvf;

namespace renderstate
{
const VertexInputStructure<int> nullVertexInput;

const VertexInputStructure<Pos2h> pos2h(0,
    {0, 0, VK_FORMAT_R16G16_SFLOAT, 0});
const VertexInputStructure<Pos2f> pos2f(0,
    {0, 0, VK_FORMAT_R32G32_SFLOAT, 0});
const VertexInputStructure<Pos2d> pos2d(0,
    {0, 0, VK_FORMAT_R64G64_SFLOAT, 0});
// 6-byte format usually not supported
//const VertexInputStructure<Pos3h> pos3h(0,
//    {0, 0, VK_FORMAT_R16G16B16_SFLOAT, 0});
const VertexInputStructure<Pos3f> pos3f(0,
    {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0});
const VertexInputStructure<Pos3d> pos3d(0,
    {0, 0, VK_FORMAT_R64G64B64_SFLOAT, 0});
const VertexInputStructure<Pos4h> pos4h(0,
    {0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 0});
const VertexInputStructure<Pos4f> pos4f(0,
    {0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0});
const VertexInputStructure<Pos4d> pos4d(0,
    {0, 0, VK_FORMAT_R64G64B64A64_SFLOAT, 0});

#define MAGMA_COLOR_VERTEX(Vertex, name)\
const VertexInputStructure<Vertex> name(0,\
    {\
        {0, &Vertex::pos},\
        {1, &Vertex::color}\
    });

MAGMA_COLOR_VERTEX(Pos2hColor4ub, pos2hColor4ub)
MAGMA_COLOR_VERTEX(Pos2fColor4ub, pos2fColor4ub)
MAGMA_COLOR_VERTEX(Pos2dColor4ub, pos2dColor4ub)
MAGMA_COLOR_VERTEX(Pos4hColor4ub, pos4hColor4ub)
MAGMA_COLOR_VERTEX(Pos3fColor4ub, pos3fColor4ub)
MAGMA_COLOR_VERTEX(Pos3dColor4ub, pos3dColor4ub)

#define MAGMA_TEX_VERTEX(Vertex, name)\
const VertexInputStructure<Vertex> name(0,\
    {\
        {0, &Vertex::pos},\
        {1, &Vertex::uv}\
    });

MAGMA_TEX_VERTEX(Pos2hTex2us, pos2hTex2us)
MAGMA_TEX_VERTEX(Pos2hTex2h, pos2hTex2h)
MAGMA_TEX_VERTEX(Pos2hTex2f, pos2hTex2f)
MAGMA_TEX_VERTEX(Pos2fTex2us, pos2fTex2us)
MAGMA_TEX_VERTEX(Pos2fTex2h, pos2fTex2h)
MAGMA_TEX_VERTEX(Pos2fTex2f, pos2fTex2f)
MAGMA_TEX_VERTEX(Pos2dTex2us, pos2dTex2us)
MAGMA_TEX_VERTEX(Pos2dTex2h, pos2dTex2h)
MAGMA_TEX_VERTEX(Pos2dTex2f, pos2dTex2f)
MAGMA_TEX_VERTEX(Pos4hTex2us, pos4hTex2us)
MAGMA_TEX_VERTEX(Pos4hTex2h, pos4hTex2h)
MAGMA_TEX_VERTEX(Pos4hTex2f, pos4hTex2f)
MAGMA_TEX_VERTEX(Pos3fTex2us, pos3fTex2us)
MAGMA_TEX_VERTEX(Pos3fTex2h, pos3fTex2h)
MAGMA_TEX_VERTEX(Pos3fTex2f, pos3fTex2f)
MAGMA_TEX_VERTEX(Pos3dTex2us, pos3dTex2us)
MAGMA_TEX_VERTEX(Pos3dTex2h, pos3dTex2h)
MAGMA_TEX_VERTEX(Pos3dTex2f, pos3dTex2f)

#define MAGMA_LIT_VERTEX(Vertex, name)\
const VertexInputStructure<Vertex> name(0,\
    {\
        {0, &Vertex::pos},\
        {1, &Vertex::normal}\
    });

MAGMA_LIT_VERTEX(Pos4hNormal4b, pos4hNormal4b)
MAGMA_LIT_VERTEX(Pos4hNormal4ub, pos4hNormal4ub)
MAGMA_LIT_VERTEX(Pos4hNormal4s, pos4hNormal4s)
MAGMA_LIT_VERTEX(Pos4hNormal4us, pos4hNormal4us)
MAGMA_LIT_VERTEX(Pos4hNormal4h, pos4hNormal4h)
MAGMA_LIT_VERTEX(Pos4hNormal3f, pos4hNormal3f)
MAGMA_LIT_VERTEX(Pos3fNormal4b, pos3fNormal4b)
MAGMA_LIT_VERTEX(Pos3fNormal4ub, pos3fNormal4ub)
MAGMA_LIT_VERTEX(Pos3fNormal4s, pos3fNormal4s)
MAGMA_LIT_VERTEX(Pos3fNormal4us, pos3fNormal4us)
MAGMA_LIT_VERTEX(Pos3fNormal4h, pos3fNormal4h)
MAGMA_LIT_VERTEX(Pos3fNormal3f, pos3fNormal3f)
MAGMA_LIT_VERTEX(Pos3dNormal4b, pos3dNormal4b)
MAGMA_LIT_VERTEX(Pos3dNormal4ub, pos3dNormal4ub)
MAGMA_LIT_VERTEX(Pos3dNormal4s, pos3dNormal4s)
MAGMA_LIT_VERTEX(Pos3dNormal4us, pos3dNormal4us)
MAGMA_LIT_VERTEX(Pos3dNormal4h, pos3dNormal4h)
MAGMA_LIT_VERTEX(Pos3dNormal3f, pos3dNormal3f)

#define MAGMA_LIT_TEX_VERTEX(Vertex, name)\
const VertexInputStructure<Vertex> name(0,\
    {\
        {0, &Vertex::pos},\
        {1, &Vertex::normal},\
        {2, &Vertex::uv}\
    });

MAGMA_LIT_TEX_VERTEX(Pos4hNormal4bTex2us, pos4hNormal4bTex2us)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4bTex2h, pos4hNormal4bTex2h)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4bTex2f, pos4hNormal4bTex2f)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4ubTex2us, pos4hNormal4ubTex2us)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4ubTex2h, pos4hNormal4ubTex2h)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4ubTex2f, pos4hNormal4ubTex2f)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4sTex2us, pos4hNormal4sTex2us)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4sTex2h, pos4hNormal4sTex2h)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4sTex2f, pos4hNormal4sTex2f)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4usTex2us, pos4hNormal4usTex2us)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4usTex2h, pos4hNormal4usTex2h)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4usTex2f, pos4hNormal4usTex2f)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4hTex2us, pos4hNormal4hTex2us)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4hTex2h, pos4hNormal4hTex2h)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal4hTex2f, pos4hNormal4hTex2f)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal3fTex2us, pos4hNormal3fTex2us)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal3fTex2h, pos4hNormal3fTex2h)
MAGMA_LIT_TEX_VERTEX(Pos4hNormal3fTex2f, pos4hNormal3fTex2f)

MAGMA_LIT_TEX_VERTEX(Pos3fNormal4bTex2us, pos3fNormal4bTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4bTex2h, pos3fNormal4bTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4bTex2f, pos3fNormal4bTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4ubTex2us, pos3fNormal4ubTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4ubTex2h, pos3fNormal4ubTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4ubTex2f, pos3fNormal4ubTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4sTex2us, pos3fNormal4sTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4sTex2h, pos3fNormal4sTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4sTex2f, pos3fNormal4sTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4usTex2us, pos3fNormal4usTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4usTex2h, pos3fNormal4usTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4usTex2f, pos3fNormal4usTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4hTex2us, pos3fNormal4hTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4hTex2h, pos3fNormal4hTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal4hTex2f, pos3fNormal4hTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal3fTex2us, pos3fNormal3fTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal3fTex2h, pos3fNormal3fTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3fNormal3fTex2f, pos3fNormal3fTex2f)

MAGMA_LIT_TEX_VERTEX(Pos3dNormal4bTex2us, pos3dNormal4bTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4bTex2h, pos3dNormal4bTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4bTex2f, pos3dNormal4bTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4ubTex2us, pos3dNormal4ubTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4ubTex2h, pos3dNormal4ubTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4ubTex2f, pos3dNormal4ubTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4sTex2us, pos3dNormal4sTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4sTex2h, pos3dNormal4sTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4sTex2f, pos3dNormal4sTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4usTex2us, pos3dNormal4usTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4usTex2h, pos3dNormal4usTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4usTex2f, pos3dNormal4usTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4hTex2us, pos3dNormal4hTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4hTex2h, pos3dNormal4hTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal4hTex2f, pos3dNormal4hTex2f)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal3fTex2us, pos3dNormal3fTex2us)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal3fTex2h, pos3dNormal3fTex2h)
MAGMA_LIT_TEX_VERTEX(Pos3dNormal3fTex2f, pos3dNormal3fTex2f)

#define MAGMA_BUMP_VERTEX(Vertex, name)\
const VertexInputStructure<Vertex> name(0,\
    {\
        {0, &Vertex::pos},\
        {1, &Vertex::normal},\
        {2, &Vertex::tangent},\
        {3, &Vertex::bitangent},\
        {4, &Vertex::uv}\
    });

MAGMA_BUMP_VERTEX(Pos4hTBN4bTex2h, pos4hTBN4bTex2h)
MAGMA_BUMP_VERTEX(Pos4hTBN4ubTex2h, pos4hTBN4ubTex2h)
MAGMA_BUMP_VERTEX(Pos4hTBN4sTex2h, pos4hTBN4sTex2h)
MAGMA_BUMP_VERTEX(Pos4hTBN4usTex2h, pos4hTBN4usTex2h)
MAGMA_BUMP_VERTEX(Pos4hTBN4bTex2f, pos4hTBN4bTex2f)
MAGMA_BUMP_VERTEX(Pos4hTBN4ubTex2f, pos4hTBN4ubTex2f)
MAGMA_BUMP_VERTEX(Pos4hTBN4sTex2f, pos4hTBN4sTex2f)
MAGMA_BUMP_VERTEX(Pos4hTBN4usTex2f, pos4hTBN4usTex2f)
MAGMA_BUMP_VERTEX(Pos3fTBN4bTex2h, pos3fTBN4bTex2h)
MAGMA_BUMP_VERTEX(Pos3fTBN4ubTex2h, pos3fTBN4ubTex2h)
MAGMA_BUMP_VERTEX(Pos3fTBN4sTex2h, pos3fTBN4sTex2h)
MAGMA_BUMP_VERTEX(Pos3fTBN4usTex2h, pos3fTBN4usTex2h)
MAGMA_BUMP_VERTEX(Pos3fTBN4bTex2f, pos3fTBN4bTex2f)
MAGMA_BUMP_VERTEX(Pos3fTBN4ubTex2f, pos3fTBN4ubTex2f)
MAGMA_BUMP_VERTEX(Pos3fTBN4sTex2f, pos3fTBN4sTex2f)
MAGMA_BUMP_VERTEX(Pos3fTBN4usTex2f, pos3fTBN4usTex2f)
MAGMA_BUMP_VERTEX(Pos3dTBN4bTex2h, pos3dTBN4bTex2h)
MAGMA_BUMP_VERTEX(Pos3dTBN4ubTex2h, pos3dTBN4ubTex2h)
MAGMA_BUMP_VERTEX(Pos3dTBN4sTex2h, pos3dTBN4sTex2h)
MAGMA_BUMP_VERTEX(Pos3dTBN4usTex2h, pos3dTBN4usTex2h)
MAGMA_BUMP_VERTEX(Pos3dTBN4bTex2f, pos3dTBN4bTex2f)
MAGMA_BUMP_VERTEX(Pos3dTBN4ubTex2f, pos3dTBN4ubTex2f)
MAGMA_BUMP_VERTEX(Pos3dTBN4sTex2f, pos3dTBN4sTex2f)
MAGMA_BUMP_VERTEX(Pos3dTBN4usTex2f, pos3dTBN4usTex2f)
} // namespace renderstate
} // namespace magma
