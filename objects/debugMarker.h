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

    class DebugMarker : public NonCopyable
    {
    public:
        DebugMarker(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<const Device> device);
        void setObjectTag(uint64_t name, size_t tagSize, const void *tag) noexcept;
        template<typename Tag>
        void setObjectTag(uint64_t name, const Tag& tag) noexcept 
            { setObjectTag(name, sizeof(Tag), &tag); }
        void setObjectName(const char *name) noexcept;
        virtual uint64_t getObject() const = 0;
        virtual std::shared_ptr<const Device> getDevice() const noexcept
            { return device; }

    protected:
        VkDebugReportObjectTypeEXT objectType;
        std::shared_ptr<const Device> device;
    };
} // namespace magma
