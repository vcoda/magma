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
#include <vector>
#include <string>
#include "handle.h"

namespace magma
{
    class PhysicalDevice;

    /* There is no global state in Vulkan and all per-application state
       is stored in a instance object. Creating an instance object initializes
       the Vulkan library and allows the application to pass information
       about itself to the implementation. */

    class Instance : public Dispatchable<VkInstance>,
        public std::enable_shared_from_this<Instance>
    {
    public:
        explicit Instance(const char *applicationName,
            const char *engineName,
            uint32_t apiVersion,
            const std::vector<const char *>& layerNames,
            const std::vector<const char *>& extensionNames,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~Instance();
        uint32_t countPhysicalDevices() const;
        std::shared_ptr<PhysicalDevice> getPhysicalDevice(uint32_t deviceId);
        std::vector<VkExtensionProperties> enumerateExtensions(const char *layerName = nullptr) const;
    };
} // namespace magma
