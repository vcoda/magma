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
#include <memory>
#include "../api/vulkan.h"

namespace magma
{
    /* Global memory barriers apply to memory accesses involving
       all memory objects that exist at the time of its execution. */

    struct GlobalMemoryBarrier : VkMemoryBarrier
    {
        constexpr GlobalMemoryBarrier(VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask) noexcept
        {
            sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            pNext = nullptr;
            this->srcAccessMask = srcAccessMask;
            this->dstAccessMask = dstAccessMask;
        }
    };
} // namespace magma
