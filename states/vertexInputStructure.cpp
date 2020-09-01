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
#include "pch.h"
#pragma hdrstop
#include "vertexInputStructure.h"

namespace magma
{
using namespace vertexlayout;

namespace renderstates
{
static constexpr VertexInputAttribute::Normalized<true> normalized;

const VertexInputStructure<int> nullVertexInput;

const VertexInputStructure<core::half2> pos2h(0,
    {0, 0, VK_FORMAT_R16G16_SFLOAT, 0});
const VertexInputStructure<core::half3> pos3h(0,
    {0, 0, VK_FORMAT_R16G16B16_SFLOAT, 0});
const VertexInputStructure<core::half4> pos4h(0,
    {0, 0, VK_FORMAT_R16G16B16A16_SFLOAT, 0});

const VertexInputStructure<Pos2hTex2h> pos2hTex2h(0, {
    {0, &Pos2hTex2h::position},
    {1, &Pos2hTex2h::attrib}});
const VertexInputStructure<Pos2hColor3h> pos2hColor3h(0, {
    {0, &Pos2hColor3h::position},
    {1, &Pos2hColor3h::attrib}});
const VertexInputStructure<Pos2hColor4h> pos2hColor4h(0, {
    {0, &Pos2hColor4h::position},
    {1, &Pos2hColor4h::attrib}});
const VertexInputStructure<Pos2hColor4b> pos2hColor4b(0, {
    {0, &Pos2hColor4b::position},
    {1, &Pos2hColor4b::attrib, normalized}});

const VertexInputStructure<Pos3hTex2h> pos3hTex2h(0, {
    {0, &Pos3hTex2h::position},
    {1, &Pos3hTex2h::attrib}});
const VertexInputStructure<Pos3hColor3h> pos3hColor3h(0, {
    {0, &Pos3hColor3h::position},
    {1, &Pos3hColor3h::attrib}});
const VertexInputStructure<Pos3hColor4h> pos3hColor4h(0, {
    {0, &Pos3hColor4h::position},
    {1, &Pos3hColor4h::attrib}});
const VertexInputStructure<Pos3hColor4b> pos3hColor4b(0, {
    {0, &Pos3hColor4b::position},
    {1, &Pos3hColor4b::attrib, normalized}});
const VertexInputStructure<Pos3hNormal3h> pos3hNormal3h(0, {
    {0, &Pos3hNormal3h::position},
    {1, &Pos3hNormal3h::attrib}});

const VertexInputStructure<Pos3hNormal3hTex2h> pos3hNormal3hTex2(0, {
    {0, &Pos3hNormal3hTex2h::position},
    {1, &Pos3hNormal3hTex2h::normal},
    {2, &Pos3hNormal3hTex2h::attrib}});
const VertexInputStructure<Pos3hNormal3hColor3h> pos3hNormal3hColor3h(0, {
    {0, &Pos3hNormal3hColor3h::position},
    {1, &Pos3hNormal3hColor3h::normal},
    {2, &Pos3hNormal3hColor3h::attrib}});
const VertexInputStructure<Pos3hNormal3hColor4h> pos3hNormal3hColor4h(0, {
    {0, &Pos3hNormal3hColor4h::position},
    {1, &Pos3hNormal3hColor4h::normal},
    {2, &Pos3hNormal3hColor4h::attrib}});
const VertexInputStructure<Pos3hNormal3hColor4b> pos3hNormal3hColor4b(0, {
    {0, &Pos3hNormal3hColor4b::position},
    {1, &Pos3hNormal3hColor4b::normal},
    {2, &Pos3hNormal3hColor4b::attrib, normalized}});

const VertexInputStructure<core::float2> pos2f(0,
    {0, 0, VK_FORMAT_R32G32_SFLOAT, 0});
const VertexInputStructure<core::float3> pos3f(0,
    {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0});
const VertexInputStructure<core::float4> pos4f(0,
    {0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0});

const VertexInputStructure<Pos2fTex2f> pos2fTex2f(0, {
    {0, &Pos2fTex2f::position},
    {1, &Pos2fTex2f::attrib}});
const VertexInputStructure<Pos2fColor3f> pos2fColor3f(0, {
    {0, &Pos2fColor3f::position},
    {1, &Pos2fColor3f::attrib}});
const VertexInputStructure<Pos2fColor4f> pos2fColor4f(0, {
    {0, &Pos2fColor4f::position},
    {1, &Pos2fColor4f::attrib}});
const VertexInputStructure<Pos2fColor4b> pos2fColor4b(0, {
    {0, &Pos2fColor4b::position},
    {1, &Pos2fColor4b::attrib, normalized}});

const VertexInputStructure<Pos3fTex2f> pos3fTex2f(0, {
    {0, &Pos3fTex2f::position},
    {1, &Pos3fTex2f::attrib}});
const VertexInputStructure<Pos3fColor3f> pos3fColor3f(0, {
    {0, &Pos3fColor3f::position},
    {1, &Pos3fColor3f::attrib}});
const VertexInputStructure<Pos3fColor4f> pos3fColor4f(0, {
    {0, &Pos3fColor4f::position},
    {1, &Pos3fColor4f::attrib}});
const VertexInputStructure<Pos3fColor4b> pos3fColor4b(0, {
    {0, &Pos3fColor4b::position},
    {1, &Pos3fColor4b::attrib, normalized}});
const VertexInputStructure<Pos3fNormal3f> pos3fNormal3f(0, {
    {0, &Pos3fNormal3f::position},
    {1, &Pos3fNormal3f::attrib}});

const VertexInputStructure<Pos3fNormal3fTex2f> pos3fNormal3fTex2(0, {
    {0, &Pos3fNormal3fTex2f::position},
    {1, &Pos3fNormal3fTex2f::normal},
    {2, &Pos3fNormal3fTex2f::attrib}});
const VertexInputStructure<Pos3fNormal3fColor3f> pos3fNormal3fColor3f(0, {
    {0, &Pos3fNormal3fColor3f::position},
    {1, &Pos3fNormal3fColor3f::normal},
    {2, &Pos3fNormal3fColor3f::attrib}});
const VertexInputStructure<Pos3fNormal3fColor4f> pos3fNormal3fColor4f(0, {
    {0, &Pos3fNormal3fColor4f::position},
    {1, &Pos3fNormal3fColor4f::normal},
    {2, &Pos3fNormal3fColor4f::attrib}});
const VertexInputStructure<Pos3fNormal3fColor4b> pos3fNormal3fColor4b(0, {
    {0, &Pos3fNormal3fColor4b::position},
    {1, &Pos3fNormal3fColor4b::normal},
    {2, &Pos3fNormal3fColor4b::attrib, normalized}});
} // namespace renderstates
} // namespace magma
