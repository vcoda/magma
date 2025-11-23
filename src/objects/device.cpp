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
#include "pch.h"
#pragma hdrstop
#include "device.h"
#include "physicalDevice.h"
#include "instance.h"
#include "surface.h"
#include "queue.h"
#include "fence.h"
#include "timelineSemaphore.h"
#include "deviceResourcePool.h"
#include "../misc/featureQuery.h"
#include "../raytracing/accelerationStructureHeader.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"
#include "../misc/extension.h"

namespace magma
{
Device::Device(std::shared_ptr<PhysicalDevice> physicalDevice_,
    const std::vector<DeviceQueueDescriptor>& queueDescriptors_,
    const NullTerminatedStringArray& enabledLayers_,
    const NullTerminatedStringArray& enabledExtensions_,
    const VkPhysicalDeviceFeatures& enabledFeatures_,
    const StructureChain& enabledExtendedFeatures_,
    const StructureChain& extendedInfo,
    std::shared_ptr<IAllocator> allocator):
    Dispatchable<VkDevice>(VK_OBJECT_TYPE_DEVICE, std::move(allocator)),
    physicalDevice(std::move(physicalDevice_)),
    queueDescriptors(queueDescriptors_),
    enabledFeatures(enabledFeatures_),
    enabledExtendedFeatures(enabledExtendedFeatures_)
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
   ,resourcePool(std::make_unique<DeviceResourcePool>())
#endif
{
    VkDeviceCreateInfo deviceInfo;
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = extendedInfo.headNode();
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = core::countof(queueDescriptors);
    deviceInfo.pQueueCreateInfos = queueDescriptors.data();
    deviceInfo.enabledLayerCount = core::countof(enabledLayers_);
    deviceInfo.ppEnabledLayerNames = enabledLayers_.data();
    deviceInfo.enabledExtensionCount = core::countof(enabledExtensions_);
    deviceInfo.ppEnabledExtensionNames = enabledExtensions_.data();
    deviceInfo.pEnabledFeatures = enabledExtendedFeatures.empty() ? &enabledFeatures : nullptr;
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceFeatures2KHR deviceFeatures2;
    if (!enabledExtendedFeatures.empty())
    {
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
        deviceFeatures2.pNext = (void *)enabledExtendedFeatures.headNode();
        deviceFeatures2.features = enabledFeatures;
        if (!deviceInfo.pNext)
            deviceInfo.pNext = &deviceFeatures2;
        else
        {   // Link extended features as tail node
            auto *tailNode = (VkBaseInStructure *)extendedInfo.tailNode();
            tailNode->pNext = (const VkBaseInStructure *)&deviceFeatures2;
        }
    }
#endif // VK_KHR_get_physical_device_properties2
    const VkResult result = vkCreateDevice(*physicalDevice, &deviceInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
#ifndef MAGMA_NO_EXCEPTIONS
    switch (result)
    {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("initialization of logical device failed");
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        for (const char *extension: enabledExtensions_)
        {
            if (!physicalDevice->extensionSupported(extension))
                throw exception::ExtensionNotPresent(extension);
        }
        break;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        throw exception::FeatureNotPresent(enabledFeatures, physicalDevice->getFeatures());
    default:
        MAGMA_HANDLE_RESULT(result, "failed to create logical device");
    }
#endif // !MAGMA_NO_EXCEPTIONS
    MAGMA_HANDLE_RESULT(result, "failed to create logical device");
    // Store enabled layers and extensions
    for (auto const& layer: enabledLayers_)
        enabledLayers.emplace(layer);
    for (auto const& extension: enabledExtensions_)
        enabledExtensions.emplace(extension);
    // Store feature nodes for fast search in getEnabledExtendedFeatures()
    const VkBaseInStructure *featureNode = enabledExtendedFeatures.headNode();
    while (featureNode)
    {
        extendedFeatures[featureNode->sType] = featureNode;
        featureNode = featureNode->pNext;
    }
}

Device::~Device()
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    MAGMA_ASSERT(!resourcePool->hasUnreleasedResources());
#endif
    vkDestroyDevice(handle, MAGMA_OPTIONAL(hostAllocator));
}

const std::unique_ptr<FeatureQuery>& Device::checkFeatures() const
{
    if (!featureQuery)
        featureQuery = FeatureQuery::makeUnique(this);
    return featureQuery;
}

std::shared_ptr<Queue> Device::getQueue(VkQueueFlagBits flags, uint32_t queueIndex /* 0 */) const
{
    const DeviceQueueDescriptor queueDescriptor(physicalDevice.get(), flags, QueuePriorityDefault);
    return getQueueByFamily(queueDescriptor.queueFamilyIndex, queueIndex);
}

std::shared_ptr<Queue> Device::getQueueByFamily(uint32_t queueFamilyIndex, uint32_t queueIndex /* 0 */) const
{   // Call vkGetDeviceQueue() only if logical device has been created
    // with this queue family, otherwise call will throw an exception.
    if (supportsQueueFamily(queueFamilyIndex, queueIndex))
    {   // Try to get cached queue
        auto const key = std::make_pair(queueFamilyIndex, queueIndex);
        auto it = queues.find(key);
        if (it != queues.end())
            return it->second;
        for (const VkQueueFlagBits flag: {
            VK_QUEUE_GRAPHICS_BIT,
            VK_QUEUE_COMPUTE_BIT,
            VK_QUEUE_TRANSFER_BIT})
        {   // Try to get new instance
            const DeviceQueueDescriptor queueDescriptor(physicalDevice.get(), flag);
            if (queueDescriptor.queueFamilyIndex == queueFamilyIndex)
            {
                VkQueue queue = VK_NULL_HANDLE;
                vkGetDeviceQueue(handle, queueFamilyIndex, queueIndex, &queue);
                if (VK_NULL_HANDLE == queue)
                    MAGMA_ERROR("failed to get device queue");
                return queues[key] = Queue::makeShared(queue, flag, queueFamilyIndex, queueIndex);
            }
        }
    }
    return nullptr;
}

void Device::updateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet *descriptorWrites,
    uint32_t descriptorCopyCount, const VkCopyDescriptorSet *descriptorCopies) const noexcept
{
    vkUpdateDescriptorSets(handle, descriptorWriteCount, descriptorWrites, descriptorCopyCount, descriptorCopies);
}

