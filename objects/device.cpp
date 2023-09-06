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
#include "pch.h"
#pragma hdrstop
#include "device.h"
#include "physicalDevice.h"
#include "surface.h"
#include "queue.h"
#include "fence.h"
#include "privateDataSlot.h"
#include "timelineSemaphore.h"
#include "resourcePool.h"
#include "../misc/deviceFeatures.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"

// Redefine macro because here we use handle directly
// instead of using circular reference to itself
#ifdef MAGMA_HANDLE
#undef MAGMA_HANDLE
#define MAGMA_HANDLE(p) handle
#endif
#include "../misc/extProcAddress.h"

namespace magma
{
Device::Device(std::shared_ptr<PhysicalDevice> physicalDevice_,
    const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& enabledLayers_,
    const std::vector<const char *>& enabledExtensions_,
    const VkPhysicalDeviceFeatures& enabledFeatures_,
    const StructureChain& enabledExtendedFeatures_,
    const StructureChain& extendedInfo,
    std::shared_ptr<IAllocator> allocator):
    Dispatchable<VkDevice>(VK_OBJECT_TYPE_DEVICE, std::move(allocator)),
    physicalDevice(std::move(physicalDevice_)),
    resourcePool(std::make_shared<ResourcePool>()),
    enabledFeatures(enabledFeatures_),
    enabledExtendedFeatures(enabledExtendedFeatures_)
{
    VkDeviceCreateInfo deviceInfo;
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = extendedInfo.chainNodes();
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = MAGMA_COUNT(queueDescriptors);
    deviceInfo.pQueueCreateInfos = queueDescriptors.data();
    deviceInfo.enabledLayerCount = MAGMA_COUNT(enabledLayers_);
    deviceInfo.ppEnabledLayerNames = enabledLayers_.data();
    deviceInfo.enabledExtensionCount = MAGMA_COUNT(enabledExtensions_);
    deviceInfo.ppEnabledExtensionNames = enabledExtensions_.data();
    deviceInfo.pEnabledFeatures = enabledExtendedFeatures.empty() ? &enabledFeatures : nullptr;
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceFeatures2KHR deviceFeatures2;
    if (!enabledExtendedFeatures.empty())
    {
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
        deviceFeatures2.features = enabledFeatures;
        deviceFeatures2.pNext = (void *)enabledExtendedFeatures.chainNodes();
        if (!deviceInfo.pNext)
            deviceInfo.pNext = &deviceFeatures2;
        else
        {   // Link extended features as last node
            VkNode *lastNode = (VkNode *)extendedInfo.lastNode();
            lastNode->pNext = &deviceFeatures2;
        }
    }
#endif // VK_KHR_get_physical_device_properties2
    const VkResult result = vkCreateDevice(*physicalDevice, &deviceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
#ifndef MAGMA_NO_EXCEPTIONS
    if (VK_ERROR_INITIALIZATION_FAILED == result)
        throw exception::InitializationFailed("failed to create logical device");
#endif // !MAGMA_NO_EXCEPTIONS
    MAGMA_THROW_FAILURE(result, "failed to create logical device");
    queues.reserve(queueDescriptors.size());
    for (const auto& desc : queueDescriptors)
        queues.emplace_back(desc, std::weak_ptr<Queue>());
    // Store enabled layers and extensions
    for (const auto& layer: enabledLayers_)
        enabledLayers.emplace(layer);
    for (const auto& extension: enabledExtensions_)
        enabledExtensions.emplace(extension);
    // Store feature nodes for fast search in getEnabledExtendedFeatures()
    const VkBaseInStructure *featureNode = enabledExtendedFeatures.firstNode();
    while (featureNode)
    {
        extendedFeatures[featureNode->sType] = featureNode;
        featureNode = featureNode->pNext;
    }
}

Device::~Device()
{
    MAGMA_ASSERT(resourcePool->hasAnyDeviceResource() == false);
    vkDestroyDevice(handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

const std::shared_ptr<DeviceFeatures>& Device::getDeviceFeatures() const noexcept
{
    if (!features)
        features = std::shared_ptr<DeviceFeatures>(new DeviceFeatures(shared_from_this()));
    return features;
}

std::shared_ptr<Queue> Device::getQueue(VkQueueFlagBits flags, uint32_t queueIndex) const
{
    const DeviceQueueDescriptor queueDesc(physicalDevice, flags);
    for (auto& pair : queues)
    {   // Check if queue family is present, otherwise vkGetDeviceQueue() throws an exception
        if (pair.first.queueFamilyIndex == queueDesc.queueFamilyIndex)
        {
            if (!pair.second.expired())
                return pair.second.lock();
            // Get queue that supports specified flags
            VkQueue queueHandle = VK_NULL_HANDLE;
            vkGetDeviceQueue(handle, queueDesc.queueFamilyIndex, queueIndex, &queueHandle);
            if (VK_NULL_HANDLE == queueHandle)
                MAGMA_THROW("failed to get device queue");
            auto queue = std::shared_ptr<Queue>(new Queue(queueHandle,
                std::const_pointer_cast<Device>(shared_from_this()),
                flags, queueDesc.queueFamilyIndex, queueIndex));
            // Cache using weak_ptr to break circular references
            pair.second = queue;
            return queue;
        }
    }
    MAGMA_THROW("failed to get device queue");
}

std::shared_ptr<Queue> Device::getQueueForFamily(uint32_t queueFamilyIndex) const
{
    auto it = std::find_if(queues.begin(), queues.end(),
        [queueFamilyIndex](const auto& pair)
        {
            return (pair.first.queueFamilyIndex == queueFamilyIndex) &&
                !pair.second.expired();
        });
    if (it != queues.end())
        return it->second.lock();
    for (auto flag: {
        VK_QUEUE_GRAPHICS_BIT,
        VK_QUEUE_COMPUTE_BIT,
        VK_QUEUE_TRANSFER_BIT})
    {   // Try to get new instance
        try
        {
            auto queue = device->getQueue(flag, 0);
            if (queue->getFamilyIndex() == queueFamilyIndex)
                return queue;
        } catch (...) {}
    }
    MAGMA_THROW("failed to get device queue");
}

void Device::updateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet *descriptorWrites,
    uint32_t descriptorCopyCount, const VkCopyDescriptorSet *descriptorCopies) const noexcept
{
    vkUpdateDescriptorSets(handle, descriptorWriteCount, descriptorWrites, descriptorCopyCount, descriptorCopies);
}

void Device::updateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites,
    const std::vector<VkCopyDescriptorSet>& descriptorCopies /* {} */) const noexcept
{
    updateDescriptorSets(MAGMA_COUNT(descriptorWrites), descriptorWrites.data(), MAGMA_COUNT(descriptorCopies), descriptorCopies.data());
}

bool Device::waitIdle() const
{
    const VkResult result = vkDeviceWaitIdle(handle);
    MAGMA_THROW_FAILURE(result, "failed to wait for device become idle");
    return true;
}

bool Device::resetFences(std::vector<std::shared_ptr<Fence>>& fences) const noexcept
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult result = vkResetFences(handle, dereferencedFences.size(), dereferencedFences);
    return (VK_SUCCESS == result);
}

bool Device::waitForFences(const std::vector<std::shared_ptr<Fence>>& fences, bool waitAll,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */) const
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult result = vkWaitForFences(handle, dereferencedFences.size(), dereferencedFences, MAGMA_BOOLEAN(waitAll), timeout);
    MAGMA_THROW_FAILURE(result, "failed to wait for fences");
    // VK_SUCCESS or VK_TIMEOUT
    return (result != VK_TIMEOUT);
}

