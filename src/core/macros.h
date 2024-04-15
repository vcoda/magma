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

#ifdef MAGMA_NO_EXCEPTIONS
    #define MAGMA_NEW new(std::nothrow)
#else
    #define MAGMA_NEW new
#endif

#define MAGMA_UNUSED(variable) variable

#define MAGMA_BOOLEAN(condition) (condition)\
    ? static_cast<VkBool32>(VK_TRUE)\
    : static_cast<VkBool32>(VK_FALSE)

#define MAGMA_COUNT(container) static_cast<uint32_t>((container).size())

#define MAGMA_ASSERT(expression) assert(expression)

#define MAGMA_FAILURE(message) MAGMA_ASSERT(!message)

#ifdef MAGMA_DEBUG
    #define MAGMA_ASSERT_FOR_EACH(arr, it, expr) for (auto const& it: arr) MAGMA_ASSERT(expr)
#else
    #define MAGMA_ASSERT_FOR_EACH(arr, it, expr)
#endif

#define MAGMA_CONCAT(a, b) a ## b

#define MAGMA_STRINGIZE(name) #name

#define MAGMA_STRINGIZE_ENUMERATION(identifier) case identifier: return MAGMA_STRINGIZE(identifier); // comma-ended

#define MAGMA_BITWISE_AND(flags, bits) ((flags & (bits)) == (bits))

#define MAGMA_ALIGN(size) (((size) + 0xF) & ~(0xF))

#define MAGMA_ALIGNED(p) (((uintptr_t)(const void *)(p)) % (MAGMA_ALIGNMENT) == 0)

#define MAGMA_HANDLE(obj) *(this->obj)

#define MAGMA_BUFFER_HANDLE(obj) core::reinterpret<VkBuffer>(obj)

#define MAGMA_IMAGE_HANDLE(obj) core::reinterpret<VkImage>(obj)

#if defined(VK_KHR_acceleration_structure)
    #define MAGMA_ACCELERATION_STRUCTURE_HANDLE(obj) core::reinterpret<VkAccelerationStructureKHR>(obj)
#elif defined(VK_NV_ray_tracing)
    #define MAGMA_ACCELERATION_STRUCTURE_HANDLE(obj) core::reinterpret<VkAccelerationStructureNV>(obj)
#endif

#define MAGMA_OPTIONAL_HANDLE(p) core::dereference(p)

#define MAGMA_OPTIONAL_INSTANCE(p) (this->p ? this->p.get() : nullptr)

#define MAGMA_SUCCEEDED(result)\
    ((VK_SUCCESS == result) ||\
     (VK_INCOMPLETE == result))

#define MAGMA_PRESENT_SUCCEEDED(result)\
    ((VK_SUCCESS == result) ||\
     (VK_SUBOPTIMAL_KHR == result))

#define MAGMA_STENCIL_FACE_MASK(frontFace, backFace)\
    ((frontFace && backFace) ? VK_STENCIL_FRONT_AND_BACK :\
     (frontFace ? VK_STENCIL_FACE_FRONT_BIT : VK_STENCIL_FACE_BACK_BIT))

#define MAGMA_TRY_CATCH(expression)\
    try {\
        expression;\
    }\
    catch (...) {\
    }

#define MAGMA_MAKE_SHARED(Type)\
    template<typename ...Args>\
    static std::shared_ptr<Type> makeShared(Args&& ...args)\
    {\
        struct Type##_ : Type\
        {\
            Type##_(Args&& ...args):\
                Type(std::forward<Args>(args)...) {}\
        };\
        return std::make_shared<Type##_>(std::forward<Args>(args)...);\
    }

#define MAGMA_TYPEDEF_SHARED_PTR(Type) typedef std::shared_ptr<class Type> Type##Ptr
#define MAGMA_TYPEDEF_SHARED_PTR_INTERFACE(Type) typedef std::shared_ptr<class I##Type> Type##Ptr

#define MAGMA_TYPEDEF_UNIQUE_PTR(Type) typedef std::unique_ptr<class Type> Type##Ptr
