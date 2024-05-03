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

namespace magma
{
    class IObject;
    class Device;

    /* Allows to assign debug name and tag data to an object.
       This can be used by debugging layers to easily filter
       for only data that can be used by that implementation. */

    class DebugUtilsObject : NonCopyable
    {
    public:
        const std::shared_ptr<Device>& getDevice() const noexcept { return device; }

    protected:
        DebugUtilsObject(std::shared_ptr<Device> device = nullptr) noexcept:
            device(std::move(device)) {}
        void setDebugName(const IObject *parent,
            const char *name);
        void setDebugTag(const IObject *parent,
            uint64_t tagName, size_t tagSize, const void *tag);

        std::shared_ptr<Device> device;
    };
} // namespace magma
