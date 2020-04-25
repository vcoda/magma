/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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

namespace magma
{
    /* Non-dispatchable handle types are a 64-bit integer type whose meaning is
       implementation-dependent, and may encode object information directly
       in the handle rather than acting as a reference to an underlying object. */

    template<typename Type>
    class NonDispatchable : public Object
    {
    public:
        typedef Type NativeHandle;

    public:
        virtual uint64_t getHandle() const noexcept override
        {
#ifdef MAGMA_X64
            return reinterpret_cast<uint64_t>(handle);
#else
            return handle;
#endif
        }
        operator Type() const noexcept
            { return handle; }

    protected:
        explicit NonDispatchable(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator) noexcept:
            Object(objectType, std::move(device), std::move(allocator)),
            handle(VK_NULL_HANDLE) {}

    protected:
        Type handle;
    };
} // namespace magma
