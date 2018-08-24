/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "deviceQueueDesc.h"
#include "../objects/physicalDevice.h"
#include "../misc/exception.h"
#include "../helpers/copy.h"
#include "../shared.h"

namespace magma
{
DeviceQueueDescriptor::DeviceQueueDescriptor(VkQueueFlagBits queueType,
    std::shared_ptr<const PhysicalDevice> device,
    const std::vector<float>& queuePriorities /* 1 */)
{
    sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    queueFamilyIndex = getFamilyIndex(queueType, device->getQueueFamilyProperties());
    queueCount = MAGMA_COUNT(queuePriorities);
#ifdef MAGMA_DEBUG
    for (float priority : queuePriorities)
        MAGMA_ASSERT(priority >= 0.f && priority <= 1.f);
#endif
    pQueuePriorities = helpers::copyVector(queuePriorities);
}

DeviceQueueDescriptor::DeviceQueueDescriptor(const DeviceQueueDescriptor& other)
{
    helpers::copy(this, &other);
    pQueuePriorities = helpers::copyArray(other.pQueuePriorities, queueCount);
}

DeviceQueueDescriptor& DeviceQueueDescriptor::operator=(const DeviceQueueDescriptor& other)
{
    if (this != &other)
    {
        helpers::copy(this, &other);
        pQueuePriorities = helpers::copyArray(other.pQueuePriorities, queueCount);
    }
    return *this;
}

DeviceQueueDescriptor::~DeviceQueueDescriptor()
{
    delete[] pQueuePriorities;
}

uint32_t DeviceQueueDescriptor::getFamilyIndex(VkQueueFlagBits queueType,
    const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const
{
    if (VK_QUEUE_COMPUTE_BIT == queueType)
    {   // Try to find dedicated compute queue
        uint32_t queueFamilyIndex = 0;
        for (const auto& property : queueFamilyProperties)
        {
            if (property.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                const VkFlags hasGraphics = property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                if (!hasGraphics)
                    return queueFamilyIndex;
            }
            ++queueFamilyIndex;
        }
    } else if (VK_QUEUE_TRANSFER_BIT == queueType)
    {   // Try to find dedicated transfer queue
        uint32_t queueFamilyIndex = 0;
        for (const auto& property : queueFamilyProperties)
        {
            if (property.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                const VkFlags hasGraphicsOrCompute = property.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
                if (!hasGraphicsOrCompute)
                    return queueFamilyIndex;
            }
            ++queueFamilyIndex;
        }
    }
    uint32_t queueFamilyIndex = 0;
    for (const auto& property : queueFamilyProperties)
    {   // Try to find any suitable family
        if (property.queueFlags & queueType)
            return queueFamilyIndex;
        ++queueFamilyIndex;
    }
    MAGMA_THROW("could not find suitable queue family");
}
} // namespace magma