void Device::updateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites,
    const std::vector<VkCopyDescriptorSet>& descriptorCopies /* {} */) const noexcept
{
    updateDescriptorSets(core::countof(descriptorWrites), descriptorWrites.data(), core::countof(descriptorCopies), descriptorCopies.data());
}

void Device::waitIdle() const
{
    const VkResult result = vkDeviceWaitIdle(handle);
    MAGMA_HANDLE_RESULT(result, "failed to wait for device to become idle");
    if (VK_SUCCESS == result)
    {
        for (auto& [key, queue]: queues)
            key, queue->onIdle();
    }
}

bool Device::resetFences(std::vector<lent_ptr<Fence>>& fences) const noexcept
{
    auto dereferencedFences = stackalloc(VkFence, fences.size());
    uint32_t fenceCount = 0;
    for (auto const& fence: fences)
        dereferencedFences[fenceCount++] = *fence;
    const VkResult result = vkResetFences(handle, fenceCount, dereferencedFences);
    return (VK_SUCCESS == result);
}

bool Device::waitForFences(const std::vector<lent_ptr<Fence>>& fences, bool waitAll,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */) const
{
    auto dereferencedFences = stackalloc(VkFence, fences.size());
    uint32_t fenceCount = 0;
    for (auto const& fence: fences)
        dereferencedFences[fenceCount++] = *fence;
    const VkResult result = vkWaitForFences(handle, fenceCount, dereferencedFences, MAGMA_BOOLEAN(waitAll), timeout);
    MAGMA_HANDLE_RESULT(result, "failed to wait for fence(s)");
    // VK_SUCCESS or VK_TIMEOUT
    return (result != VK_TIMEOUT);
}

