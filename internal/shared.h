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
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif
#include <memory>
#include <cassert>

#ifdef _MSC_VER
#define MAGMA_CONSTEXPR constexpr
#else
#define MAGMA_CONSTEXPR
#endif

#ifdef _DEBUG
#define MAGMA_DEBUG
#endif

#ifdef MAGMA_DEBUG
#ifndef MAGMA_ASSERT
#define MAGMA_ASSERT(condition) assert(condition)
#endif
#else
#define MAGMA_ASSERT(condition)
#endif // !MAGMA_DEBUG

#define MAGMA_BOOLEAN(condition) (condition) ? VK_TRUE : VK_FALSE
#define MAGMA_COUNT(container) static_cast<uint32_t>(container.size())

#define MAGMA_ALIGNMENT 16
#define MAGMA_ALIGN(size) (((size) + 0xF) & ~(0xF))
#define MAGMA_ALIGNED(p) (((uintptr_t)(const void *)(p)) % (MAGMA_ALIGNMENT) == 0)

#ifdef _MSC_VER
#define MAGMA_MALLOC(size) _mm_malloc(size, MAGMA_ALIGNMENT)
#define MAGMA_FREE(p) _mm_free(p)
#define MAGMA_ALLOCA(size) _malloca(size)
#define MAGMA_FREEA(p) _freea(p)
#else
#define MAGMA_MALLOC(size) malloc(size)
#define MAGMA_FREE(p) free(p)
#define MAGMA_ALLOCA(size) alloca(size)
#define MAGMA_FREEA(p)
#endif // !_MSC_VER

#define MAGMA_MAX_MESSAGE_STRING 4096

#if defined(_M_AMD64) || defined(__x86_64__)
#define MAGMA_XMM_REGISTERS 16
#else
#define MAGMA_XMM_REGISTERS 8
#endif

#define MAGMA_STRINGIZE(name) #name
#define MAGMA_STRINGIZE_FIELD(field) case field: return MAGMA_STRINGIZE(field); break
#define MAGMA_DEFAULT_UNKNOWN default: return "<unknown>"

#define MAGMA_SUCCEEDED(result)\
    ((VK_SUCCESS == result) ||\
     (VK_INCOMPLETE == result))

#define MAGMA_STENCIL_FACE_MASK(frontFace, backFace)\
    ((frontFace && backFace) ? VK_STENCIL_FRONT_AND_BACK :\
     (frontFace ? VK_STENCIL_FACE_FRONT_BIT : VK_STENCIL_FACE_BACK_BIT))

#define MAGMA_SPECIALIZE_VERTEX_ATTRIBUTE(Type, normalized, format)\
    template<>\
    struct VertexAttribute<Type, normalized> :\
        AttributeFormat<format> {};

namespace magma
{
    namespace internal
    {
        template<typename Type>
        inline typename Type::VkHandleType dereference(const std::shared_ptr<Type>& obj)
        {
            if (obj) return *obj;
#ifdef VK_NULL_HANDLE
            return VK_NULL_HANDLE;
#else
            return 0;
#endif
        }
    }
} // namespace magma

#define MAGMA_HANDLE(obj) *(this->obj)
#define MAGMA_OPTIONAL_HANDLE(obj) magma::internal::dereference(obj)
#define MAGMA_OPTIONAL_INSTANCE(obj) this->obj ? this->obj.get() : nullptr
