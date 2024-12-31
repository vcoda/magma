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
#include "object.h"
#include "deviceChild.h"
#include "../deviceResourcePool.h"

namespace magma
{
    /* Non-dispatchable handle types are a 64-bit integer type
       whose meaning is implementation-dependent. If the privateData
       feature is enabled for a VkDevice, each object of a non-
       dispatchable type created on that device must have a handle
       value that is unique among objects created on that device,
       for the duration of the object's lifetime. Otherwise, non-
       dispatchable handles may encode object information directly
       in the handle rather than acting as a reference to an underlying
       object, and thus may not have unique handle values. */

    template<class Type>
    class NonDispatchable : public DeviceChild,
        public Object<Type>
    {
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        static_assert(sizeof(Type) == sizeof(intptr_t),
    #else
        static_assert(sizeof(Type) == sizeof(uint64_t),
    #endif
            "invalid size of non-dispatchable handle type");
    public:
        ~NonDispatchable();
        Class getClass() const noexcept override { return Class::NonDispatchable; }
        VkObjectType getObjectType() const noexcept override;
        uint64_t getObjectHandle() const noexcept override;

    protected:
        NonDispatchable(VkObjectType objectType,
            Type handle) noexcept;
        NonDispatchable(VkObjectType objectType,
            std::shared_ptr<IAllocator> allocator) noexcept;
        NonDispatchable(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept;
    };
} // namespace magma

#include "nondispatchable.inl"
