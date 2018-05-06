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

    // Every Magma object inherited from debug one to enable debugging markers
    class DebugObject : public NonCopyable
    {
    public:
        static void initDebugMarker(std::shared_ptr<const Device> device);
        static bool debugMarkerEnabled();

    public:
        DebugObject(VkDebugReportObjectTypeEXT objectType,
            std::shared_ptr<const Device> device);
        void setMarkerTag(uint64_t name, size_t tagSize, const void *tag);
        template<typename Tag>
        void setMarkerTag(uint64_t name, const Tag& tag)
        { 
            setMarkerTag(name, sizeof(Tag), &tag);
        }
        void setMarkerName(const char *name);
        virtual uint64_t getObject() const = 0;
        virtual std::shared_ptr<const Device> getDevice() const { return device; }

    protected:
        VkDebugReportObjectTypeEXT objectType;
        std::shared_ptr<const Device> device;
    };
} // namespace magma
