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
#include "resource.h"
#include "physicalDevice.h"
#include "commandBuffer.h"
#include "commandPool.h"
#include "queue.h"
#include "fence.h"

namespace magma
{
Resource::Resource(const Sharing& sharing, std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator) noexcept:
    size(0),
    offset(0),
    sharing(sharing),
    deviceAllocator(std::move(deviceAllocator))
{}

void Resource::commitAndWait(std::shared_ptr<CommandBuffer> cmdBuffer)
{
    std::shared_ptr<CommandPool> cmdPool = cmdBuffer->getCommandPool();
    uint32_t queueFamilyIndex = cmdPool->getQueueFamilyIndex();
    std::shared_ptr<Queue> queue = cmdBuffer->getDevice()->getQueueByFamily(queueFamilyIndex);
    if (!queue)
        MAGMA_THROW("submission queue not found");
    std::shared_ptr<Fence> fence = cmdBuffer->getFence();
    fence->reset();
    queue->submit(std::move(cmdBuffer), 0, nullptr, nullptr, fence);
    fence->wait();
}

Resource::Sharing::Sharing(const std::vector<uint32_t>& queueFamilyIndices) noexcept:
    queueFamilyIndices(std::move(queueFamilyIndices))
{}

Resource::Sharing::Sharing(std::shared_ptr<const PhysicalDevice> device,
    const std::initializer_list<VkQueueFlagBits>& queueTypes)
{
    const auto queueFamilyProperties = device->getQueueFamilyProperties();
    for (const VkQueueFlagBits queueType : queueTypes)
    {
        const uint32_t queueFamilyIndex = chooseFamilyIndex(queueType, queueFamilyProperties);
        queueFamilyIndices.push_back(queueFamilyIndex);
    }
}

VkSharingMode Resource::Sharing::getMode() const noexcept
{
    return queueFamilyIndices.empty() ?
        VK_SHARING_MODE_EXCLUSIVE :
        VK_SHARING_MODE_CONCURRENT;
}

uint32_t Resource::Sharing::getQueueFamiliesCount() const noexcept
{
    return MAGMA_COUNT(queueFamilyIndices);
}

uint32_t Resource::Sharing::chooseFamilyIndex(VkQueueFlagBits queueType,
    const std::vector<VkQueueFamilyProperties>& queueFamilyProperties) const noexcept
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
    // Let's try first queue family
    return 0;
}
} // namespace magma
