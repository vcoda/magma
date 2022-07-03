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
#pragma once

namespace magma
{
    /* Border color used when U(V,W) address mode is set to clamp-to-border. */

    class BorderColor
    {
    public:
        constexpr BorderColor(VkBorderColor color) noexcept;
        constexpr BorderColor(VkClearColorValue color,
            VkFormat format = VK_FORMAT_UNDEFINED) noexcept;
        constexpr VkBorderColor getColor() const noexcept { return color; }
        constexpr const VkClearColorValue& getCustomColor() const noexcept { return customColor; }
        constexpr VkFormat getFormat() const noexcept { return format; }
        constexpr bool custom() const noexcept;
        constexpr std::size_t hash() const noexcept;

    private:
        const VkBorderColor color;
        // VK_EXT_custom_border_color
        const VkClearColorValue customColor;
        const VkFormat format;
    };
} // namespace magma

#include "borderColor.inl"

namespace magma
{
    namespace border
    {
        constexpr BorderColor transparentBlackFloat(VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK);
        constexpr BorderColor transparentBlackInt(VK_BORDER_COLOR_INT_TRANSPARENT_BLACK);
        constexpr BorderColor opaqueBlackFloat(VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK);
        constexpr BorderColor opaqueBlackInt(VK_BORDER_COLOR_INT_OPAQUE_BLACK);
        constexpr BorderColor opaqueWhiteFloat(VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE);
        constexpr BorderColor opaqueWhiteInt(VK_BORDER_COLOR_INT_OPAQUE_WHITE);
    } // namespace border
} // namespace magma
