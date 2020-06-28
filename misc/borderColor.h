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

namespace magma
{
    /* Border color used when U(V,W) address mode is set to clamp-to-border. */

    class BorderColor
    {
    public:
        bool isCustom() const noexcept;
        VkBorderColor getColor() const noexcept { return color; }
        const VkClearColorValue& getCustomColor() const noexcept { return customColor; }

    protected:
        BorderColor(VkBorderColor color) noexcept:
            color(color) {}
        BorderColor(VkClearColorValue color) noexcept:
            color(VK_BORDER_COLOR_MAX_ENUM), customColor(color) {}

    private:
        VkBorderColor color;
        VkClearColorValue customColor; // VK_EXT_custom_border_color
    };

    /* Predefined transparent border color. */

    template<typename Type>
    class TransparentBorderColor : public BorderColor
    {
    public:
        TransparentBorderColor() noexcept;
    };

    /* Predefined opaque border color. */

    template<typename Type, int>
    class OpaqueBorderColor : public BorderColor
    {
    public:
        OpaqueBorderColor() noexcept;
    };
} // namespace

#include "borderColor.inl"
