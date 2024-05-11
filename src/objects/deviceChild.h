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

namespace magma
{
    class Device;
    class DeviceResourcePool;

    /* Introduces concepts of object naming and tagging,
       for better tracking of Vulkan objects. This can be
       used by debugging layers to easily filter for only
       data that can be used by that implementation. */

    class DeviceChild : public IObject
    {
    public:
        void setPrivateData(uint64_t data) override;
        uint64_t getPrivateData() const noexcept override;
        const std::shared_ptr<Device>& getDevice() const noexcept { return device; }
        void setDebugName(const char *MAGMA_NOOP(name)) MAGMA_NOOP_METHOD;
        void setDebugTag(uint64_t MAGMA_NOOP(tagName),
            size_t MAGMA_NOOP(tagSize),
            const void *MAGMA_NOOP(tag)) MAGMA_NOOP_METHOD;
        template<class Tag>
        void setDebugTag(uint64_t tagName, const Tag& tag)
            { setDebugTag(tagName, sizeof(Tag), &tag); }

    protected:
        DeviceChild(std::shared_ptr<Device> device) noexcept:
            device(std::move(device)) {}
        VkDevice getNativeDevice() const noexcept;
        virtual VkPhysicalDevice getNativePhysicalDevice() const noexcept;
        virtual VkInstance getNativeInstance() const noexcept;
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        std::shared_ptr<DeviceResourcePool> getResourcePool() noexcept;
    #endif
        std::shared_ptr<Device> device;

    private:
        static std::mutex mtx;
    };
} // namespace magma
