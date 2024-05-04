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
#include "deviceResourcePool.h"
#include "../misc/structureChain.h"

namespace magma
{
    class Device;

    /* Introduces concepts of object naming and tagging,
       for better tracking of Vulkan objects. This can be
       used by debugging layers to easily filter for only
       data that can be used by that implementation.

       Allows to implement functionality of template
       NonDispatchable class without circular reference
       to it from PrivateDataSlot class. */

    class NonDispatchableImpl
    {
    public:
        const std::shared_ptr<Device>& getDevice() const noexcept { return device; }

    protected:
        NonDispatchableImpl(std::shared_ptr<Device> device) noexcept:
            device(std::move(device)) {}
        VkDevice getNativeDevice() const noexcept;
        VkInstance getNativeInstance() const noexcept;
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        std::shared_ptr<DeviceResourcePool> getResourcePool() noexcept;
    #endif
        void setPrivateData(const IObject *self,
            uint64_t data);
        uint64_t getPrivateData(const IObject *self) const noexcept;
        void setDebugName(const IObject *self,
            const char *name);
        void setDebugTag(const IObject *self,
            uint64_t tagName, size_t tagSize, const void *tag);

        std::shared_ptr<Device> device;

    private:
        static std::mutex mtx;
    };

    /* Non-dispatchable handle types are a 64-bit integer type
       whose meaning is implementation-dependent, and may encode
       object information directly in the handle rather than acting
       as a reference to an underlying object. */

    template<class Type>
    class NonDispatchable : public Object<Type>,
        public NonDispatchableImpl
    {
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        static_assert(sizeof(Type) == sizeof(intptr_t),
    #else
        static_assert(sizeof(Type) == sizeof(uint64_t),
    #endif
            "invalid size of non-dispatchable handle type");

    public:
        uint64_t getObjectHandle() const noexcept override;
        void setPrivateData(uint64_t data) override;
        uint64_t getPrivateData() const noexcept override;
        bool nonDispatchable() const noexcept override { return true; }
        void setDebugName(const char *name);
        void setDebugTag(uint64_t tagName, size_t tagSize, const void *tag);
        template<class Tag>
        void setDebugTag(uint64_t tagName, const Tag& tag);

    protected:
        NonDispatchable(VkObjectType objectType,
            std::shared_ptr<IAllocator> allocator) noexcept;
        NonDispatchable(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept;
        ~NonDispatchable();
    };
} // namespace magma

#include "nondispatchable.inl"
