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
#include <set>
#include <string>
#include "handle.h"

namespace magma
{
    class Device;
    class Surface;

    class PhysicalDevice : public Handle<VkPhysicalDevice>
    {
    public:
        PhysicalDevice(VkPhysicalDevice physicalDevice);   
        std::shared_ptr<Device> createDevice(
            const std::vector<const char *>& layers,
            const std::vector<const char *>& extensions, 
            const VkPhysicalDeviceFeatures& deviceFeatures);
        std::shared_ptr<Device> createDefaultDevice();
        std::set<std::string> enumerateExtensions(const char *layerName = nullptr) const;
        bool surfaceSupported(std::shared_ptr<Surface> surface) const;
        bool depthStencilSupported(VkFormat format) const;
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats(std::shared_ptr<Surface> surface) const;
        void getSurfaceCapabilities(std::shared_ptr<Surface> surface, 
            VkSurfaceCapabilitiesKHR& caps) const;
    };
} // namespace magma
