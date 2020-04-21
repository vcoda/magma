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

namespace magma
{
    class Format
    {
    public:
        constexpr Format(VkFormat format):
            format(format) {}
        constexpr operator VkFormat() const
            { return format; }
        constexpr bool valid() const
            { return format != VK_FORMAT_UNDEFINED; }
        constexpr bool depth() const;
        constexpr bool stencil() const;
        constexpr bool depthStencil() const;
        constexpr bool blockCompressed() const;
        constexpr bool ETC2() const;
        constexpr bool EAC() const;
        constexpr bool ASTC() const;
#ifdef VK_IMG_format_pvrtc
        constexpr bool PVRTC() const;
#endif
        constexpr bool compressed() const
            { return blockCompressed() || ETC2() || EAC() || ASTC() || PVRTC(); }
        constexpr bool floatingPoint() const;
        constexpr bool sRGB() const;
        constexpr std::size_t size() const;
        std::size_t blockCompressedSize() const noexcept;
        std::pair<int, int> blockFootprint() const noexcept;

    private:
        VkFormat format;
    };
} // namespace magma

#include "format.inl"
