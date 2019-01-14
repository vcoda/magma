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
        Format(VkFormat format) noexcept;
        bool valid() const noexcept
            { return format != VK_FORMAT_UNDEFINED; }
        bool depth() const noexcept;
        bool depthStencil() const noexcept;
        bool blockCompressed() const noexcept;
        bool ETC2() const noexcept;
        bool EAC() const noexcept;
        bool ASTC() const noexcept;
        bool PVRTC() const noexcept;
        bool compressed() const noexcept
            { return blockCompressed() || ETC2() || EAC() || ASTC() || PVRTC(); }
        bool floatingPoint() const noexcept;
        std::pair<int, int> blockFootprint() const noexcept;
        operator VkFormat() const noexcept
            { return format; }

    private:
        VkFormat format;
    };
} // namespace magma