#ifdef VK_KHR_timeline_semaphore
bool Device::waitSemaphores(const std::vector<lent_ptr<TimelineSemaphore>>& semaphores,
    const std::vector<uint64_t>& values, bool waitAll,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */) const
{
    auto dereferencedSemaphores = stackalloc(VkSemaphore, semaphores.size());
    uint32_t semaphoreCount = 0;
    for (auto const& semaphore: semaphores)
        dereferencedSemaphores[semaphoreCount++] = *semaphore;
    VkSemaphoreWaitInfo waitInfo;
    waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO_KHR;
    waitInfo.pNext = nullptr;
    waitInfo.flags = waitAll ? 0 : VK_SEMAPHORE_WAIT_ANY_BIT_KHR;
    waitInfo.semaphoreCount = semaphoreCount;
    waitInfo.pSemaphores = dereferencedSemaphores;
    waitInfo.pValues = values.data();
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkWaitSemaphoresKHR, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkWaitSemaphoresKHR(handle, &waitInfo, timeout);
    MAGMA_HANDLE_RESULT(result, "failed to wait for timeline semaphore(s)");
    // VK_SUCCESS or VK_TIMEOUT
    return (result != VK_TIMEOUT);
}
#endif // VK_KHR_timeline_semaphore

#ifdef VK_KHR_maintenance3
bool Device::getDescriptorSetLayoutSupport(const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */) const
{
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
    VkDescriptorSetLayoutSupportKHR descriptorSetLayoutSupport;
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.pNext = extendedInfo.headNode();
    descriptorSetLayoutInfo.flags = flags;
    descriptorSetLayoutInfo.bindingCount = core::countof(bindings);
    descriptorSetLayoutInfo.pBindings = bindings.data();
    descriptorSetLayoutSupport.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT_KHR;
    descriptorSetLayoutSupport.pNext = nullptr;
    descriptorSetLayoutSupport.supported = VK_FALSE;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDescriptorSetLayoutSupportKHR, VK_KHR_MAINTENANCE3_EXTENSION_NAME);
    vkGetDescriptorSetLayoutSupportKHR(handle, &descriptorSetLayoutInfo, &descriptorSetLayoutSupport);
    return descriptorSetLayoutSupport.supported;
}

#ifdef VK_EXT_descriptor_indexing
uint32_t Device::getVariableDescriptorCountLayoutSupport(const std::vector<VkDescriptorSetLayoutBinding>& bindings,
    VkDescriptorSetLayoutCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */) const
{
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
    VkDescriptorSetLayoutSupportKHR descriptorSetLayoutSupport;
    VkDescriptorSetVariableDescriptorCountLayoutSupportEXT descriptorSetVariableDescriptorCountLayoutSupport;
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.pNext = extendedInfo.headNode();
    descriptorSetLayoutInfo.flags = flags;
    descriptorSetLayoutInfo.bindingCount = core::countof(bindings);
    descriptorSetLayoutInfo.pBindings = bindings.data();
    descriptorSetLayoutSupport.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT_KHR;
    descriptorSetLayoutSupport.pNext = &descriptorSetVariableDescriptorCountLayoutSupport;
    descriptorSetLayoutSupport.supported = VK_FALSE;
    descriptorSetVariableDescriptorCountLayoutSupport.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT_EXT;
    descriptorSetVariableDescriptorCountLayoutSupport.pNext = nullptr;
    descriptorSetVariableDescriptorCountLayoutSupport.maxVariableDescriptorCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDescriptorSetLayoutSupportKHR, VK_KHR_MAINTENANCE3_EXTENSION_NAME);
    vkGetDescriptorSetLayoutSupportKHR(handle, &descriptorSetLayoutInfo, &descriptorSetLayoutSupport);
    return descriptorSetVariableDescriptorCountLayoutSupport.maxVariableDescriptorCount;
}
#endif // VK_EXT_descriptor_indexing
#endif // VK_KHR_maintenance3

