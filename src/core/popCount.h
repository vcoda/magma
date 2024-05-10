/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#if defined(_M_IX86) || defined(_M_X64)
    #if defined(MAGMA_SSE)
        #include <nmmintrin.h>
    #elif defined(_MSC_VER)
        #include <intrin.h>
    #endif
#elif defined(_M_ARM) && defined(MAGMA_NEON)
    #include <arm_neon.h>
#endif

namespace magma
{
    namespace core
    {
        int popCount(uint32_t value) noexcept;
    } // namespace core
} // namespace magma

#include "popCount.inl"
