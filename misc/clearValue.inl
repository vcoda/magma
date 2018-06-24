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
MAGMA_INLINE ColorClear::ColorClear(float r, float g, float b, float a /* 1.f */) noexcept
{
    value.color.float32[0] = r;
    value.color.float32[1] = g;
    value.color.float32[2] = b;
    value.color.float32[3] = a;
}

MAGMA_INLINE ColorClear::ColorClear(int32_t r, int32_t g, int32_t b, int32_t a) noexcept
{
    value.color.int32[0] = r;
    value.color.int32[1] = g;
    value.color.int32[2] = b;
    value.color.int32[3] = a;
}

MAGMA_INLINE ColorClear::ColorClear(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept
{
    value.color.uint32[0] = r;
    value.color.uint32[1] = g;
    value.color.uint32[2] = b;
    value.color.uint32[3] = a;
}

MAGMA_INLINE ColorClear::ColorClear(const float color[4]) noexcept
{
    for (int i = 0; i < 4; ++i)
        value.color.float32[i] = color[i];
}

MAGMA_INLINE ColorClear::ColorClear(const int32_t color[4]) noexcept
{
    for (int i = 0; i < 4; ++i)
        value.color.int32[i] = color[i];
}

MAGMA_INLINE ColorClear::ColorClear(const uint32_t color[4]) noexcept
{
    for (int i = 0; i < 4; ++i)
        value.color.uint32[i] = color[i];
}

MAGMA_INLINE DepthStencilClear::DepthStencilClear(float depth, uint8_t stencil /* 0 */) noexcept
{
    value.depthStencil.depth = depth;
    value.depthStencil.stencil = stencil;
}
} // namespace magma