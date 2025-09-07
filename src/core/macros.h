/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

#define MAGMA_UNUSED(variable) ((void)(variable))

#define MAGMA_BOOLEAN(condition) (condition) ? VkBool32(VK_TRUE) : VkBool32(VK_FALSE)

#define MAGMA_ASSERT(expression) assert(expression)

#define MAGMA_FAILURE(message) MAGMA_ASSERT(!message)

#define MAGMA_CONCAT_IMPL(x, y) x##y
#define MAGMA_CONCAT(x, y) MAGMA_CONCAT_IMPL(x, y)

#define MAGMA_STRINGIZE(identifier) #identifier
#define MAGMA_STRINGIZE_ENUMERATOR(name) case name: return MAGMA_STRINGIZE(name)

#define MAGMA_BITWISE_AND(usage, flags) ((usage & (flags)) == (flags))

#define MAGMA_OPTIONAL(p) (this->p ? this->p.get() : nullptr)

#define MAGMA_OPTIONAL_HANDLE(p) core::dereference(p.get())

#define MAGMA_SUCCEEDED(result)\
    ((VK_SUCCESS == result) ||\
     (VK_INCOMPLETE == result))

#define MAGMA_PRESENT_SUCCEEDED(result)\
    ((VK_SUCCESS == result) ||\
     (VK_SUBOPTIMAL_KHR == result))

#define MAGMA_STENCIL_FACE_MASK(frontFace, backFace)\
    ((frontFace && backFace) ? VK_STENCIL_FRONT_AND_BACK :\
     (frontFace ? VK_STENCIL_FACE_FRONT_BIT : VK_STENCIL_FACE_BACK_BIT))

 #define MAGMA_DWORD_TO_FLOAT_RGBA(packed, r, g, b, a)\
    r = ((packed >> 24) & 0xFF) / float(0xFF);\
    g = ((packed >> 16) & 0xFF) / float(0xFF);\
    b = ((packed >> 8) & 0xFF) / float(0xFF);\
    a = (packed & 0xFF) / float(0xFF);

#if defined(MAGMA_DIAGNOSTIC) && defined(MAGMA_DEBUG)
    #define MAGMA_CHECKPOINT(stage) magma::CommandBuffer::insertDebugCheckpoint(__FUNCTION__, (VkPipelineStageFlagBits)stage)
#else
    #define MAGMA_CHECKPOINT(stage)
#endif

#ifdef MAGMA_STATS
    #define MAGMA_INCR(cnt, n) cnt += (uint16_t)n
#else
    #define MAGMA_INCR(cnt, n)
#endif // MAGMA_STATS

#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    #define MAGMA_INUSE(obj)\
    if (!(getUsage() & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))\
    {\
        if (obj.shareable()) try\
        {\
            inUse.push_back(obj.get_shared());\
        } catch(...) {}\
    }
#else
    #define MAGMA_INUSE(obj)
#endif // MAGMA_RETAIN_OBJECTS_IN_USE

#ifdef MAGMA_SSE
    #define mm_permute_ps(v, c) _mm_shuffle_ps((v), (v), c)
#endif

#define MAGMA_MAKE(Type, kind, method)\
template<typename ...Args>\
static std::kind##_ptr<Type> method(Args&& ...args)\
{\
    struct Type##_ final : Type\
    {\
        Type##_(Args&& ...args):\
            Type(std::forward<Args>(args)...) {}\
    };\
    return std::make_##kind<Type##_>(std::forward<Args>(args)...);\
}

#define MAGMA_MAKE_SHARED(Type) MAGMA_MAKE(Type, shared, makeShared)
#define MAGMA_MAKE_UNIQUE(Type) MAGMA_MAKE(Type, unique, makeUnique)

#define MAGMA_TYPEDEF_MANAGED_PTR(Type)\
typedef std::unique_ptr<class Type> Type##UPtr;\
typedef std::shared_ptr<class Type> Type##SPtr;
