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
#pragma once
#include <utility>
#include "../api/vulkan.h"

namespace magma
{
    class Format
    {
    public:
        constexpr Format(VkFormat format) noexcept:
            format(format) {}
        constexpr operator VkFormat() const noexcept
            { return format; }
        constexpr bool valid() const noexcept
            { return format != VK_FORMAT_UNDEFINED; }
        constexpr bool depth() const noexcept;
        constexpr bool stencil() const noexcept;
        constexpr bool depthStencil() const noexcept;
        constexpr bool blockCompressed() const noexcept;
        constexpr bool ETC2() const noexcept;
        constexpr bool EAC() const noexcept;
        constexpr bool ASTC() const noexcept;
        constexpr bool PVRTC() const noexcept;
        constexpr bool compressed() const noexcept
            { return blockCompressed() || ETC2() || EAC() || ASTC() || PVRTC(); }
        constexpr bool floatingPoint() const noexcept;
        constexpr bool sRGB() const noexcept;
        constexpr size_t size() const noexcept;
        size_t blockCompressedSize() const noexcept;
        std::pair<int, int> blockFootprint() const noexcept;

    private:
        VkFormat format;
    };
} // namespace magma

#include "format.inl"
