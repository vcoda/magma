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
#include "../misc/deviceQueueDesc.h"

namespace magma
{
    class Instance;
    class Surface;
    class Display;

    /* A physical device usually represents a single complete implementation of Vulkan
       (excluding instance-level functionality) available to the host,
       of which there are a finite number. */

    class PhysicalDevice : public Dispatchable<VkPhysicalDevice>,
        public std::enable_shared_from_this<PhysicalDevice>
    {
        explicit PhysicalDevice(std::shared_ptr<Instance> instance, VkPhysicalDevice handle,
            std::shared_ptr<IAllocator> allocator);
        friend class Instance;

    public:
        const VkPhysicalDeviceFeatures& getFeatures() const noexcept;
        VkFormatProperties getFormatProperties(VkFormat format) const noexcept;
        VkImageFormatProperties getImageFormatProperties(VkFormat format,
            VkImageType, bool optimalTiling, VkImageUsageFlags usage,
            VkImageCreateFlags flags = 0) const;
        const VkPhysicalDeviceProperties& getProperties() const noexcept;
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const noexcept;
        const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const noexcept;
        std::shared_ptr<Device> createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
            const std::vector<const char *>& layers,
            const std::vector<const char *>& extensions,
            const VkPhysicalDeviceFeatures& deviceFeatures,
            const std::vector<void *>& extendedDeviceFeatures = {}) const;
        std::vector<VkExtensionProperties> enumerateExtensions(const char *layerName = nullptr) const;
        std::vector<VkLayerProperties> enumerateLayerProperties() const;
        bool getSurfaceSupport(std::shared_ptr<Surface> surface) const noexcept;
        VkSurfaceCapabilitiesKHR getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const;
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats(std::shared_ptr<const Surface> surface) const;
        std::vector<VkPresentModeKHR> getSurfacePresentModes(std::shared_ptr<const Surface> surface) const;
        // VK_KHR_xxx_surface
        bool getPresentationSupport(uint32_t queueFamilyIndex,
            void *display, const void *visualID = nullptr) const noexcept;
        // VK_KHR_display
        std::vector<VkDisplayPropertiesKHR> getDisplayProperties() const;
        std::vector<VkDisplayPlanePropertiesKHR> getDisplayPlaneProperties() const;
        std::vector<std::shared_ptr<Display>> getSupportedDisplays(uint32_t planeIndex) const;
        // VK_AMD_shared_core_properties
        const VkPhysicalDeviceShaderCorePropertiesAMD& getShaderCoreProperties() const;
        // Non-API
        std::shared_ptr<Instance> getInstance() const noexcept { return instance; }
        std::shared_ptr<Device> createDefaultDevice() const;
        bool checkPipelineCacheDataCompatibility(const void *cacheData) const noexcept;

    private:
        std::shared_ptr<Instance> instance;
        mutable VkPhysicalDeviceFeatures features = {};
        mutable VkPhysicalDeviceProperties properties = {};
        mutable VkPhysicalDeviceMemoryProperties memoryProperties = {};
        mutable VkPhysicalDeviceShaderCorePropertiesAMD shaderCoreProperties = {};
    };
} // namespace magma
