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
#include "../allocator/objectAllocator.h"

namespace magma
{
    class Device;
    class IAllocator;

    /* Base non-copyable object for dispatchable and non-dispatchable handles.
       Provides user-defined new and delete operators for custom allocations. */

    class Object : public detail::NonCopyable
    {
    public:
        void *operator new(std::size_t size);
        void *operator new(std::size_t size, const std::nothrow_t&) noexcept;
        void* operator new(std::size_t, void* where) noexcept { return where; }
        void operator delete(void *ptr);
        void operator delete(void *, void *) {}
        explicit Object(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept;
        virtual ~Object() = default;
        VkObjectType getObjectType() const noexcept { return objectType; }
        std::shared_ptr<Device> getDevice() const noexcept { return device; }
        std::shared_ptr<IAllocator> getAllocator() const noexcept { return allocator; }
        void setObjectName(const char *name) noexcept;
        void setObjectTag(uint64_t tagName, size_t tagSize, const void *tag) noexcept;
        template<typename Tag>
        void setObjectTag(uint64_t tagName, const Tag& tag) noexcept
            { setObjectTag(tagName, sizeof(Tag), &tag); }
        virtual uint64_t getHandle() const noexcept = 0;
        static void overrideDefaultAllocator(std::shared_ptr<IObjectAllocator> allocator);
        static std::shared_ptr<IObjectAllocator> getOverridenAllocator() noexcept;

    protected:
        VkObjectType objectType;
        std::shared_ptr<Device> device;
        std::shared_ptr<IAllocator> allocator;

    private:
        static std::shared_ptr<IObjectAllocator> objectAllocator;
        static std::atomic<int64_t> allocCount;
    };
} // namespace magma

#include "object.inl"