#ifdef VK_KHR_performance_query
void Device::acquireProfilingLock(VkAcquireProfilingLockFlagsKHR flags /* 0 */,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */)
{
    VkAcquireProfilingLockInfoKHR acquireProfilingLockInfo;
    acquireProfilingLockInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_PROFILING_LOCK_INFO_KHR;
    acquireProfilingLockInfo.pNext = nullptr;
    acquireProfilingLockInfo.flags = flags;
    acquireProfilingLockInfo.timeout = timeout;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkAcquireProfilingLockKHR, VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
    const VkResult result = vkAcquireProfilingLockKHR(handle, &acquireProfilingLockInfo);
    MAGMA_HANDLE_RESULT(result, "failed to acquire profiling lock");
}

void Device::releaseProfilingLock()
{
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkReleaseProfilingLockKHR, VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
    vkReleaseProfilingLockKHR(handle);
}
#endif // VK_KHR_performance_query

#ifdef VK_KHR_device_group
VkDeviceGroupPresentCapabilitiesKHR Device::getDeviceGroupPresentCapabilitiesKHR() const
{
    VkDeviceGroupPresentCapabilitiesKHR deviceGroupPresentCapabitilies = {};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupPresentCapabilitiesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupPresentCapabilitiesKHR(handle, &deviceGroupPresentCapabitilies);
    MAGMA_HANDLE_RESULT(result, "failed to get present capabilities of device group");
    return deviceGroupPresentCapabitilies;
}

VkDeviceGroupPresentModeFlagsKHR Device::getDeviceGroupSurfacePresentModes(lent_ptr<const Surface> surface) const
{
    VkDeviceGroupPresentModeFlagsKHR presentModes = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupSurfacePresentModesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupSurfacePresentModesKHR(handle, *surface, &presentModes);
    MAGMA_HANDLE_RESULT(result, "failed to get surface present modes of device group");
    return presentModes;
}

#ifdef VK_EXT_full_screen_exclusive
VkDeviceGroupPresentModeFlagsKHR Device::getDeviceGroupFullScreenExclusiveSurfacePresentModes(lent_ptr<const Surface> surface,
    VkFullScreenExclusiveEXT fullScreenExclusive
#ifdef VK_KHR_win32_surface
   ,HMONITOR hMonitor /* NULL */
#endif
    ) const
{
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveInfo;
    fullScreenExclusiveInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
    fullScreenExclusiveInfo.pNext = nullptr;
    fullScreenExclusiveInfo.fullScreenExclusive = fullScreenExclusive;
#ifdef VK_KHR_win32_surface
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32Info;
    if (hMonitor)
    {
        fullScreenExclusiveInfo.pNext = &fullScreenExclusiveWin32Info;
        fullScreenExclusiveWin32Info.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
        fullScreenExclusiveWin32Info.pNext = nullptr;
        fullScreenExclusiveWin32Info.hmonitor = hMonitor;
    }
#endif // VK_KHR_win32_surface
    VkDeviceGroupPresentModeFlagsKHR presentModes = 0;
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR physicalDeviceSurfaceInfo;
    physicalDeviceSurfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    physicalDeviceSurfaceInfo.pNext = &fullScreenExclusiveInfo;
    physicalDeviceSurfaceInfo.surface = *surface;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupSurfacePresentModes2EXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupSurfacePresentModes2EXT(handle, &physicalDeviceSurfaceInfo, &presentModes);
    MAGMA_HANDLE_RESULT(result, "failed to get full-screen exclusive surface present modes of device group");
#endif // VK_KHR_get_surface_capabilities2
    return presentModes;
}
#endif // VK_EXT_full_screen_exclusive

VkPeerMemoryFeatureFlags Device::getDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex) const
{
    VkPeerMemoryFeatureFlags peerMemoryFeatures;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupPeerMemoryFeaturesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    vkGetDeviceGroupPeerMemoryFeaturesKHR(handle, heapIndex, localDeviceIndex, remoteDeviceIndex, &peerMemoryFeatures);
    return peerMemoryFeatures;
}
#endif // VK_KHR_device_group

