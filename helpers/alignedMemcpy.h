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
#include "../shared.h"

namespace magma
{
    namespace helpers
    {
        inline void *alignedMemcpy(void *dst, const void *src, size_t size)
        {
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(src);
            MAGMA_ASSERT(MAGMA_ALIGNED(dst));
            MAGMA_ASSERT(MAGMA_ALIGNED(src));
            return memcpy(dst, src, size);
        }
    } // namespace helpers
} // namespace magma
