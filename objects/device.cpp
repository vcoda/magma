/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "queue.h"
#include "fence.h"
#include "resourcePool.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"
#include "../helpers/stackArray.h"

namespace magma
{
Device::Device(std::shared_ptr<PhysicalDevice> physicalDevice,
    const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers,
    const std::vector<const char *>& extensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures,
    std::shared_ptr<IAllocator> allocator):
    Dispatchable<VkDevice>(VK_OBJECT_TYPE_DEVICE, nullptr, std::move(allocator)),
    physicalDevice(std::move(physicalDevice)),
    resourcePool(std::make_shared<ResourcePool>())
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
            void *pNext;
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
        lastNode->pNext = nullptr; // End of list
    }
#endif // VK_KHR_get_physical_device_properties2
    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
#ifdef VK_KHR_get_physical_device_properties2
    info.pNext = extendedDeviceFeatures.empty() ? nullptr : &features;
#else
    info.pNext = nullptr;
#endif
    info.flags = 0;
    info.queueCreateInfoCount = MAGMA_COUNT(queueDescriptors);
    info.pQueueCreateInfos = queueDescriptors.data();
    info.enabledLayerCount = MAGMA_COUNT(layers);
    info.ppEnabledLayerNames = layers.data();
    info.enabledExtensionCount = MAGMA_COUNT(extensions);
    info.ppEnabledExtensionNames = extensions.data();
    info.pEnabledFeatures = extendedDeviceFeatures.empty() ? &deviceFeatures : nullptr;
    const VkResult create = vkCreateDevice(MAGMA_HANDLE(physicalDevice), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    switch (create) {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed("failed to create logical device");
    case VK_ERROR_DEVICE_LOST:
        throw exception::DeviceLost("failed to create logical device");
    }
    MAGMA_THROW_FAILURE(create, "failed to create logical device");
    queues.reserve(queueDescriptors.size());
    for (const auto& desc : queueDescriptors)
        queues.emplace_back(desc, std::weak_ptr<Queue>());
}

Device::~Device()
{
    MAGMA_ASSERT(resourcePool->hasAnyDeviceResource() == false);
    vkDestroyDevice(handle, MAGMA_OPTIONAL_INSTANCE(allocator));
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

bool Device::waitIdle() const
{
    const VkResult waitIdle = vkDeviceWaitIdle(handle);
    if (VK_ERROR_DEVICE_LOST == waitIdle)
        throw exception::DeviceLost("failed to wait for device become idle");
    MAGMA_THROW_FAILURE(waitIdle, "failed to wait for device become idle");
    return true;
}

bool Device::resetFences(std::vector<std::shared_ptr<Fence>>& fences) const noexcept
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult reset = vkResetFences(handle, dereferencedFences.size(), dereferencedFences);
    return (VK_SUCCESS == reset);
}

bool Device::waitForFences(std::vector<std::shared_ptr<Fence>>& fences, bool waitAll,
    uint64_t timeout /* UINT64_MAX */) const noexcept
{
    MAGMA_STACK_ARRAY(VkFence, dereferencedFences, fences.size());
    for (const auto& fence : fences)
        dereferencedFences.put(*fence);
    const VkResult wait = vkWaitForFences(handle, dereferencedFences.size(), dereferencedFences,
        MAGMA_BOOLEAN(waitAll), timeout);
    return (VK_SUCCESS == wait) || (VK_TIMEOUT == wait);
}

#ifdef VK_KHR_device_group
VkPeerMemoryFeatureFlags Device::getGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex) const noexcept
{
    VkPeerMemoryFeatureFlags peerMemoryFeatures = 0;
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkGetDeviceGroupPeerMemoryFeaturesKHR);
    if (vkGetDeviceGroupPeerMemoryFeaturesKHR)
        vkGetDeviceGroupPeerMemoryFeaturesKHR(handle, heapIndex, localDeviceIndex, remoteDeviceIndex, &peerMemoryFeatures);
    return peerMemoryFeatures;
}
#endif // VK_KHR_device_group
} // namespace magma
