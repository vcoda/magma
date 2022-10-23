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
#include "../misc/deviceQueueDescriptor.h"
#include "../helpers/stackArray.h"

namespace magma
{
    class PhysicalDevice;
    class Surface;
    class Queue;
    class Fence;
#ifdef VK_KHR_timeline_semaphore
    class TimelineSemaphore;
#endif
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
            const StructureChain& extendedInfo,
            std::shared_ptr<IAllocator> allocator);
        MAGMA_FRIEND_MAKE_SHARED(Device);

    public:
        ~Device();
        std::shared_ptr<Queue> getQueue(VkQueueFlagBits flags, uint32_t queueIndex) const;
        std::shared_ptr<Queue> getQueueForFamily(uint32_t queueFamilyIndex) const;
        void updateDescriptorWrites(const VkWriteDescriptorSet *descriptorWrites,
            uint32_t descriptorWriteCount) const noexcept;
        void updateDescriptorCopies(const VkCopyDescriptorSet *descriptorCopies,
            uint32_t descriptorCopyCount) const noexcept;
        void updateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites,
            const std::vector<VkCopyDescriptorSet>& descriptorCopies = {}) const noexcept;
        bool waitIdle() const;
        bool resetFences(std::vector<std::shared_ptr<Fence>>& fences) const noexcept;
        bool waitForFences(const std::vector<std::shared_ptr<Fence>>& fences,
            bool waitAll,
            uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
    #ifdef VK_KHR_timeline_semaphore
        bool waitSemaphores(const std::vector<std::shared_ptr<TimelineSemaphore>>& semaphores,
            const std::vector<uint64_t>& values,
            bool waitAll,
            uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
    #endif // VK_KHR_timeline_semaphore
#ifdef VK_KHR_device_group
        VkDeviceGroupPresentModeFlagsKHR getDeviceGroupSurfacePresentModes(std::shared_ptr<const Surface> surface) const;
#   ifdef VK_EXT_full_screen_exclusive
        VkDeviceGroupPresentModeFlagsKHR getDeviceGroupSurfaceFullScreenExclusivePresentModes(std::shared_ptr<const Surface> surface,
            VkFullScreenExclusiveEXT fullScreenExclusive,
            void *hMonitor /* nullptr */) const;
#   endif
        VkPeerMemoryFeatureFlags getDeviceGroupPeerMemoryFeatures(uint32_t heapIndex,
            uint32_t localDeviceIndex,
            uint32_t remoteDeviceIndex) const;
#endif // VK_KHR_device_group
        std::shared_ptr<PhysicalDevice> getPhysicalDevice() noexcept { return physicalDevice; }
        std::shared_ptr<const PhysicalDevice> getPhysicalDevice() const noexcept { return physicalDevice; }
        std::shared_ptr<ResourcePool> getResourcePool() noexcept { return resourcePool; }
        std::shared_ptr<const ResourcePool> getResourcePool() const noexcept { return resourcePool; }
        const std::vector<std::string>& getEnabledLayers() const noexcept { return enabledLayers; }
        const std::vector<std::string>& getEnabledExtensions() const noexcept { return enabledExtensions; }
        const VkPhysicalDeviceFeatures& getEnabledFeatures() const noexcept { return enabledFeatures; }
        template<typename PhysicalDeviceFeatures>
        const PhysicalDeviceFeatures *getEnabledExtendedFeatures() const noexcept;
        // Utility methods
        bool extensionEnabled(const char *extensionName) const;
        bool negativeViewportHeightEnabled(bool khronos) const noexcept;
        bool separateDepthStencilLayoutsEnabled() const noexcept;
        bool stippledLinesEnabled() const noexcept;

    private:
        const VkBaseInStructure *findEnabledExtendedFeatures(VkStructureType sType) const noexcept;

        std::shared_ptr<PhysicalDevice> physicalDevice;
        mutable std::vector<std::pair<DeviceQueueDescriptor, std::weak_ptr<Queue>>> queues;
        std::shared_ptr<ResourcePool> resourcePool;
        std::vector<std::string> enabledLayers;
        std::vector<std::string> enabledExtensions;
        VkPhysicalDeviceFeatures enabledFeatures;
        std::vector<const VkBaseInStructure *> enabledExtendedFeatures;
    };
} // namespace magma

#define MAGMA_SPECIALIZE_PHYSICAL_DEVICE_FEATURES(DeviceFeatures, StructureType)\
template<>\
inline const DeviceFeatures *magma::Device::getEnabledExtendedFeatures<DeviceFeatures>() const noexcept\
{\
    return reinterpret_cast<const DeviceFeatures *>(findEnabledExtendedFeatures(StructureType));\
}

#include "device.inl"
