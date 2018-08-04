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
#include "debugMarker.h"

namespace magma
{
    /* Dispatchable handle types are a pointer to an opaque type.
       This pointer may be used by layers as part of intercepting API commands,
       and thus each API command takes a dispatchable type as its first parameter.
       Each object of a dispatchable type must have a unique handle value during its lifetime. */

    template<typename Type>
    class Dispatchable : public DebugMarker
    {
    public:
        typedef Type VkType;

    public:
        virtual uint64_t getHandle() const noexcept override
            { return reinterpret_cast<uint64_t>(handle); }
        operator Type() const noexcept
            { return handle; }

    protected:
        Dispatchable(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<const Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept:
            DebugMarker(objectType, std::move(device), std::move(allocator)),
            handle(nullptr) {}

    protected:
        Type handle;
    };

    /* Non-dispatchable handle types are a 64-bit integer type whose meaning is
       implementation-dependent, and may encode object information directly
       in the handle rather than acting as a reference to an underlying object. */

    template<typename Type>
    class NonDispatchable : public DebugMarker
    {
    public:
        typedef Type VkType;

    public:
#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
        virtual uint64_t getHandle() const noexcept override
            { return reinterpret_cast<uint64_t>(handle); }
#else
        virtual uint64_t getHandle() const noexcept override
            { return handle; }
#endif
        operator Type() const noexcept
            { return handle; }

    protected:
        NonDispatchable(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<const Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept:
            DebugMarker(objectType, std::move(device), std::move(allocator)),
            handle(VK_NULL_HANDLE) {}

    protected:
        Type handle;
    };
} // namespace magma
