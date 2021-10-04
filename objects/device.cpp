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
#include "pch.h"
#pragma hdrstop
#include "device.h"
#include "physicalDevice.h"
#include "surface.h"
#include "queue.h"
#include "fence.h"
#include "resourcePool.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"

namespace magma
{
Device::Device(std::shared_ptr<PhysicalDevice> physicalDevice,
    const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& enabledLayers,
    const std::vector<const char *>& enabledExtensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures,
    const CreateInfo& chainedCreateInfo,
    std::shared_ptr<IAllocator> allocator):
    Dispatchable<VkDevice>(VK_OBJECT_TYPE_DEVICE, nullptr, std::move(allocator)),
    physicalDevice(std::move(physicalDevice)),
    resourcePool(std::make_shared<ResourcePool>()),
    enabledFeatures(deviceFeatures)
#ifdef VK_KHR_get_physical_device_properties2
    ,enabledExtendedFeatures(extendedDeviceFeatures)
#endif
{
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceFeatures2KHR features;
    if (!extendedDeviceFeatures.empty())
    {
        features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
        features.features = deviceFeatures;
        features.pNext = extendedDeviceFeatures.front();
        struct VkFeaturesNode
        {
            VkStructureType sType;
            const void *pNext;
            // ...
        };
        auto curr = extendedDeviceFeatures.begin();
        auto next = curr; ++next;
        while (next != extendedDeviceFeatures.end())
        {   // Make linked list
            VkFeaturesNode *currNode = reinterpret_cast<VkFeaturesNode *>(*curr);
            VkFeaturesNode *nextNode = reinterpret_cast<VkFeaturesNode *>(*next);
            currNode->pNext = nextNode;
            ++curr; ++next;
        }
        VkFeaturesNode *lastNode = reinterpret_cast<VkFeaturesNode *>(*curr);
        lastNode->pNext = chainedCreateInfo.getNode();
    }
#endif // VK_KHR_get_physical_device_properties2
    VkDeviceCreateInfo deviceInfo;
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
#ifdef VK_KHR_get_physical_device_properties2
    deviceInfo.pNext = extendedDeviceFeatures.empty() ? chainedCreateInfo.getNode() : &features;
#else
    deviceInfo.pNext = chainedCreateInfo.getNode();
#endif
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = MAGMA_COUNT(queueDescriptors);
    deviceInfo.pQueueCreateInfos = queueDescriptors.data();
    deviceInfo.enabledLayerCount = MAGMA_COUNT(enabledLayers);
    deviceInfo.ppEnabledLayerNames = enabledLayers.data();
    deviceInfo.enabledExtensionCount = MAGMA_COUNT(enabledExtensions);
    deviceInfo.ppEnabledExtensionNames = enabledExtensions.data();
    deviceInfo.pEnabledFeatures = extendedDeviceFeatures.empty() ? &deviceFeatures : nullptr;
    const VkResult result = vkCreateDevice(MAGMA_HANDLE(physicalDevice), &deviceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    switch (result)
    {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("failed to create logical device");
    case VK_ERROR_DEVICE_LOST:
        throw exception::DeviceLost("failed to create logical device");
    }
    MAGMA_THROW_FAILURE(result, "failed to create logical device");
    queues.reserve(queueDescriptors.size());
    for (const auto& desc : queueDescriptors)
        queues.emplace_back(desc, std::weak_ptr<Queue>());
    // Store enabled layers and extensions
    for (const auto& layer : enabledLayers)
        this->enabledLayers.emplace_back(layer);
    for (const auto& extension : enabledExtensions)
        this->enabledExtensions.emplace_back(extension);
}

Device::~Device()
{
    MAGMA_ASSERT(resourcePool->hasAnyDeviceResource() == false);
    vkDestroyDevice(handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
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
            VkQueue queue = VK_NULL_HANDLE;
            vkGetDeviceQueue(handle, queueDesc.queueFamilyIndex, queueIndex, &queue);
            if (VK_NULL_HANDLE == queue)
                MAGMA_THROW("failed to get device queue");
            auto queueObj = std::shared_ptr<Queue>(new Queue(queue,
                std::const_pointer_cast<Device>(shared_from_this()),
                flags, queueDesc.queueFamilyIndex, queueIndex));
            // Cache using weak_ptr to break circular references
            pair.second = queueObj;
            return queueObj;
        }
    }
    MAGMA_THROW("failed to get device queue");
}

void Device::updateDescriptorWrites(const helpers::StackArray<VkWriteDescriptorSet>& descriptorWrites) const noexcept
{
    vkUpdateDescriptorSets(handle, MAGMA_COUNT(descriptorWrites), descriptorWrites, 0, nullptr);
}

void Device::updateDescriptorCopies(const helpers::StackArray<VkCopyDescriptorSet>& descriptorCopies) const noexcept
{
    vkUpdateDescriptorSets(handle, 0, nullptr, MAGMA_COUNT(descriptorCopies), descriptorCopies);
}

void Device::updateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites,
    const std::vector<VkCopyDescriptorSet>& descriptorCopies /* {} */) const noexcept
{
    vkUpdateDescriptorSets(handle,
        MAGMA_COUNT(descriptorWrites), descriptorWrites.data(),
        MAGMA_COUNT(descriptorCopies), descriptorCopies.data());
}

