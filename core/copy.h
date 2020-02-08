/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
template<typename Type>
inline Type *copy(const void *const src) noexcept
{
    MAGMA_ASSERT(src);
    if (!src)
        return nullptr;
    Type *dst = new(std::nothrow) Type();
    if (dst)
        memcpy(dst, src, sizeof(Type));
    return dst;
}

template<typename Type>
inline Type *copy(const Type *const src) noexcept
{
    MAGMA_ASSERT(src);
    if (src)
        return nullptr;
    Type *dst = new(std::nothrow) Type();
    if (dst)
        memcpy(dst, src, sizeof(Type));
    return dst;
}

template<typename Type>
inline Type *copy(Type *const dst, const Type *const src) noexcept
{
    MAGMA_ASSERT(dst);
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(dst != src);
    if (dst && src && dst != src)
        memcpy(dst, src, sizeof(Type));
    return dst;
}

template<typename Type>
inline Type *copyArray(const void *const src, std::size_t count) noexcept
{
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(count);
    if (!src || !count)
        return nullptr;
    Type *dst = new(std::nothrow) Type[count];
    if (dst)
        memcpy(dst, src, sizeof(Type) * count);
    return dst;
}

template<typename Type>
inline Type *copyArray(const Type *const src, std::size_t count) noexcept
{
    MAGMA_ASSERT(src);
    MAGMA_ASSERT(count > 0);
    if (!src || !count)
        return nullptr;
    Type *dst = new(std::nothrow) Type[count];
    if (dst)
        memcpy(dst, src, sizeof(Type) * count);
    return dst;
}

template<typename Type>
inline Type *copyVector(const std::vector<Type>& src) noexcept
{
    MAGMA_ASSERT(!src.empty());
    if (src.empty())
        return nullptr;
    Type *dst = new(std::nothrow) Type[src.size()];
    if (dst)
        memcpy(dst, src.data(), sizeof(Type) * src.size());
    return dst;
}

template<typename Out, typename In>
inline Out *castCopyVector(const std::vector<In>& src) noexcept
{
    static_assert(sizeof(Out) == sizeof(In), "type size mismatch");
    MAGMA_ASSERT(!src.empty());
    if (src.empty())
        return nullptr;
    Out *dst = new(std::nothrow) Out[src.size()];
    if (dst)
        memcpy(dst, src.data(), sizeof(In) * src.size());
    return dst;
}

template<typename Type> inline
Type *copyInitializerList(const std::initializer_list<Type>& src) noexcept
{
    MAGMA_ASSERT(src.size() > 0);
    if (!src.size())
        return nullptr;
    Type *dst = new(std::nothrow) Type[src.size()];
    if (dst)
        memcpy(dst, src.begin(), sizeof(Type) * src.size());
    return dst;
}

template<typename X, typename Type>
inline X *copyInitializerList(const std::initializer_list<Type>& src) noexcept
{
    static_assert(sizeof(X) == sizeof(Type), "type size mismatch");
    MAGMA_ASSERT(src.size() > 0);
    if (!src.size())
        return nullptr;
    X *dst = new(std::nothrow) X[src.size()];
    if (dst)
        memcpy(dst, src.begin(), sizeof(Type) * src.size());
    return dst;
}

inline char *copyString(const char *const src) noexcept
{
    MAGMA_ASSERT(src);
    if (!src)
        return nullptr;
    const std::size_t size = strlen(src) + 1;
    char *const dst = new(std::nothrow) char[size];
    if (!dst)
        return nullptr;
#ifdef _MSC_VER
    const errno_t err = strcpy_s(dst, size, src);
    MAGMA_ASSERT(0 == err);
#else
    strcpy(dst, src);
#endif // _MSC_VER
    return dst;
}
} // namespace core
} // namespace magma
