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
#include "iobject.h"
#include "objectType.h"
#include "../../misc/structureChain.h"

namespace magma
{
    class IAllocator;
    class StructureChain;

    /* Base non-copyable class of dispatchable and non-
       dispatchable objects. Stores native Vulkan handle,
       VkObjectType and allocator used to create object.
       Additional storage is required under x86 target
       as Vulkan non-dispatchable handles are all defined
       as single uint64_t type and thus cannot be
       distinguished with template specialization. */

    template<class Type>
    class Object : NonCopyable
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        ,public ObjectType<Type>
    #endif
    {
    public:
        typedef Type NativeHandle;
        NativeHandle getHandle() const noexcept { return handle; }
        operator NativeHandle() const noexcept { return handle; }
        const NativeHandle *getHandleAddress() const noexcept { return &handle; }
        const std::shared_ptr<IAllocator>& getHostAllocator() const noexcept { return hostAllocator; }

    protected:
        Object(VkObjectType objectType,
            Type handle,
            std::shared_ptr<IAllocator> allocator) noexcept;
        VkObjectType getType() const noexcept;

    #if (VK_USE_64_BIT_PTR_DEFINES == 0)
        const VkObjectType objectType;
    #endif
        NativeHandle handle;
        std::shared_ptr<IAllocator> hostAllocator;
    };
} // namespace magma

#include "object.inl"
