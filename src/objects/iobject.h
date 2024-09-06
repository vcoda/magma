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
#include "../misc/compatibility.h"

namespace magma
{
    /* Base interface class of Vulkan object. At the API level,
       all objects are referred to by handles. There are two
       classes of handles, dispatchable and non-dispatchable. */

    class IObject : public IClass
    {
    public:
        enum class Class : uint8_t;
        virtual Class getClass() const noexcept = 0;
        virtual VkObjectType getObjectType() const noexcept = 0;
        virtual uint64_t getObjectHandle() const noexcept = 0;
    };

    /* Dispatchable handle types are a pointer to an opaque type.
       Non-dispatchable handle types are a 64-bit integer type
       whose meaning is implementation-dependent. */

    enum class IObject::Class : uint8_t
    {
        Dispatchable, NonDispatchable
    };
} // namespace magma
