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
#pragma once
#include "clearValue.h"

namespace magma
{
    struct ClearAttachment : VkClearAttachment
    {
        constexpr ClearAttachment() noexcept;
        constexpr ClearAttachment(VkImageAspectFlags aspectMask, uint32_t attachment) noexcept;
    };

    struct ColorClearAttachment : ClearAttachment
    {
        constexpr ColorClearAttachment(uint32_t attachment, float r, float g, float b, float a = 1.f) noexcept;
        constexpr ColorClearAttachment(uint32_t attachment, int32_t r, int32_t g, int32_t b, int32_t a) noexcept;
        constexpr ColorClearAttachment(uint32_t attachment, uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept;
        constexpr ColorClearAttachment(uint32_t attachment, const float color[4]) noexcept;
        constexpr ColorClearAttachment(uint32_t attachment, const int32_t color[4]) noexcept;
        constexpr ColorClearAttachment(uint32_t attachment, const uint32_t color[4]) noexcept;
        constexpr ColorClearAttachment(uint32_t attachment, const ClearColor& color) noexcept;
    };

    struct DepthClearAttachment : ClearAttachment
    {
        constexpr DepthClearAttachment(float depth) noexcept;
    };

    struct StencilClearAttachment : ClearAttachment
    {
        constexpr StencilClearAttachment(uint8_t stencil) noexcept;
    };

    struct DepthStencilClearAttachment : ClearAttachment
    {
        constexpr DepthStencilClearAttachment(float depth, uint8_t stencil) noexcept;
        constexpr DepthStencilClearAttachment(const ClearDepthStencil& depthStencil) noexcept;
    };
} // namespace magma

#include "clearAttachment.inl"
