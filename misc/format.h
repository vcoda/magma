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
    /* Utility class over VkFormat enumeration.
       Mostly used for internal purposes to determine
       format properties. */

    class Format
    {
    public:
        constexpr Format() noexcept;
        constexpr Format(VkFormat format) noexcept;
        constexpr operator VkFormat() const noexcept;
        constexpr bool valid() const noexcept;
        constexpr bool unorm() const noexcept;
        constexpr bool snorm() const noexcept;
        constexpr bool uscaled() const noexcept;
        constexpr bool sscaled() const noexcept;
        constexpr bool uint() const noexcept;
        constexpr bool sint() const noexcept;
        constexpr bool ufloat() const noexcept;
        constexpr bool sfloat() const noexcept;
        constexpr bool depth() const noexcept;
        constexpr bool stencil() const noexcept;
        constexpr bool depthStencil() const noexcept;
        constexpr bool alpha() const noexcept;
        constexpr bool srgb() const noexcept;
        constexpr bool bc() const noexcept;
        constexpr bool etc2() const noexcept;
        constexpr bool eac() const noexcept;
        constexpr bool astc() const noexcept;
        constexpr bool astcHdr() const noexcept;
        constexpr bool pvrtc() const noexcept;
        constexpr bool ycbcr() const noexcept;
        constexpr bool packed() const noexcept;
        bool blockCompressed() const noexcept;
        uint8_t componentCount() const noexcept;
        uint8_t planeCount() const noexcept;
        std::size_t size() const noexcept;
        std::size_t uncompressedSize() const noexcept;
        std::size_t bcSize() const noexcept;
        std::size_t etc2Size() const noexcept;
        std::size_t eacSize() const noexcept;
        std::size_t videoCompressedSize() const noexcept;
        std::pair<uint32_t, uint32_t> blockFootprint() const noexcept;
        VkFormat unormToSrgb() const noexcept;
        VkFormat srgbToUnorm() const noexcept;

    private:
        const VkFormat format;
    };
} // namespace magma

#include "format.inl"
