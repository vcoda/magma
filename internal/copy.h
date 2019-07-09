/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include <vector>
#include <cstring>
#include "shared.h"

namespace magma
{
    namespace internal
    {
        template<typename Type>
        inline Type *copy(const void *const src)
        {
            MAGMA_ASSERT(src);
            Type *dst = new Type();
            memcpy(dst, src, sizeof(Type));
            return dst;
        }

        template<typename Type>
        inline Type *copy(const Type *const src)
        {
            MAGMA_ASSERT(src);
            Type *dst = new Type();
            memcpy(dst, src, sizeof(Type));
            return dst;
        }

        template<typename Type>
        inline Type *copy(Type *const dst, const Type *const src)
        {
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(src);
            MAGMA_ASSERT(dst != src);
            memcpy(dst, src, sizeof(Type));
            return dst;
        }

        template<typename Type>
        inline Type *copyArray(const void *const src, size_t count)
        {
            MAGMA_ASSERT(src);
            MAGMA_ASSERT(count > 0);
            Type *dst = new Type[count];
            memcpy(dst, src, sizeof(Type) * count);
            return dst;
        }

        template<typename Type>
        inline Type *copyArray(const Type *const src, size_t count)
        {
            MAGMA_ASSERT(src);
            MAGMA_ASSERT(count > 0);
            Type *dst = new Type[count];
            memcpy(dst, src, sizeof(Type) * count);
            return dst;
        }

        template<typename Type>
        inline Type *copyVector(const std::vector<Type>& src)
        {
            MAGMA_ASSERT(src.size() > 0);
            Type *dst = new Type[src.size()];
            memcpy(dst, src.data(), sizeof(Type) * src.size());
            return dst;
        }

        template<typename DstType, typename SrcType>
        inline DstType *copyVector(const std::vector<SrcType>& src)
        {
            MAGMA_ASSERT(sizeof(DstType) == sizeof(SrcType));
            MAGMA_ASSERT(src.size() > 0);
            DstType *dst = new DstType[src.size()];
            memcpy(dst, src.data(), sizeof(SrcType) * src.size());
            return dst;
        }

        template<typename Type>
        inline Type *copyInitializerList(const std::initializer_list<Type>& src)
        {
            MAGMA_ASSERT(src.size() > 0);
            Type *dst = new Type[src.size()];
            memcpy(dst, src.begin(), sizeof(Type) * src.size());
            return dst;
        }

        inline char *copyString(const char *const src)
        {
            MAGMA_ASSERT(src);
            const size_t size = strlen(src) + 1;
            char *const dst = new char[size];
#ifdef _MSC_VER
            const errno_t err = strcpy_s(dst, size, src);
            MAGMA_ASSERT(0 == err);
#else
            strcpy(dst, src);
#endif // _MSC_VER
            return dst;
        }
    } // namespace internal
} // namespace magma
