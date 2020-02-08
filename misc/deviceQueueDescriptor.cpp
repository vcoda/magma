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
#include "deviceQueueDescriptor.h"
#include "../objects/physicalDevice.h"
#include "../misc/exception.h"
#include "../core/copy.h"

namespace magma
{
DeviceQueueDescriptor::DeviceQueueDescriptor(std::shared_ptr<const PhysicalDevice> device,
    VkQueueFlagBits queueType, const std::vector<float>& queuePriorities /* 1 */)
{
#ifdef MAGMA_DEBUG
    for (float priority : queuePriorities)
        MAGMA_ASSERT((priority >= 0.f) && (priority <= 1.f));
#endif
    sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    queueFamilyIndex = chooseFamilyIndex(queueType, device->getQueueFamilyProperties());
    queueCount = MAGMA_COUNT(queuePriorities);
    pQueuePriorities = core::copyVector(queuePriorities);
}

DeviceQueueDescriptor::DeviceQueueDescriptor(const DeviceQueueDescriptor& other)
{
    core::copy(this, &other);
    pQueuePriorities = core::copyArray(other.pQueuePriorities, queueCount);
}

DeviceQueueDescriptor& DeviceQueueDescriptor::operator=(const DeviceQueueDescriptor& other)
{
    if (this != &other)
    {
        delete[] pQueuePriorities;
        core::copy(this, &other);
        pQueuePriorities = core::copyArray(other.pQueuePriorities, queueCount);
    }
    return *this;
}

DeviceQueueDescriptor::~DeviceQueueDescriptor()
{
    delete[] pQueuePriorities;
}

uint32_t DeviceQueueDescriptor::chooseFamilyIndex(VkQueueFlagBits queueType,
    const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const
{
    uint32_t queueFamilyIndex = 0;
    if (VK_QUEUE_COMPUTE_BIT == queueType)
    {   // Try to find dedicated compute queue
        for (const auto& property : queueFamilyProperties)
        {
            if (property.queueFlags & queueType)
            {   // Compute queue would be better separated from graphics
                const VkFlags hasGraphics = property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                if (!hasGraphics)
                    return queueFamilyIndex;
            }
            ++queueFamilyIndex;
        }
    } else if ((VK_QUEUE_TRANSFER_BIT == queueType) || (VK_QUEUE_SPARSE_BINDING_BIT == queueType))
    {   // Try to find dedicated transfer/sparse queue
        for (const auto& property : queueFamilyProperties)
        {
            if (property.queueFlags & queueType)
            {   // Transfer queue would be better separated from graphics or compute
                const VkFlags hasGraphicsOrCompute = property.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
                if (!hasGraphicsOrCompute)
                    return queueFamilyIndex;
            }
            ++queueFamilyIndex;
        }
    }
    // Some hardware is limited to have only single queue family with one queue in it
    queueFamilyIndex = 0;
    for (const auto& property : queueFamilyProperties)
    {   // Try to find any suitable family
        if (property.queueFlags & queueType)
            return queueFamilyIndex;
        ++queueFamilyIndex;
    }
    MAGMA_THROW("could not find suitable queue family");
}
} // namespace magma
