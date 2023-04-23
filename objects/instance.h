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
#include "dispatchable.h"

namespace magma
{
    class Application;
    class PhysicalDevice;
    class PhysicalDeviceGroup;

    /* There is no global state in Vulkan and all per-application state
       is stored in a instance object. Creating an instance object initializes
       the Vulkan library and allows the application to pass information
       about itself to the implementation. */

    class Instance : public Dispatchable<VkInstance>,
        public std::enable_shared_from_this<Instance>
    {
    public:
        explicit Instance(const std::vector<const char *>& enabledLayers,
            const std::vector<const char *>& enabledExtensions,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const Application *applicationInfo = nullptr,
        #ifdef VK_EXT_debug_utils
            PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsCallback = nullptr,
        #endif
        #ifdef VK_EXT_debug_report
            PFN_vkDebugReportCallbackEXT debugReportCallback = nullptr,
        #endif
            void *userData = nullptr);
        ~Instance();
        uint32_t getApiVersion() const noexcept { return apiVersion; }
        const std::set<std::string>& getEnabledLayers() const noexcept { return enabledLayers; }
        const std::set<std::string>& getEnabledExtensions() const noexcept { return enabledExtensions; }
        uint32_t enumeratePhysicalDevices() const;
        std::shared_ptr<PhysicalDevice> getPhysicalDevice(uint32_t deviceId) const;
#ifdef VK_KHR_device_group
        std::vector<VkPhysicalDeviceGroupPropertiesKHR> enumeratePhysicalDeviceGroups() const;
        std::shared_ptr<PhysicalDeviceGroup> getPhysicalDeviceGroup(uint32_t groupId) const;
#endif
        static uint32_t enumerateVersion() noexcept;
        static std::vector<VkLayerProperties> enumerateLayers();
        static std::vector<VkExtensionProperties> enumerateExtensions(const char *layerName = nullptr);
        bool extensionSupported(const char *extensionName) const noexcept;
        bool extensionEnabled(const char *extensionName) const noexcept;

    private:
        const uint32_t apiVersion;
        std::set<std::string> extensions;
        std::set<std::string> enabledLayers;
        std::set<std::string> enabledExtensions;
    };
} // namespace magma