bool Device::waitIdle() const
{
    const VkResult result = vkDeviceWaitIdle(handle);
    if (VK_ERROR_DEVICE_LOST == result)
        throw exception::DeviceLost("failed to wait for device become idle");
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

bool Device::waitForFences(std::vector<std::shared_ptr<Fence>>& fences, bool waitAll,
    uint64_t timeout /* UINT64_MAX */) const noexcept
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult result = vkWaitForFences(handle, dereferencedFences.size(), dereferencedFences,
        MAGMA_BOOLEAN(waitAll), timeout);
    return (VK_SUCCESS == result) || (VK_TIMEOUT == result);
}

#ifdef VK_KHR_device_group
VkDeviceGroupPresentModeFlagsKHR Device::getDeviceGroupSurfacePresentModes(std::shared_ptr<const Surface> surface) const
{
    VkDeviceGroupPresentModeFlagsKHR presentModes;
    MAGMA_DEVICE_EXTENSION(vkGetDeviceGroupSurfacePresentModesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
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
    MAGMA_DEVICE_EXTENSION(vkGetDeviceGroupSurfacePresentModes2EXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    const VkResult result = vkGetDeviceGroupSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModes);
    MAGMA_THROW_FAILURE(result, "failed to get full-screen exclusive surface present modes for a device group");
#endif // VK_KHR_get_surface_capabilities2
    return presentModes;
}
#endif // VK_EXT_full_screen_exclusive

VkPeerMemoryFeatureFlags Device::getDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex) const
{
    VkPeerMemoryFeatureFlags peerMemoryFeatures;
    MAGMA_DEVICE_EXTENSION(vkGetDeviceGroupPeerMemoryFeaturesKHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    vkGetDeviceGroupPeerMemoryFeaturesKHR(handle, heapIndex, localDeviceIndex, remoteDeviceIndex, &peerMemoryFeatures);
    return peerMemoryFeatures;
}
#endif // VK_KHR_device_group

bool Device::extensionEnabled(const char *extensionName) const
{
    MAGMA_ASSERT(extensionName);
    if (physicalDevice->checkExtensionSupport(extensionName))
    {
        for (const auto& extension : enabledExtensions)
        {
            if (extension == extensionName)
                return true;
        }
    }
    return false;
}

bool Device::separateDepthStencilLayoutsEnabled() const noexcept
{
#ifdef VK_KHR_separate_depth_stencil_layouts
    const void *features = findExtendedFeatures(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES_KHR);
    if (features)
    {
        const VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR *separateDepthStencilFeatures =
            reinterpret_cast<const VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR *>(features);
        return separateDepthStencilFeatures->separateDepthStencilLayouts ? true : false;
    }
#endif
    return false;
}

bool Device::negativeViewportHeightEnabled(bool khronos) const noexcept
{
#if defined(VK_KHR_maintenance1) && defined(VK_AMD_negative_viewport_height)
    return extensionEnabled(khronos ?
        VK_KHR_MAINTENANCE1_EXTENSION_NAME :
        VK_AMD_NEGATIVE_VIEWPORT_HEIGHT_EXTENSION_NAME);
#else
    MAGMA_UNUSED(khronos);
    return false;
#endif
}

const void *Device::findExtendedFeatures(VkStructureType featuresType) const noexcept
{
#ifdef VK_KHR_get_physical_device_properties2
    for (auto features : enabledExtendedFeatures)
    {
        struct VkFeaturesNode
        {
            VkStructureType sType;
            void *pNext;
            // ...
        };
        const VkFeaturesNode *featuresNode = reinterpret_cast<const VkFeaturesNode *>(features);
        if (featuresNode->sType == featuresType)
            return features;
    }
#else
    MAGMA_UNUSED(featuresType);
#endif
    return nullptr;
}
} // namespace magma
