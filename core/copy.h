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
#include <cstring>

namespace magma
{
namespace core
{
template<typename T> inline T *copy(const void *const p) noexcept
{
    MAGMA_ASSERT(p);
    if (!p) return nullptr;
    T *o = new(std::nothrow) T;
    if (o) memcpy(o, p, sizeof(T));
    return o;
}

template<typename T> inline T *copy(const T *const p) noexcept
{
    MAGMA_ASSERT(p);
    if (p) return nullptr;
    T *o = new(std::nothrow) T;
    if (o) memcpy(o, p, sizeof(T));
    return o;
}

template<typename T> inline T *copy(T *const dst, const T *const src) noexcept
{
    MAGMA_ASSERT(dst != src);
    if (dst && src && dst != src)
        memcpy(dst, src, sizeof(T));
    return dst;
}

inline void *copyBinaryData(const void *const src, const VkDeviceSize size) noexcept
{
    void *dst = new(std::nothrow) uint8_t[static_cast<std::size_t>(size)];
    if (dst)
        memcpy(dst, src, static_cast<std::size_t>(size));
    return dst;
}

template<typename T> inline T *copyArray(const void *const a, std::size_t n) noexcept
{
    MAGMA_ASSERT(n);
    if (!a || !n) return nullptr;
    T *o = new(std::nothrow) T[n];
    if (o) memcpy(o, a, sizeof(T) * n);
    return o;
}

template<typename T> inline T *copyArray(const T *const a, std::size_t n) noexcept
{
    MAGMA_ASSERT(n);
    if (!a || !n) return nullptr;
    T *o = new(std::nothrow) T[n];
    if (o) memcpy(o, a, sizeof(T) * n);
    return o;
}

template<typename T> inline T *copyVector(const std::vector<T>& v) noexcept
{
    MAGMA_ASSERT(!v.empty());
    if (v.empty()) return nullptr;
    const std::size_t n = v.size();
    T *o = new(std::nothrow) T[n];
    if (o) memcpy(o, v.data(), sizeof(T) * n);
    return o;
}

template<typename C, typename T> inline C *copyVector(const std::vector<T>& v) noexcept
{
    static_assert(sizeof(C) == sizeof(T), "type size mismatch");
    MAGMA_ASSERT(!v.empty());
    if (v.empty()) return nullptr;
    const std::size_t n = v.size();
    C *o = new(std::nothrow) C[n];
    if (o) memcpy(o, v.data(), sizeof(T) * n);
    return o;
}

template<typename T> inline T *copyInitializerList(const std::initializer_list<T>& ls) noexcept
{
    const std::size_t n = ls.size();
    MAGMA_ASSERT(n);
    if (0 == n) return nullptr;
    T *o = new(std::nothrow) T[n];
    if (o) memcpy(o, ls.begin(), sizeof(T) * n);
    return o;
}

template<typename C, typename T> inline C *copyInitializerList(const std::initializer_list<T>& ls) noexcept
{
    static_assert(sizeof(C) == sizeof(T), "type size mismatch");
    const std::size_t n = ls.size();
    MAGMA_ASSERT(n);
    if (0 == n) return nullptr;
    C *o = new(std::nothrow) C[n];
    if (o) memcpy(o, ls.begin(), sizeof(T) * n);
    return o;
}

inline char *copyString(const char *const s) noexcept
{
    MAGMA_ASSERT(s);
    if (!s) return nullptr;
    const std::size_t n = strlen(s) + 1;
    char *const o = new(std::nothrow) char[n];
    if (o)
    {
#ifdef _MSC_VER
        const errno_t err = strcpy_s(o, n, s);
        MAGMA_ASSERT(0 == err);
#else
        strcpy(o, s);
#endif // _MSC_VER
    }
    return o;
}
} // namespace core
} // namespace magma