#ifdef VK_KHR_acceleration_structure
bool Device::getAccelerationStructureCompatibility(const AccelerationStructureHeader *header) const
{
    VkAccelerationStructureVersionInfoKHR versionInfo;
    versionInfo.sType =  VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_VERSION_INFO_KHR;
    versionInfo.pNext = nullptr;
    versionInfo.pVersionData = header->driverUUID;
    VkAccelerationStructureCompatibilityKHR compatibility = VK_ACCELERATION_STRUCTURE_COMPATIBILITY_INCOMPATIBLE_KHR;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceAccelerationStructureCompatibilityKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    vkGetDeviceAccelerationStructureCompatibilityKHR(handle, &versionInfo, &compatibility);
    return (VK_ACCELERATION_STRUCTURE_COMPATIBILITY_COMPATIBLE_KHR == compatibility);
}
#endif // VK_KHR_acceleration_structure

#ifdef VK_EXT_calibrated_timestamps
std::vector<uint64_t> Device::getCalibratedTimestamps(const std::vector<VkTimeDomainEXT>& timeDomains,
    uint64_t *maxDeviation /* nullptr */) const
{
    auto calibratedTimestampInfos = stackalloc(VkCalibratedTimestampInfoEXT, timeDomains.size());
    uint32_t timestampCount = 0;
    for (VkTimeDomainEXT timeDomain: timeDomains)
    {
        VkCalibratedTimestampInfoEXT& info = calibratedTimestampInfos[timestampCount++];
        info.sType = VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT;
        info.pNext = nullptr;
        info.timeDomain = timeDomain;
    }
    std::vector<uint64_t> timestamps(timeDomains.size(), 0ull);
    uint64_t deviation = 0ull;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetCalibratedTimestampsEXT, VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME);
    vkGetCalibratedTimestampsEXT(handle, timestampCount, calibratedTimestampInfos, timestamps.data(),
        maxDeviation ? maxDeviation : &deviation);
    return timestamps;
}
#endif // VK_EXT_calibrated_timestamps

#ifdef VK_EXT_device_fault
DeviceFaultInfo Device::getFaultInfo() const
{
    VkDeviceFaultCountsEXT deviceFaultCounts;
    deviceFaultCounts.sType = VK_STRUCTURE_TYPE_DEVICE_FAULT_COUNTS_EXT;
    deviceFaultCounts.pNext = nullptr;
    deviceFaultCounts.addressInfoCount = 0;
    deviceFaultCounts.vendorInfoCount = 0;
    deviceFaultCounts.vendorBinarySize = 0ull;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceFaultInfoEXT, VK_EXT_DEVICE_FAULT_EXTENSION_NAME);
    VkResult result = vkGetDeviceFaultInfoEXT(handle, &deviceFaultCounts, nullptr);
    if (MAGMA_SUCCEEDED(result))
    {
        DeviceFaultInfo deviceFaultInfo(deviceFaultCounts);
        result = vkGetDeviceFaultInfoEXT(handle, &deviceFaultCounts, &deviceFaultInfo);
        if (MAGMA_SUCCEEDED(result))
            return deviceFaultInfo;
    }
    MAGMA_HANDLE_RESULT(result, "failed to get device fault info");
    return DeviceFaultInfo();
}
#endif // VK_EXT_device_fault

bool Device::extensionEnabled(const char *extensionName) const noexcept
{
    if (!physicalDevice->extensionSupported(extensionName))
        return false;
    const auto it = enabledExtensions.find(extensionName);
    return it != enabledExtensions.end();
}

VkInstance Device::getNativeInstance() const noexcept
{
    return physicalDevice->getInstance()->getHandle();
}

bool Device::supportsQueueFamily(uint32_t queueFamilyIndex, uint32_t queueIndex) const noexcept
{
    auto it = std::find_if(queueDescriptors.begin(), queueDescriptors.end(),
        [queueFamilyIndex](const DeviceQueueDescriptor& descriptor) {
            return descriptor.queueFamilyIndex == queueFamilyIndex;
        });
    if (it != queueDescriptors.end())
        return queueIndex < it->queueCount;
    return false;
}
} // namespace magma
