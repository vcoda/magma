/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "clearValue.h"

namespace magma
{
    struct ClearAttachment : VkClearAttachment
    {
        constexpr ClearAttachment();
    };

    struct ColorClearAttachment : ClearAttachment
    {
        constexpr ColorClearAttachment(uint32_t attachment, float r, float g, float b, float a = 1.f);
        constexpr ColorClearAttachment(uint32_t attachment, int32_t r, int32_t g, int32_t b, int32_t a);
        constexpr ColorClearAttachment(uint32_t attachment, uint32_t r, uint32_t g, uint32_t b, uint32_t a);
        constexpr ColorClearAttachment(uint32_t attachment, const float color[4]);
        constexpr ColorClearAttachment(uint32_t attachment, const int32_t color[4]);
        constexpr ColorClearAttachment(uint32_t attachment, const uint32_t color[4]);
        constexpr ColorClearAttachment(uint32_t attachment, const ClearColor& color);

    private:
        constexpr ColorClearAttachment(uint32_t attachment);
    };

    struct DepthClearAttachment : ClearAttachment
    {
        constexpr DepthClearAttachment(float depth);
    };

    struct StencilClearAttachment : ClearAttachment
    {
        constexpr StencilClearAttachment(uint8_t stencil);
    };

    struct DepthStencilClearAttachment : ClearAttachment
    {
        constexpr DepthStencilClearAttachment(float depth, uint8_t stencil);
        constexpr DepthStencilClearAttachment(const ClearDepthStencil& depthStencil);
    };
} // namespace magma

#include "clearAttachment.inl"
