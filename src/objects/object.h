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
#include "iobject.h"
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
#include "objectType.h"
#endif

namespace magma
{
    class IAllocator;

    /* Base non-copyable template class of dispatchable and non-
       dispatchable objects. Stores native Vulkan handle and
       provides getObjectType() method. */

    template<class Type>
    class Object : public IObject,
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        public ObjectType<Type>, // Use custom template specialization
    #endif
        /* private */ NonCopyable
    {
    public:
        typedef Type NativeHandle;
        explicit Object(VkObjectType objectType,
            std::shared_ptr<IAllocator> allocator,
            Type handle = VK_NULL_HANDLE) noexcept;
        const std::shared_ptr<IAllocator>& getHostAllocator() const noexcept { return hostAllocator; }
        NativeHandle getHandle() const noexcept { return handle; }
        operator NativeHandle() const noexcept { return handle; }
        const NativeHandle *getHandleAddress() const noexcept { return &handle; }
        VkObjectType getObjectType() const noexcept override;

    protected:
        // Additional storage is required under x86 target as
        // Vulkan non-dispatchable handles are defined as uint64_t
        // and thus cannot be used with template specialization.
    #if (VK_USE_64_BIT_PTR_DEFINES == 0)
        const VkObjectType objectType;
    #endif
        NativeHandle handle;
        std::shared_ptr<IAllocator> hostAllocator;
    };
} // namespace magma

#include "object.inl"
