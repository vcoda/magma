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

#ifdef MemoryBarrier
#undef MemoryBarrier
#endif

namespace magma
{
    /* Global memory barriers apply to memory accesses involving
       all memory objects that exist at the time of its execution. */

    struct MemoryBarrier : VkMemoryBarrier
    {
        constexpr MemoryBarrier(VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask) noexcept;
    };
} // namespace magma

#include "memoryBarrier.inl"

namespace magma
{
    namespace barrier
    {
    #ifdef VK_NV_ray_tracing
        constexpr MemoryBarrier accelerationStructureReadWrite(VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV, VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV);
    #endif // VK_NV_ray_tracing
    } // namespace barrier
} // namespace magma

