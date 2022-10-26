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
    /* Utility wrapper over VkFormat enumeration.
       Mostly used for internal purposes to get format properties. */

    class Format
    {
    public:
        constexpr Format() noexcept;
        constexpr Format(VkFormat format) noexcept;
        constexpr operator VkFormat() const noexcept;
        constexpr bool valid() const noexcept;
        constexpr bool depth() const noexcept;
        constexpr bool stencil() const noexcept;
        constexpr bool depthStencil() const noexcept;
        constexpr bool bc() const noexcept;
        constexpr bool etc2() const noexcept;
        constexpr bool eac() const noexcept;
        constexpr bool astc() const noexcept;
        constexpr bool pvrtc() const noexcept;
        constexpr bool srgb() const noexcept;
        constexpr bool ycbcr() const noexcept;
        constexpr bool blockCompressed() const noexcept;
        constexpr bool floatingPoint() const noexcept;
        uint8_t componentCount() const noexcept;
        uint8_t planeCount() const noexcept;
        std::size_t size() const noexcept;
        std::size_t nonCompressedSize() const noexcept;
        std::size_t blockCompressedSize() const noexcept;
        std::size_t videoCompressedSize() const noexcept;
        std::pair<int, int> blockFootprint() const noexcept;
        VkFormat unormToSrgb() const noexcept;
        VkFormat srgbToUnorm() const noexcept;

    private:
        const VkFormat format;
    };
} // namespace magma

#include "format.inl"
