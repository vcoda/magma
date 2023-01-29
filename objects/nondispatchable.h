/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "resourcePool.h"
#include "../misc/structureChain.h"

namespace magma
{
    /* Non-dispatchable handle types are a 64-bit integer type whose meaning is
       implementation-dependent, and may encode object information directly
       in the handle rather than acting as a reference to an underlying object. */

    template<typename Type>
    class NonDispatchable : public ObjectT<Type>,
        /* private */ DeviceResourcePool
    {
    public:
        typedef Type NativeHandle;
    #ifdef MAGMA_X64
        // VK_EXT_debug_utils/VK_EXT_debug_marker requires uint64_t type
        virtual uint64_t getHandle() const noexcept override
            { return reinterpret_cast<uint64_t>(handle); }
    #else
        virtual uint64_t getHandle() const noexcept override { return handle; }
    #endif
        const NativeHandle *getHandleAddress() const noexcept { return &handle; }
        operator NativeHandle() const noexcept { return handle; }

    protected:
        explicit NonDispatchable(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> hostAllocator) noexcept:
            ObjectT<Type>(objectType, std::move(device), std::move(hostAllocator)),
            handle(VK_NULL_HANDLE)
        {
#       ifdef MAGMA_X64
            std::shared_ptr<ResourcePool> pool = DeviceResourcePool::getPool(ObjectT<Type>::getDevice());
            if (pool) // Put resource in pool
                pool->getPool<NonDispatchable<Type>>().insert(this);
#       endif // MAGMA_X64
        }

        ~NonDispatchable()
        {
#       ifdef MAGMA_X64
            std::shared_ptr<ResourcePool> pool = DeviceResourcePool::getPool(ObjectT<Type>::getDevice());
            if (pool) // Remove resource from pool
                pool->getPool<NonDispatchable<Type>>().erase(this);
#       endif // MAGMA_X64
        }

    protected:
        Type handle;
    };
} // namespace magma
