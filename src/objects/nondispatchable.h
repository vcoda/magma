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
#include "object.h"
#include "resourcePool.h"
#include "../misc/structureChain.h"

namespace magma
{
    class Device;
    class NonDispatchableImpl;

    /* Non-dispatchable handle types are a 64-bit integer type
       whose meaning is implementation-dependent, and may encode
       object information directly in the handle rather than acting
       as a reference to an underlying object. */

    template<class Type>
    class NonDispatchable : public Object<Type>,
        /* private */ DeviceResourcePool
    {
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        static_assert(sizeof(Type) == sizeof(intptr_t),
    #else
        static_assert(sizeof(Type) == sizeof(uint64_t),
    #endif
            "invalid size of non-dispatchable handle type");

    public:
        const std::shared_ptr<Device>& getDevice() const noexcept { return device; }
        uint64_t getObjectHandle() const noexcept override;
        void setPrivateData(uint64_t data) override;
        uint64_t getPrivateData() const noexcept override;
        bool nonDispatchable() const noexcept override { return true; }
        void setDebugName(const char *name);
        void setDebugTag(uint64_t tagName, size_t tagSize, const void *tag);

    protected:
        NonDispatchable(VkObjectType objectType,
            std::shared_ptr<IAllocator> allocator) noexcept;
        NonDispatchable(VkObjectType objectType,
            std::shared_ptr<Device> device_,
            std::shared_ptr<IAllocator> allocator) noexcept;
        ~NonDispatchable();

        std::shared_ptr<Device> device;
        std::unique_ptr<NonDispatchableImpl> pimpl;
    };

    /* The purpose is to implement functionality of template
       NonDispatchable class without circular reference to it
       from PrivateDataSlot object. Also allows include dependencies
       from implementation file and not from base header file. */

    class NonDispatchableImpl
    {
    public:
        template<class Type>
        explicit NonDispatchableImpl(const NonDispatchable<Type> *parent) noexcept:
            parent(parent), device(parent->getDevice().get()) {}
        void setPrivateData(uint64_t data);
        uint64_t getPrivateData() const noexcept;
        void setDebugName(const char *name);
        void setDebugTag(uint64_t tagName, size_t tagSize, const void *tag);

    private:
        const IObject *parent;
        Device *device;
        static std::mutex mtx;
    };
} // namespace magma

#include "nondispatchable.inl"
