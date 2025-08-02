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
#include <atomic>
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
        void lock() noexcept
        {
            while (flag.test_and_set(std::memory_order_acquire))
            { /* The pause intrinsic is used in spin-wait loops
                 with the processors implementing dynamic execution
                 (especially out-of-order execution). In the spin-
                 wait loop, the pause intrinsic improves the speed
                 at which the code detects the release of the lock
                 and provides especially significant performance gain.
                 The execution of the next instruction is delayed
                 for an implementation-specific amount of time. */
            #if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
                _mm_pause();
            #elif defined(_M_ARM) || defined(_M_ARM64)
                __yield();
            #elif defined(__arm__) || defined(__aarch64__)
             /* In a Symmetric Multi-Threading (SMT) design,
                a thread can use a Yield instruction to give a hint
                to the processor that it is running on. The Yield
                hint indicates that whatever the thread is currently
                doing is of low importance, and so could yield. For
                example, the thread might be sitting in a spin-lock. */
                #if defined(__ARM_ACLE)
                    __yield();
                #elif defined(__has_builtin)
                    #if __has_builtin(__builtin_arm_yield)
                        __builtin_arm_yield();
                    #else
                        __asm__ __volatile__("yield");
                    #endif
                #else
                    __asm__ __volatile__("yield");
                #endif
            #else // !x86-64 && !ARM
                std::this_thread::yield();
            #endif
            }
        }

        void unlock() noexcept
        {
            flag.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
} // namespace magma::core