#ifdef VK_KHR_timeline_semaphore
bool Device::waitSemaphores(const std::vector<std::shared_ptr<TimelineSemaphore>>& semaphores,
    const std::vector<uint64_t>& values, bool waitAll,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */) const
{
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedSemaphores, semaphores.size());
    for (const auto& semaphore: semaphores)
        dereferencedSemaphores.put(*semaphore);
    VkSemaphoreWaitInfo waitInfo;
    waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO_KHR;
    waitInfo.pNext = nullptr;
    waitInfo.flags = waitAll ? 0 : VK_SEMAPHORE_WAIT_ANY_BIT_KHR;
    waitInfo.semaphoreCount = dereferencedSemaphores.size();
    waitInfo.pSemaphores = dereferencedSemaphores;
    waitInfo.pValues = values.data();
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkWaitSemaphoresKHR, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkWaitSemaphoresKHR(handle, &waitInfo, timeout);
    MAGMA_THROW_FAILURE(result, "failed to wait timeline semaphores");
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
    descriptorSetLayoutInfo.pNext = extendedInfo.chainNodes();
    descriptorSetLayoutInfo.flags = flags;
    descriptorSetLayoutInfo.bindingCount = MAGMA_COUNT(bindings);
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
    descriptorSetLayoutInfo.pNext = extendedInfo.chainNodes();
    descriptorSetLayoutInfo.flags = flags;
    descriptorSetLayoutInfo.bindingCount = MAGMA_COUNT(bindings);
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

