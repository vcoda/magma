/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
    #include <emmintrin.h>
#elif defined(__arm__) || defined(__aarch64__)
    #include <arm_acle.h>
#endif

namespace magma::core
{
    /* Fast spin-lock based mutex implementation.
       Used in scenarios where lock contention is expected
       to be low and the critical section is short.
       Instead of sleeping, threads actively wait (spin)
       until the lock becomes available. */

    class Spinlock
    {
    public:
        void lock() noexcept;
        void unlock() noexcept;

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
} // namespace magma::core

#include "spinlock.inl"
