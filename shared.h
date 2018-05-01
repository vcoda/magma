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

#ifdef _DEBUG
#define MAGMA_DEBUG
#endif

#include <malloc.h>
#include <cassert>   
#include <memory>
#include <vector>
#ifdef MAGMA_DEBUG
#include <iostream> // std::cerr
#endif

#include "vulkan.h"
#include "misc/exception.h"

#define MAGMA_INLINE inline
#define MAGMA_BOOLEAN(x) x ? VK_TRUE : VK_FALSE
#define MAGMA_COUNT(v) static_cast<uint32_t>(v.size())

#ifdef _DEBUG
#   ifndef MAGMA_ASSERT
#   define MAGMA_ASSERT(x) assert(x)
#   endif
#else
#   define MAGMA_ASSERT(x)
#endif

#define MAGMA_STENCIL_FACE_MASK(frontFace, backFace)\
    ((frontFace && backFace) ? VK_STENCIL_FRONT_AND_BACK :\
     (frontFace ? VK_STENCIL_FACE_FRONT_BIT : VK_STENCIL_FACE_BACK_BIT))

#define MAGMA_STRINGIZE(x) #x
#define MAGMA_STRINGIZE_FIELD(x) case x: return MAGMA_STRINGIZE(x); break
#define MAGMA_UNKNOWN "<unknown>"

#ifdef _MSC_VER
#   define MAGMA_MALLOC(size) _mm_malloc(size, 16)
#   define MAGMA_FREE(p) _mm_free(p)
#   define MAGMA_ALLOCA(size) _malloca(size)
#   define MAGMA_FREEA(p) _freea(p)
#else
#   define MAGMA_MALLOC(size) malloc(size)
#   define MAGMA_FREE(p) free(p)
#   define MAGMA_ALLOCA(size) alloca(size)
#   define MAGMA_FREEA(p) 
#endif

#define MAGMA_ALIGN(size) ((size + 15) & ~(15)) 
#define MAGMA_ALIGNED(p) (((uintptr_t)(const void *)(p)) % (16) == 0)

#define MAGMA_MAX_STACK_ALLOC 1024

#define MAGMA_SUCCEEDED(result)\
    ((VK_SUCCESS == result) ||\
     (VK_INCOMPLETE == result))

#define MAGMA_THROW(message) throw Exception(message, __FILE__, __LINE__)
#define MAGMA_THROW_FAILURE(result, message)\
    if (!MAGMA_SUCCEEDED(result))\
	    throw BadResultException(result, message, __FILE__, __LINE__)
#define MAGMA_THROW_NOT_PRESENT(extension) throw ExtensionNotPresentException(extension, __FILE__, __LINE__)
#ifdef _MSC_VER
#   define MAGMA_THROW_NOT_IMPLEMENTED() throw NotImplementedException(__FUNCSIG__, __FILE__, __LINE__)
#else
#   define MAGMA_THROW_NOT_IMPLEMENTED() throw NotImplementedException(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#endif

#ifdef MAGMA_DEBUG
#   define MAGMA_REPORT_FAILURE(result, message)\
        if (!MAGMA_SUCCEEDED(result))\
            std::cerr<<__FILE__<<"("<<__LINE__<<"): "<<message<<"\n"
#else
#   define MAGMA_REPORT_FAILURE(result, message)
#endif

namespace magma
{
    template <typename Type>
    MAGMA_INLINE typename Type::VkType __handle(const std::shared_ptr<const Type>& obj)
    {
        //return obj ? *obj : VK_NULL_HANDLE;
        if (obj) return *obj;
        return VK_NULL_HANDLE;
    }
}

#define MAGMA_OPTIONAL_HANDLE(obj) magma::__handle(obj)
#define MAGMA_OPTIONAL_INSTANCE(obj) obj ? obj.get() : nullptr

#include "helpers/copying.h"
