/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../misc/deviceQueueDescriptor.h"
#include "../helpers/stackArray.h"

namespace magma
{
    class PhysicalDevice;
    class Surface;
    class Queue;
    class Fence;
    class ResourcePool;

    /* Device objects represent logical connections to physical devices.
       Each device exposes a number of queue families each having one or more queues.
       An application must create a separate logical device for each physical device it will use.
       The created logical device is then the primary interface to the physical device. */

    class Device : public Dispatchable<VkDevice>,
        public std::enable_shared_from_this<Device>
    {
        explicit Device(std::shared_ptr<PhysicalDevice> physicalDevice,
            const std::vector<DeviceQueueDescriptor>& queueDescriptors,
            const std::vector<const char *>& enabledLayers,
            const std::vector<const char *>& enabledExtensions,
            const VkPhysicalDeviceFeatures& deviceFeatures,
            const std::vector<void *>& extendedDeviceFeatures,
            std::shared_ptr<IAllocator> allocator);
        friend PhysicalDevice;

    public:
        ~Device();
        std::shared_ptr<Queue> getQueue(VkQueueFlagBits flags, uint32_t queueIndex) const;
        void updateDescriptorWrites(const helpers::StackArray<VkWriteDescriptorSet>& descriptorWrites) const noexcept;
        void updateDescriptorCopies(const helpers::StackArray<VkCopyDescriptorSet>& descriptorCopies) const noexcept;
        void updateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites,
            const std::vector<VkCopyDescriptorSet>& descriptorCopies = {}) const noexcept;
        bool waitIdle() const;
        bool resetFences(std::vector<std::shared_ptr<Fence>>& fences) const noexcept;
        bool waitForFences(std::vector<std::shared_ptr<Fence>>& fences,
            bool waitAll, uint64_t timeout = UINT64_MAX) const noexcept;
#ifdef VK_KHR_device_group
        VkDeviceGroupPresentModeFlagsKHR getDeviceGroupSurfacePresentModes(std::shared_ptr<const Surface> surface) const;
#   ifdef VK_EXT_full_screen_exclusive
        VkDeviceGroupPresentModeFlagsKHR getDeviceGroupSurfaceFullScreenExclusivePresentModes(std::shared_ptr<const Surface> surface,
            VkFullScreenExclusiveEXT fullScreenExclusive,
            void *hMonitor /* nullptr */) const;
#   endif
        VkPeerMemoryFeatureFlags getGroupPeerMemoryFeatures(uint32_t heapIndex,
            uint32_t localDeviceIndex,
            uint32_t remoteDeviceIndex) const noexcept;
#endif // VK_KHR_device_group
        // Non-API
        std::shared_ptr<PhysicalDevice> getPhysicalDevice() noexcept { return physicalDevice; }
        std::shared_ptr<const PhysicalDevice> getPhysicalDevice() const noexcept { return physicalDevice; }
        std::shared_ptr<ResourcePool> getResourcePool() noexcept { return resourcePool; }
        std::shared_ptr<const ResourcePool> getResourcePool() const noexcept { return resourcePool; }
        const std::vector<std::string>& getEnabledLayers() const noexcept { return enabledLayers; }
        const std::vector<std::string>& getEnabledExtensions() const noexcept { return enabledExtensions; }
        const VkPhysicalDeviceFeatures& getEnabledFeatures() const noexcept { return enabledFeatures; }
#ifdef VK_KHR_get_physical_device_properties2
        const std::vector<void *>& getEnabledExtendedFeatures() const noexcept { return enabledExtendedFeatures; }
#endif
        bool extensionEnabled(const char *extensionName) const;
        bool separateDepthStencilLayoutsEnabled() const noexcept;
        bool negativeViewportHeightEnabled(bool khronos) const noexcept;

    private:
        const void *findExtendedFeatures(VkStructureType featuresType) const noexcept;

        std::shared_ptr<PhysicalDevice> physicalDevice;
        mutable std::vector<std::pair<DeviceQueueDescriptor, std::weak_ptr<Queue>>> queues;
        std::shared_ptr<ResourcePool> resourcePool;
        std::vector<std::string> enabledLayers;
        std::vector<std::string> enabledExtensions;
        VkPhysicalDeviceFeatures enabledFeatures;
#ifdef VK_KHR_get_physical_device_properties2
        std::vector<void *> enabledExtendedFeatures;
#endif
    };
} // namespace magma
