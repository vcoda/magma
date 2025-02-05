/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "model/dispatchable.h"
#include "privateDataSlot.h"
#include "../misc/deviceQueueDescriptor.h"
#include "../misc/deviceFaultInfo.h"
#include "../misc/structureChain.h"

namespace magma
{
    class PhysicalDevice;
    class Surface;
    class Queue;
    class Fence;
#ifdef VK_EXT_private_data
    class PrivateDataSlot;
#endif
#ifdef VK_KHR_timeline_semaphore
    class TimelineSemaphore;
#endif
    class FeatureQuery;
    class DeviceResourcePool;
#ifdef VK_KHR_acceleration_structure
    struct AccelerationStructureHeader;
#endif

    /* Device objects represent logical connections to physical
       devices. Each device exposes a number of queue families
       each having one or more queues. An application must create
       a separate logical device for each physical device it will
       use. The created logical device is then the primary interface
       to the physical device. */

    class Device : public Dispatchable<VkDevice>,
        public std::enable_shared_from_this<Device>
    {
    public:
        ~Device();
        const std::shared_ptr<PhysicalDevice>& getPhysicalDevice() const noexcept { return physicalDevice; }
        const std::set<std::string>& getEnabledLayers() const noexcept { return enabledLayers; }
        const std::set<std::string>& getEnabledExtensions() const noexcept { return enabledExtensions; }
        const VkPhysicalDeviceFeatures& getEnabledFeatures() const noexcept { return enabledFeatures; }
        const std::unique_ptr<FeatureQuery>& checkFeatures() const;
        std::shared_ptr<Queue> getQueue(VkQueueFlagBits flags,
            uint32_t queueIndex = 0) const;
        std::shared_ptr<Queue> getQueueByFamily(uint32_t queueFamilyIndex,
            uint32_t queueIndex = 0) const;
        void updateDescriptorSets(uint32_t descriptorWriteCount,
            const VkWriteDescriptorSet *descriptorWrites,
            uint32_t descriptorCopyCount,
            const VkCopyDescriptorSet *descriptorCopies) const noexcept;
        void updateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites,
            const std::vector<VkCopyDescriptorSet>& descriptorCopies = {}) const noexcept;
        bool waitIdle() const;
        bool resetFences(std::vector<lent_ptr<Fence>>& fences) const noexcept;
        bool waitForFences(const std::vector<lent_ptr<Fence>>& fences,
            bool waitAll,
            uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
    #ifdef VK_KHR_timeline_semaphore
        bool waitSemaphores(const std::vector<lent_ptr<TimelineSemaphore>>& semaphores,
            const std::vector<uint64_t>& values,
            bool waitAll,
            uint64_t timeout = std::numeric_limits<uint64_t>::max()) const;
    #endif // VK_KHR_timeline_semaphore
    #ifdef VK_KHR_maintenance3
        bool getDescriptorSetLayoutSupport(const std::vector<VkDescriptorSetLayoutBinding>& bindings,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain()) const;
    #ifdef VK_EXT_descriptor_indexing
        uint32_t getVariableDescriptorCountLayoutSupport(const std::vector<VkDescriptorSetLayoutBinding>& bindings,
            VkDescriptorSetLayoutCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain()) const;
    #endif // VK_EXT_descriptor_indexing
    #endif // VK_KHR_maintenance3
    #ifdef VK_KHR_performance_query
        void acquireProfilingLock(VkAcquireProfilingLockFlagsKHR flags = 0,
            uint64_t timeout = std::numeric_limits<uint64_t>::max());
        void releaseProfilingLock();
    #endif // VK_KHR_performance_query
    #ifdef VK_KHR_device_group
        VkDeviceGroupPresentCapabilitiesKHR getDeviceGroupPresentCapabilitiesKHR() const;
        VkDeviceGroupPresentModeFlagsKHR getDeviceGroupSurfacePresentModes(lent_ptr<const Surface> surface) const;
    #ifdef VK_EXT_full_screen_exclusive
        VkDeviceGroupPresentModeFlagsKHR getDeviceGroupFullScreenExclusiveSurfacePresentModes(lent_ptr<const Surface> surface,
            VkFullScreenExclusiveEXT fullScreenExclusive
        #ifdef VK_KHR_win32_surface
           ,HMONITOR hMonitor = NULL
        #endif
            ) const;
    #endif // VK_EXT_full_screen_exclusive
        VkPeerMemoryFeatureFlags getDeviceGroupPeerMemoryFeatures(uint32_t heapIndex,
            uint32_t localDeviceIndex,
            uint32_t remoteDeviceIndex) const;
    #endif // VK_KHR_device_group
    #ifdef VK_KHR_acceleration_structure
        bool getAccelerationStructureCompatibility(const AccelerationStructureHeader *header) const;
    #endif
    #ifdef VK_EXT_calibrated_timestamps
        std::vector<uint64_t> getCalibratedTimestamps(const std::vector<VkTimeDomainEXT>& timeDomains,
            uint64_t *maxDeviation = nullptr) const;
    #endif
    #ifdef VK_EXT_device_fault
        DeviceFaultInfo getFaultInfo() const;
    #endif
        std::unordered_map<uint64_t, uint64_t>& getPrivateDataMap() noexcept { return privateData; }
    #ifdef VK_EXT_private_data
        void setPrivateDataSlot(std::unique_ptr<PrivateDataSlot> privateDataSlot_) noexcept { privateDataSlot = std::move(privateDataSlot_); }
        const std::unique_ptr<PrivateDataSlot>& getPrivateDataSlot() const noexcept { return privateDataSlot; }
    #endif // VK_EXT_private_data
        template<class PhysicalDeviceFeatures>
        const PhysicalDeviceFeatures *getEnabledExtendedFeatures() const noexcept;
        bool extensionEnabled(const char *extensionName) const noexcept;
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        const std::unique_ptr<DeviceResourcePool>& getResourcePool() const noexcept { return resourcePool; }
    #endif

    private:
        MAGMA_MAKE_SHARED(Device)
        Device(std::shared_ptr<PhysicalDevice> physicalDevice,
            const std::vector<DeviceQueueDescriptor>& queueDescriptors,
            const NullTerminatedStringArray& enabledLayers,
            const NullTerminatedStringArray& enabledExtensions,
            const VkPhysicalDeviceFeatures& enabledFeatures,
            const StructureChain& enabledExtendedFeatures,
            const StructureChain& extendedInfo,
            std::shared_ptr<IAllocator> allocator);
        VkDevice getNativeDevice() const noexcept { return handle; }
        VkInstance getNativeInstance() const noexcept;
        bool supportsQueueFamily(uint32_t queueFamilyIndex,
            uint32_t queueIndex) const noexcept;

        std::shared_ptr<PhysicalDevice> physicalDevice;
        const std::vector<DeviceQueueDescriptor> queueDescriptors;
        std::set<std::string> enabledLayers;
        std::set<std::string> enabledExtensions;
        const VkPhysicalDeviceFeatures enabledFeatures;
        const StructureChain enabledExtendedFeatures;
        std::map<VkStructureType, const VkBaseInStructure *> extendedFeatures;
        mutable std::map<std::pair<uint32_t, uint32_t>, std::shared_ptr<Queue>> queues;
        std::unordered_map<uint64_t, uint64_t> privateData;
    #ifdef VK_EXT_private_data
        std::unique_ptr<PrivateDataSlot> privateDataSlot;
    #endif
        mutable std::unique_ptr<FeatureQuery> featureQuery;
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        std::unique_ptr<DeviceResourcePool> resourcePool;
    #endif
        friend PhysicalDevice;
    };
} // namespace magma

#define MAGMA_SPECIALIZE_PHYSICAL_DEVICE_FEATURES(PhysicalDeviceFeatures, structureType)\
template<>\
inline const PhysicalDeviceFeatures *Device::getEnabledExtendedFeatures<PhysicalDeviceFeatures>() const noexcept\
{\
    auto it = extendedFeatures.find(structureType);\
    if (it != extendedFeatures.end())\
        return reinterpret_cast<const PhysicalDeviceFeatures *>(it->second);\
    return nullptr;\
}

#include "device.inl"
