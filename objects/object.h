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
#include "../vulkan.h"
#include "../nonCopyable.h"

namespace magma
{
    class Device;
    class IAllocator;
    class IObjectAllocator;

    /* Base non-copyable object for dispatchable and non-dispatchable handles. */

    class Object : public NonCopyable
    {
    public:
        // Notice that std::make_shared() constructs an objects via placement new,
        // so custom allocation functions do not used in that case.
        void *operator new(std::size_t size);
        void *operator new(std::size_t size, const std::nothrow_t&) noexcept;
        void operator delete(void *ptr);

        static void setAllocator(std::shared_ptr<IObjectAllocator> allocator);
        static std::shared_ptr<IObjectAllocator> getAllocator() noexcept;

    public:
        explicit Object(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator):
            objectType(objectType),
            device(std::move(device)),
            allocator(std::move(allocator)) {}
        VkDebugReportObjectTypeEXT getObjectType() const noexcept { return objectType; }
        std::shared_ptr<Device> getDevice() const noexcept { return device; }
        virtual uint64_t getHandle() const noexcept = 0;

    protected:
        VkDebugReportObjectTypeEXT objectType;
        std::shared_ptr<Device> device;
        std::shared_ptr<IAllocator> allocator;

    private:
        static std::shared_ptr<IObjectAllocator> _allocator;
        static int32_t _allocCount;
    };
} // namespace magma
