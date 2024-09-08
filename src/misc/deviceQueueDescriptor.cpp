/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "deviceQueueDescriptor.h"
#include "../objects/physicalDevice.h"
#include "../exceptions/exception.h"

namespace magma
{
DeviceQueueDescriptor::DeviceQueueDescriptor(std::shared_ptr<const PhysicalDevice> device,
    VkQueueFlagBits capabilities, const std::vector<float>& queuePriorities /* 1 */):
    VkDeviceQueueCreateInfo{
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        nullptr, // pNext,
        0, // flags
        0, // queueFamilyIndex
        MAGMA_COUNT(queuePriorities),
        core::copyVector(queuePriorities)
    }
{
    const std::vector<VkQueueFamilyProperties> properties = device->getQueueFamilyProperties();
    std::optional<uint32_t> familyIndex;
    if (VK_QUEUE_GRAPHICS_BIT == capabilities)
        familyIndex = findDedicatedQueueFamily(properties, VK_QUEUE_GRAPHICS_BIT);
    else if (VK_QUEUE_COMPUTE_BIT == capabilities)
        familyIndex = findDedicatedQueueFamily(properties, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT);
    else if (capabilities & (VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT))
        familyIndex = findDedicatedQueueFamily(properties, capabilities, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    if (familyIndex)
        queueFamilyIndex = familyIndex.value();
    else
    {
        familyIndex = findAnySuitableQueueFamily(properties, capabilities);
        if (familyIndex)
            queueFamilyIndex = familyIndex.value();
        else
            MAGMA_ERROR("could not find suitable queue family");
    }
}

DeviceQueueDescriptor::DeviceQueueDescriptor(const DeviceQueueDescriptor& other):
    VkDeviceQueueCreateInfo{
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        other.pNext,
        other.flags,
        other.queueFamilyIndex,
        other.queueCount,
        core::copyArray(other.pQueuePriorities, other.queueCount)
    }
{}

DeviceQueueDescriptor& DeviceQueueDescriptor::operator=(const DeviceQueueDescriptor& other)
{
    if (this != &other)
    {
        pNext = other.pNext;
        flags = other.flags;
        queueFamilyIndex = other.queueFamilyIndex;
        queueCount = other.queueCount;
        delete[] pQueuePriorities;
        pQueuePriorities = core::copyArray(other.pQueuePriorities, other.queueCount);
    }
    return *this;
}

DeviceQueueDescriptor::~DeviceQueueDescriptor()
{
    delete[] pQueuePriorities;
}

std::optional<uint32_t> DeviceQueueDescriptor::findDedicatedQueueFamily(const std::vector<VkQueueFamilyProperties>& properties,
    VkQueueFlags capability, VkQueueFlags excludeCapabilities /* 0 */) const noexcept
{
    uint32_t queueFamilyIndex = 0;
    for (auto const& property: properties)
    {   // Best if queue has only dedicated capability
        if (property.queueFlags == capability)
            return queueFamilyIndex;
        ++queueFamilyIndex;
    }
    queueFamilyIndex = 0;
    for (auto const& property: properties)
    {
        if (property.queueFlags & capability)
        {   // Check if queue doesn't support mixed capabilities
            if (!(property.queueFlags & excludeCapabilities))
                return queueFamilyIndex;
        }
        ++queueFamilyIndex;
    }
    return std::nullopt;
}

std::optional<uint32_t> DeviceQueueDescriptor::findAnySuitableQueueFamily(const std::vector<VkQueueFamilyProperties>& properties,
    VkQueueFlags capabilities) const noexcept
{
    uint32_t queueFamilyIndex = 0;
    for (auto const& property: properties)
    {   // Check if queue supports requested capabilities
        if (property.queueFlags & capabilities)
            return queueFamilyIndex;
        ++queueFamilyIndex;
    }
    return std::nullopt;
}
} // namespace magma
