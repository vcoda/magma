/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
    namespace detail
    {
        template<typename T> inline T *copy(const void *const source) noexcept
        {
            MAGMA_ASSERT(source);
            if (!source)
                return nullptr;
            T *dest = new(std::nothrow) T();
            if (dest)
                memcpy(dest, source, sizeof(T));
            return dest;
        }

        template<typename T> inline T *copy(const T *const source) noexcept
        {
            MAGMA_ASSERT(source);
            if (source)
                return nullptr;
            T *dest = new(std::nothrow) T();
            if (dest)
                memcpy(dest, source, sizeof(T));
            return dest;
        }

        template<typename T> inline T *copy(T *const dest, const T *const source) noexcept
        {
            MAGMA_ASSERT(dest);
            MAGMA_ASSERT(source);
            MAGMA_ASSERT(dest != source);
            if (dest && source && dest != source)
                memcpy(dest, source, sizeof(T));
            return dest;
        }

        template<typename T> inline T *copyArray(const void *const source, std::size_t count) noexcept
        {
            MAGMA_ASSERT(source);
            MAGMA_ASSERT(count);
            if (!source || !count)
                return nullptr;
            T *dest = new(std::nothrow) T[count];
            if (dest)
                memcpy(dest, source, sizeof(T) * count);
            return dest;
        }

        template<typename T> inline T *copyArray(const T *const source, std::size_t count) noexcept
        {
            MAGMA_ASSERT(source);
            MAGMA_ASSERT(count > 0);
            if (!source || !count)
                return nullptr;
            T *dest = new(std::nothrow) T[count];
            if (dest)
                memcpy(dest, source, sizeof(T) * count);
            return dest;
        }

        template<typename T> inline T *copyVector(const std::vector<T>& source) noexcept
        {
            MAGMA_ASSERT(!source.empty());
            if (source.empty())
                return nullptr;
            T *dest = new(std::nothrow) T[source.size()];
            if (dest)
                memcpy(dest, source.data(), sizeof(T) * source.size());
            return dest;
        }

        template<typename X, typename T> inline X *copyVector(const std::vector<T>& source) noexcept
        {
            static_assert(sizeof(X) == sizeof(T), "type size mismatch");
            MAGMA_ASSERT(!source.empty());
            if (source.empty())
                return nullptr;
            X *dest = new(std::nothrow) X[source.size()];
            if (dest)
                memcpy(dest, source.data(), sizeof(T) * source.size());
            return dest;
        }

        template<typename T> inline T *copyInitializerList(const std::initializer_list<T>& source) noexcept
        {
            MAGMA_ASSERT(source.size() > 0);
            if (!source.size())
                return nullptr;
            T *dest = new(std::nothrow) T[source.size()];
            if (dest)
                memcpy(dest, source.begin(), sizeof(T) * source.size());
            return dest;
        }

        template<typename X, typename T> inline X *copyInitializerList(const std::initializer_list<T>& source) noexcept
        {
            static_assert(sizeof(X) == sizeof(T), "type size mismatch");
            MAGMA_ASSERT(source.size() > 0);
            if (!source.size())
                return nullptr;
            X *dest = new(std::nothrow) X[source.size()];
            if (dest)
                memcpy(dest, source.begin(), sizeof(T) * source.size());
            return dest;
        }

        inline char *copyString(const char *const source) noexcept
        {
            MAGMA_ASSERT(source);
            if (!source)
                return nullptr;
            const std::size_t size = strlen(source) + 1;
            char *const dest = new(std::nothrow) char[size];
#ifdef _MSC_VER
            const errno_t err = strcpy_s(dest, size, source);
            MAGMA_ASSERT(0 == err);
#else
            strcpy(dest, source);
#endif // _MSC_VER
            return dest;
        }
    } // namespace detail
} // namespace magma
