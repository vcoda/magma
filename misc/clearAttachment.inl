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
MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment) noexcept
{
    aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorAttachment = attachment;
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, float r, float g, float b, float a /* 1.f */) noexcept:
    ColorClearAttachment(attachment)
{
    clearValue.color.float32[0] = r;
    clearValue.color.float32[1] = g;
    clearValue.color.float32[2] = b;
    clearValue.color.float32[3] = a;
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, int32_t r, int32_t g, int32_t b, int32_t a) noexcept:
    ColorClearAttachment(attachment)
{
    clearValue.color.int32[0] = r;
    clearValue.color.int32[1] = g;
    clearValue.color.int32[2] = b;
    clearValue.color.int32[3] = a;
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept:
    ColorClearAttachment(attachment)
{
    clearValue.color.uint32[0] = r;
    clearValue.color.uint32[1] = g;
    clearValue.color.uint32[2] = b;
    clearValue.color.uint32[3] = a;
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const float color[4]) noexcept:
    ColorClearAttachment(attachment)
{
    for (int i = 0; i < 4; ++i)
        clearValue.color.float32[i] = color[i];
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const int32_t color[4]) noexcept:
    ColorClearAttachment(attachment)
{
    for (int i = 0; i < 4; ++i)
        clearValue.color.int32[i] = color[i];
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const uint32_t color[4]) noexcept:
    ColorClearAttachment(attachment)
{
    for (int i = 0; i < 4; ++i)
        clearValue.color.uint32[i] = color[i];
}

MAGMA_INLINE ColorClearAttachment::ColorClearAttachment(uint32_t attachment, const ClearColor& color) noexcept:
    ColorClearAttachment(attachment)
{
    clearValue = color;
}

MAGMA_INLINE DepthClearAttachment::DepthClearAttachment(float depth) noexcept
{
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    colorAttachment = 0;
    clearValue.depthStencil.depth = depth;
    clearValue.depthStencil.stencil = 0;
}

MAGMA_INLINE StencilClearAttachment::StencilClearAttachment(uint8_t stencil) noexcept
{
    aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    colorAttachment = 0;
    clearValue.depthStencil.depth = 0.f;
    clearValue.depthStencil.stencil = stencil;
}

MAGMA_INLINE DepthStencilClearAttachment::DepthStencilClearAttachment(float depth, uint8_t stencil) noexcept
{
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    colorAttachment = 0;
    clearValue.depthStencil.depth = depth;
    clearValue.depthStencil.stencil = stencil;
}

MAGMA_INLINE DepthStencilClearAttachment::DepthStencilClearAttachment(const ClearDepthStencil& depthStencil) noexcept
{
    aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    colorAttachment = 0;
    clearValue = depthStencil;
}
} // namespace magma