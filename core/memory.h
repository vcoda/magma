/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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

#define MAGMA_NEW new(std::nothrow)

namespace magma
{
namespace core
{
template<class Type>
inline Type *copy(const Type *src) noexcept
{
    MAGMA_ASSERT(src);
    if (!src)
        return nullptr;
    Type *dst = MAGMA_NEW Type();
    if (dst)
        memcpy(dst, src, sizeof(Type));
    return dst;
}

template<class Type>
inline void copy(Type *dst, const Type *src) noexcept
{
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(dst);
    MAGMA_ASSERT(dst != src);
    if (dst != src)
        memcpy(dst, src, sizeof(Type));
}

template<class Type>
inline Type *copyArray(const Type *src, std::size_t size) noexcept
{
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(size);
    if (!src || (0 == size))
        return nullptr;
    Type *dst = MAGMA_NEW Type[size];
    if (dst)
        memcpy(dst, src, sizeof(Type) * size);
    return dst;
}

template<class Type>
inline Type *copyVector(const std::vector<Type>& src) noexcept
{
    MAGMA_ASSERT(!src.empty());
    if (src.empty())
        return nullptr;
    const std::size_t size = src.size();
    Type *dst = MAGMA_NEW Type[size];
    if (dst)
        memcpy(dst, src.data(), sizeof(Type) * size);
    return dst;
}

template<class Dst, class Src>
inline Dst *copyVector(const std::vector<Src>& src) noexcept
{
    static_assert(sizeof(Dst) == sizeof(Src), "type size mismatch");
    if (src.empty())
        return nullptr;
    const std::size_t size = src.size();
    Dst *dst = MAGMA_NEW Dst[size];
    if (dst)
        memcpy(dst, src.data(), sizeof(Src) * size);
    return dst;
}

template<class Type>
inline Type *copyInitializerList(const std::initializer_list<Type>& src) noexcept
{
    const std::size_t size = src.size();
    MAGMA_ASSERT(size);
    if (0 == size)
        return nullptr;
    Type *dst = MAGMA_NEW Type[size];
    if (dst)
        memcpy(dst, src.begin(), sizeof(Type) * size);
    return dst;
}

template<class Dst, class Src>
inline Dst *copyInitializerList(const std::initializer_list<Src>& src) noexcept
{
    static_assert(sizeof(Dst) == sizeof(Src), "type size mismatch");
    const std::size_t size = src.size();
    MAGMA_ASSERT(size);
    if (0 == size)
        return nullptr;
    Dst *dst = MAGMA_NEW Dst[size];
    if (dst)
        memcpy(dst, src.begin(), sizeof(Src) * size);
    return dst;
}

inline char *copyString(const char *src) noexcept
{
    MAGMA_ASSERT(src);
    if (!src)
        return nullptr;
    const std::size_t count = strlen(src) + 1;
    char *dst = MAGMA_NEW char[count];
    if (dst)
    {
    #ifdef _MSC_VER
        const errno_t err = strcpy_s(dst, count, src);
        MAGMA_ASSERT(0 == err);
    #else
        strcpy(dst, src);
    #endif // _MSC_VER
    }
    return dst;
}

inline void *copyBinaryData(const void *src, std::size_t size) noexcept
{
    void *dst = MAGMA_NEW char[size];
    if (dst)
        memcpy(dst, src, size);
    return dst;
}

template<class Type>
inline bool compare(const void *p1, const void *p2) noexcept
{
    if (p1 == p2)
        return true;
    if (!p1 || !p2)
        return false;
    return !memcmp(p1, p2, sizeof(Type));
}

template<class Type>
inline bool compare(const Type *p1, const Type *p2) noexcept
{
    if (p1 == p2)
        return true;
    if (!p1 || !p2)
        return false;
    return !memcmp(p1, p2, sizeof(Type));
}

template<class Type>
inline bool compareArrays(const Type *p1, const Type *p2, std::size_t count) noexcept
{
    if (p1 == p2)
        return true;
    if (!p1 || !p2)
        return false;
    return !memcmp(p1, p2, sizeof(Type) * count);
}

template<class Type>
inline void zeroMemory(Type& var) noexcept
{
    memset(&var, 0, sizeof(Type));
}

template<class Type, std::size_t Size>
inline void zeroMemory(Type (&array)[Size]) noexcept
{
    memset(array, 0, sizeof(Type) * Size);
}
} // core
} // magma
