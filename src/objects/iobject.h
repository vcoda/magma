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
    /* Base interface class of Vulkan object that has associated
       handle and type. Derived classes should implement virtual
       methods declared here. */

    class IObject : public IClass
    {
    public:
        virtual VkObjectType getObjectType() const noexcept = 0;
        virtual uint64_t getObjectHandle() const noexcept = 0;
        virtual void setPrivateData(uint64_t data) = 0;
        virtual uint64_t getPrivateData() const noexcept = 0;
        virtual bool nonDispatchable() const noexcept = 0;
    };
} // namespace magma
