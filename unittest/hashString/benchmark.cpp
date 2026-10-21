/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#if defined(_MSC_VER)
    #include <intrin.h>
#elif defined(__x86_64__) || defined(__i386__)
    #include <x86intrin.h>
#endif
#include <random>
#if defined(_M_X64) || defined(_M_IX86) || \
    defined(__x86_64__) || defined(__i386__)
    #define MAGMA_SSE
#elif defined(_M_ARM64) || defined(__aarch64__)
    #define MAGMA_NEON
#endif
#include "core/pch.h"

void barrier()
{
#if defined(_M_X64) || defined(_M_IX86) ||\
    defined(__x86_64__) || defined(__i386__)
    _mm_lfence();
#elif defined(_M_ARM64) || defined(__aarch64__)
    #ifdef _MSC_VER
        __isb(_ARM64_BARRIER_SY);
    #else
        __asm__ __volatile__("isb" ::: "memory");
    #endif
#endif
}

uint64_t timestamp()
{
#if defined(_M_X64) || defined(_M_IX86) || \
    defined(__x86_64__) || defined(__i386__)
    unsigned aux;
    return __rdtscp(&aux);
#elif defined(_M_ARM64) || defined(__aarch64__)
    #ifdef _MSC_VER
        return (uint64_t)_ReadStatusReg(ARM64_CNTVCT);
    #else
        uint64_t value;
        __asm__ __volatile__("mrs %0, cntvct_el0" : "=r"(value) :: "memory");
        return value;
    #endif
#else
    #error Unsupported architecture
#endif
}

std::string randomString(std::mt19937_64& rng)
{
    std::uniform_int_distribution<int> lenDist(16, 128);
    std::uniform_int_distribution<int> charDist(1, 127);
    int len = lenDist(rng);
    std::string str(len + 16, '\0');
    for (int i = 0; i < len; ++i)
        str[i] = (char)charDist(rng);
    return str;
}

template<class F>
inline uint64_t benchmark(const char *name, int count, F&& f)
{
    barrier();
    uint64_t begin = timestamp();
    barrier();
    {
        for (int i = 0; i < count; ++i)
            f(i);
    }
    barrier();
    uint64_t end = timestamp();
    barrier();
    uint64_t ticks = end - begin;
    std::cout << name << ": " << ticks << " ticks ("
        << (double(ticks) / count) << " ticks/op)" << std::endl;
    return ticks;
}

int main()
{
#ifdef MAGMA_X64
    std::cout << "64-bit build" << std::endl;
#else
    std::cout << "32-bit build" << std::endl;
#endif
    constexpr int N = 1'000'000;
    constexpr int M = 256;
    static_assert((M & (M - 1)) == 0, "M must be power of two");
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::string strings[M];
    for (int i = 0; i < M; ++i)
        strings[i] = randomString(rng);
    volatile magma::hash_t hash = 0;
    uint64_t ticks1 = benchmark("hash string", N,
        [&](int i)
        {
            hash += magma::core::hashString(strings[i & (M - 1)].c_str());
        });
    volatile magma::hash_t hashSimd = 0;
    uint64_t ticks2 = benchmark("hash string SIMD", N,
        [&](int i)
        {
            hashSimd += magma::core::hashStringSIMD(strings[i & (M - 1)].c_str());
        });
    std::cout << "speedup: " << double(ticks1) / double(ticks2) << std::endl;
    std::cout << "hash sums: " << std::endl << hash << std::endl << hashSimd << std::endl;
}
