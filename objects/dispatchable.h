/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../misc/structureChain.h"

namespace magma
{
    /* Dispatchable handle types are a pointer to an opaque type.
       This pointer may be used by layers as part of intercepting API commands,
       and thus each API command takes a dispatchable type as its first parameter.
       Each object of a dispatchable type must have a unique handle value during its lifetime. */

    template<class Type>
    class Dispatchable : public TObject<Type>
    {
    protected:
        explicit Dispatchable(VkObjectType objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> hostAllocator) noexcept:
            TObject<Type>(objectType, std::move(device), std::move(hostAllocator)) {}
        explicit Dispatchable(VkObjectType objectType,
            Type handle,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> hostAllocator) noexcept:
            TObject<Type>(objectType, handle, std::move(device), std::move(hostAllocator)) {}
        uint64_t getHandle() const noexcept override
            { return reinterpret_cast<uint64_t>(TObject<Type>::handle); }
    };
} // namespace magma
