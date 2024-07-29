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

namespace magma
{
namespace core
{
template<class Type>
inline Type *copy(const Type *src) noexcept
{
    Type *dst = nullptr;
    if (src)
    {
        dst = MAGMA_NEW Type();
        if (dst)
            memcpy(dst, src, sizeof(Type));
    }
    return dst;
}

template<class Type>
inline void copy(Type *dst, const Type *src) noexcept
{
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(dst);
    MAGMA_ASSERT(dst != src);
    if (dst && dst != src)
        memcpy(dst, src, sizeof(Type));
}

template<class Type>
inline Type *copyArray(const Type *src, std::size_t size) noexcept
{
    Type *dst = nullptr;
    if (src && size)
    {
        dst = MAGMA_NEW Type[size];
        if (dst)
            memcpy(dst, src, sizeof(Type) * size);
    }
    return dst;
}

template<class Type>
inline Type *copyVector(const std::vector<Type>& src) noexcept
{
    Type *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW Type[size];
        if (dst)
            memcpy(dst, src.data(), sizeof(Type) * size);
    }
    return dst;
}

template<class DstType, class SrcType>
inline DstType *copyVector(const std::vector<SrcType>& src) noexcept
{
    static_assert(sizeof(DstType) == sizeof(SrcType), "type size mismatch");
    DstType *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW DstType[size];
        if (dst)
            memcpy(dst, src.data(), sizeof(SrcType) * size);
    }
    return dst;
}

template<class Type>
inline Type *copyInitializerList(const std::initializer_list<Type>& src) noexcept
{
    Type *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW Type[size];
        if (dst)
        {
            Type *elem = dst;
            for (const Type& value: src)
                *elem++ = value;
        }
    }
    return dst;
}

template<class DstType, class SrcType>
inline DstType *copyInitializerList(const std::initializer_list<SrcType>& src) noexcept
{
    static_assert(sizeof(DstType) == sizeof(SrcType), "type size mismatch");
    DstType *dst = nullptr;
    if (std::size_t size = src.size())
    {
        dst = MAGMA_NEW DstType[size];
        if (dst)
        {
            DstType *elem = dst;
            for (const SrcType& value: src)
                memcpy(elem++, &value, sizeof(SrcType));
        }
    }
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
    #if defined(_MSC_VER) || defined(__MINGW32__)
        const errno_t err = strcpy_s(dst, count, src);
        MAGMA_ASSERT(0 == err);
    #else
        strcpy(dst, src);
    #endif // _MSC_VER || __MINGW32__
    }
    return dst;
}

inline uint8_t *copyBinaryData(const void *src, std::size_t size) noexcept
{
    uint8_t *dst = nullptr;
    if (src && size)
    {
        dst = MAGMA_NEW uint8_t[size];
        if (dst)
            memcpy(dst, src, size);
    }
    return dst;
}

template<class Type>
inline uint8_t *copyBinaryData(const Type& src) noexcept
{
    return copyBinaryData(&src, sizeof(Type));
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
