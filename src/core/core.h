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

#if !defined(VK_USE_64_BIT_PTR_DEFINES)
    #if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
        #define VK_USE_64_BIT_PTR_DEFINES 1
    #else
        #define VK_USE_64_BIT_PTR_DEFINES 0
    #endif
#endif

#if defined(_DEBUG) || defined(DEBUG)
    #define MAGMA_DEBUG
#endif

#if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
    #define MAGMA_NULL (VkDeviceAddress)0ull
#endif

#define MAGMA_ALIGNMENT 16
#define MAGMA_DEVICE_ADDRESS_ALIGNMENT 256

#define MAGMA_MAX_STRING 4096

#define MAGMA_MAX_BUFFER_MARKERS 1024

#define MAGMA_BAD_CODE 0xBAADC0DE

#define MAGMA_EXTENSION_PREFIX "VK_"
#define MAGMA_LAYER_PREFIX "VK_LAYER_"

#if defined(_M_AMD64) || defined(__x86_64__)
  #define MAGMA_XMM_REGISTERS 16
#else
  #define MAGMA_XMM_REGISTERS 8
#endif
#define MAGMA_XMM_BLOCK_SIZE (sizeof(__m128i) * MAGMA_XMM_REGISTERS)
#define MAGMA_CONCURRENT_COPY_THREADS 4
#define MAGMA_COPY_PAGE_SIZE (MAGMA_XMM_BLOCK_SIZE * MAGMA_CONCURRENT_COPY_THREADS)

#ifdef MAGMA_SSE
  #define mm_permute_ps(v, c) _mm_shuffle_ps((v), (v), c)
#endif

namespace magma
{
typedef std::vector<const char *> NullTerminatedStringArray;

#if (VK_USE_64_BIT_PTR_DEFINES == 1)
typedef void *NonDispatchableHandle;
#else
typedef uint64_t NonDispatchableHandle;
#endif

/* Base class with virtual destructor. */

class IClass
{
public:
    virtual ~IClass() = default;
};

/* Prevents objects of a class from being copy-
   constructed or assigned to each other. */

class NonCopyable
{
    NonCopyable(const NonCopyable&) = delete;
    const NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

template<class Parent, class Child>
inline void linkNode(Parent& parent, Child& child) noexcept
{
    child.pNext = const_cast<void *>(parent.pNext);
    parent.pNext = &child;
}

namespace core
{
template<class Type>
inline typename Type::NativeHandle dereference(const Type *obj) noexcept
{
    return obj ? obj->getHandle() : VK_NULL_HANDLE;
}

template<class Type>
inline typename Type::NativeHandle dereference(const std::shared_ptr<Type>& obj) noexcept
{
    return obj ? obj->getHandle() : VK_NULL_HANDLE;
}

template<class Type>
inline typename Type::NativeHandle dereference(const std::unique_ptr<Type>& obj) noexcept
{
    return obj ? obj->getHandle() : VK_NULL_HANDLE;
}

template<class Type>
inline typename Type::NativeHandle dereference(const std::weak_ptr<Type>& obj) noexcept
{
    return obj.expired() ? VK_NULL_HANDLE : obj.lock()->getHandle();
}

template<class VkObject>
inline VkObject reinterpret(NonDispatchableHandle handle) noexcept
{
    if (handle)
    {
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        return reinterpret_cast<VkObject>(handle);
    #else
        return static_cast<VkObject>(handle);
    #endif
    }
    return VK_NULL_HANDLE;
}

template<class T>
inline uint32_t countof(const T& container) noexcept
{
    return static_cast<uint32_t>(container.size());
}

template<class Type>
inline const std::unique_ptr<Type>& null() noexcept
{
    static std::unique_ptr<Type> ptr;
    return ptr;
}

template<class Int>
inline Int roundUp(Int value, Int multiple) noexcept
{
    if (Int(0) == multiple)
        return Int(0);
    if (Int(1) == multiple)
        return value;
    return ((value + multiple - Int(1)) / multiple) * multiple;
}

template<class T>
constexpr T alignUp(T value, T alignment) noexcept
{
    return (value + alignment - 1) & ~(alignment - 1);
}

template<class T>
constexpr T alignDown(T value, T alignment) noexcept
{
    return value & ~(alignment - 1);
}
} // namespace core
} // namespace magma

#include "macros.h"
#include "hash.h"
#include "memory.h"
#include "stackArray.h"
#include "lentPtr.h"
#include "variantPtr.h"

namespace magma::core
{
template<class Type>
inline typename Type::NativeHandle dereference(const lent_ptr<Type>& obj) noexcept
{
    return obj ? obj->getHandle() : VK_NULL_HANDLE;
}
} // namespace magma::core
