/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include <memory>
#include "../allocator/allocator.h"

namespace magma
{
    class Device;
    class IAllocator;

    /* Base non-copyable object for dispatchable and non-dispatchable handles. */

    class Object : public NonCopyable
    {
    public:
        Object(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<const Device> device,
            std::shared_ptr<IAllocator> allocator):
            objectType(objectType),
            device(std::move(device)),
            allocator(std::move(allocator)) {}
        VkDebugReportObjectTypeEXT getObjectType() const noexcept { return objectType; }
        std::shared_ptr<const Device> getDevice() const noexcept { return device; }
        virtual uint64_t getHandle() const noexcept = 0;

    protected:
        VkDebugReportObjectTypeEXT objectType;
        std::shared_ptr<const Device> device;
        std::shared_ptr<IAllocator> allocator;
    };
} // namespace magma
