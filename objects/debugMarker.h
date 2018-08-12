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
#include "object.h"

namespace magma
{
    class Device;

    /* Debug markers provide a flexible way for debugging and validation layers
       to receive annotation and debug information. */

    class DebugMarker : public Object
    {
    public:
        DebugMarker(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator);
        void setObjectTag(uint64_t name, size_t tagSize, const void *tag) noexcept;
        template<typename Tag>
        void setObjectTag(uint64_t name, const Tag& tag) noexcept
            { setObjectTag(name, sizeof(Tag), &tag); }
        void setObjectName(const char *name) noexcept;
    };
} // namespace magma