#ifdef VK_KHR_device_group
VkDeviceGroupPresentCapabilitiesKHR Device::getDeviceGroupPresentCapabilitiesKHR() const
{
    VkDeviceGroupPresentCapabilitiesKHR presentCapabitilies = {};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupPresentCapabilitiesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupPresentCapabilitiesKHR(handle, &presentCapabitilies);
    MAGMA_THROW_FAILURE(result, "failed to get device present capabilities for a device group");
    return presentCapabitilies;
}

VkDeviceGroupPresentModeFlagsKHR Device::getDeviceGroupSurfacePresentModes(std::shared_ptr<const Surface> surface) const
{
    VkDeviceGroupPresentModeFlagsKHR presentModes = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupSurfacePresentModesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupSurfacePresentModesKHR(handle, *surface, &presentModes);
    MAGMA_THROW_FAILURE(result, "failed to get surface present modes for a device group");
    return presentModes;
}

#ifdef VK_EXT_full_screen_exclusive
VkDeviceGroupPresentModeFlagsKHR Device::getDeviceGroupSurfaceFullScreenExclusivePresentModes(std::shared_ptr<const Surface> surface,
    VkFullScreenExclusiveEXT fullScreenExclusive, void *hMonitor /* nullptr */) const
{
    VkDeviceGroupPresentModeFlagsKHR presentModes = 0;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32SurfaceInfo;
    fullScreenExclusiveWin32SurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
    fullScreenExclusiveWin32SurfaceInfo.pNext = nullptr;
    fullScreenExclusiveWin32SurfaceInfo.hmonitor = reinterpret_cast<HMONITOR>(hMonitor);
#endif // VK_USE_PLATFORM_WIN32_KHR
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveSurfaceInfo;
    fullScreenExclusiveSurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    fullScreenExclusiveSurfaceInfo.pNext = hMonitor ? &fullScreenExclusiveWin32SurfaceInfo : nullptr;
#else
    fullScreenExclusiveSurfaceInfo.pNext = nullptr;
    MAGMA_UNUSED(hMonitor);
#endif
    fullScreenExclusiveSurfaceInfo.fullScreenExclusive = fullScreenExclusive;
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = &fullScreenExclusiveSurfaceInfo;
    surfaceInfo.surface = *surface;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetDeviceGroupSurfacePresentModes2EXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModes);
    MAGMA_THROW_FAILURE(result, "failed to get full-screen exclusive surface present modes for a device group");
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

#ifdef VK_EXT_calibrated_timestamps
std::vector<uint64_t> Device::getCalibratedTimestamps(const std::vector<VkTimeDomainEXT>& timeDomains,
    uint64_t *maxDeviation /* nullptr */) const
{
    MAGMA_STACK_ARRAY(VkCalibratedTimestampInfoEXT, calibratedTimestampInfos, timeDomains.size());
    VkCalibratedTimestampInfoEXT calibratedTimestampInfo;
    calibratedTimestampInfo.sType = VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_EXT;
    calibratedTimestampInfo.pNext = nullptr;
    for (VkTimeDomainEXT timeDomain: timeDomains)
    {
        calibratedTimestampInfo.timeDomain = timeDomain;
        calibratedTimestampInfos.put(calibratedTimestampInfo);
    }
    std::vector<uint64_t> timestamps(timeDomains.size(), 0ull);
    uint64_t deviation = 0ull;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetCalibratedTimestampsEXT, VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME);
    vkGetCalibratedTimestampsEXT(handle, MAGMA_COUNT(calibratedTimestampInfos), calibratedTimestampInfos, timestamps.data(),
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
    MAGMA_THROW_FAILURE(result, "failed to get device fault counts");
    DeviceFaultInfo deviceFaultInfo(deviceFaultCounts);
    result = vkGetDeviceFaultInfoEXT(handle, &deviceFaultCounts, &deviceFaultInfo);
    MAGMA_THROW_FAILURE(result, "failed to get device fault info");
    return deviceFaultInfo;
}
#endif // VK_EXT_device_fault

bool Device::extensionEnabled(const char *extensionName) const noexcept
{
    if (!physicalDevice->extensionSupported(extensionName))
        return false;
    const auto it = enabledExtensions.find(extensionName);
    return it != enabledExtensions.end();
}
} // namespace magma
