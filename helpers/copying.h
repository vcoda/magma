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
#include <cstring>
#include "../shared.h"

namespace magma
{
    namespace helpers
    {
        template <typename Type>
        MAGMA_INLINE Type *copy(Type *const dst, const Type *const src)
        {
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(src);
            return reinterpret_cast<Type *>(memcpy(dst, src, sizeof(Type)));
        }

        template <typename Type>
        MAGMA_INLINE Type *copy(Type *const dst, const Type *const src, uint32_t count)
        {
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(src);
            MAGMA_ASSERT(count);
            return reinterpret_cast<Type *>(memcpy(dst, src, sizeof(Type) * count));
        }

        template <typename DstType, typename SrcType>
        MAGMA_INLINE DstType *copy(DstType *const dst, const std::vector<SrcType>& src)
        {
            static_assert(sizeof(DstType) == sizeof(SrcType), "equal size expected");
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(!src.empty());
            return reinterpret_cast<DstType *>(memcpy(dst, src.data(), sizeof(SrcType) * src.size()));
        }

        template <typename DstType, typename SrcType>
        MAGMA_INLINE DstType *copy(DstType *const dst, const std::initializer_list<SrcType>& src)
        {
            static_assert(sizeof(DstType) == sizeof(SrcType), "equal size expected");
            MAGMA_ASSERT(dst);
            MAGMA_ASSERT(src.size() > 0);
            return reinterpret_cast<DstType *>(memcpy(dst, src.begin(), sizeof(SrcType) * src.size()));
        }

        MAGMA_INLINE char *copyString(const char *const src)
        {
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
    } // namespace helpers
} // namespace magma
