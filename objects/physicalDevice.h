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
#include "../misc/deviceQueueDesc.h"

namespace magma
{
    class Instance;
    class Surface;

    class PhysicalDevice : public Dispatchable<VkPhysicalDevice>,
        public std::enable_shared_from_this<PhysicalDevice>
    {
        PhysicalDevice(std::shared_ptr<const Instance> instance, VkPhysicalDevice handle,
            std::shared_ptr<IAllocator> allocator = nullptr);
        friend class Instance;

    public: 
        const VkPhysicalDeviceFeatures& getFeatures() const;
        const VkPhysicalDeviceProperties& getProperties() const;
        VkFormatProperties getFormatProperties(VkFormat format) const;
        VkImageFormatProperties getImageFormatProperties(VkFormat format, 
            VkImageType, bool optimalTiling, VkImageUsageFlags usage,
            VkImageCreateFlags flags = 0) const;
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
        const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const;
        std::set<std::string> enumerateExtensions(const char *layerName = nullptr) const;
        std::vector<VkLayerProperties> enumerateLayerProperties() const;
        std::shared_ptr<Device> createDevice(
            const std::vector<DeviceQueueDescriptor>& queueDescriptors,
            const std::vector<const char *>& layers,
            const std::vector<const char *>& extensions,
            const VkPhysicalDeviceFeatures& deviceFeatures) const;
        std::shared_ptr<Device> createDefaultDevice() const;
        bool getSurfaceSupport(std::shared_ptr<Surface> surface) const;
        VkSurfaceCapabilitiesKHR getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const;
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats(std::shared_ptr<const Surface> surface) const;
        std::vector<VkPresentModeKHR> getSurfacePresentModes(std::shared_ptr<const Surface> surface) const;
        // VK_AMD_shared_core_properties
        const VkPhysicalDeviceShaderCorePropertiesAMD& getShaderCoreProperties() const;
        // Non-API
        bool checkPipelineCacheDataCompatibility(const void *cacheData) const;

    private:
        std::shared_ptr<const Instance> instance;
        mutable VkPhysicalDeviceFeatures features = {};
        mutable VkPhysicalDeviceProperties properties = {};
        mutable VkPhysicalDeviceMemoryProperties memoryProperties = {};
        mutable VkPhysicalDeviceShaderCorePropertiesAMD shaderCoreProperties = {};
    };
} // namespace magma
