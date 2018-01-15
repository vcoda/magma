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
#include "../vulkan.h"

namespace magma
{
    class Format
    {
    public:
        Format(VkFormat format);
        bool valid() const { return format != VK_FORMAT_UNDEFINED; }
        bool depth() const;
        bool depthStencil() const;
        bool blockCompressed() const;
        bool etc() const;
        bool adaptiveCompressed() const;
        bool compressed() const 
        {
            return blockCompressed() || etc() || adaptiveCompressed();
        }
        bool floatingPoint() const;
        std::pair<uint8_t, uint8_t> blockFootprint() const;
        operator VkFormat() const { return format; }

    private:
        VkFormat format;
    };
} // namespace magma
