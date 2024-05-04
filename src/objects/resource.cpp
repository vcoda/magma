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
#include "resource.h"
#include "device.h"
#include "physicalDevice.h"
#include "commandBuffer.h"
#include "commandPool.h"
#include "queue.h"
#include "fence.h"

namespace magma
{
Resource::Resource(VkDeviceSize size, const Sharing& sharing, std::shared_ptr<IDeviceMemoryAllocator> deviceAllocator) noexcept:
    size(size),
    offset(0),
    sharing(sharing),
    deviceAllocator(std::move(deviceAllocator))
{}

void Resource::commitAndWait(std::shared_ptr<CommandBuffer> cmdBuffer)
{
    std::shared_ptr<CommandPool> cmdPool = cmdBuffer->getCommandPool();
    const uint32_t queueFamilyIndex = cmdPool->getQueueFamilyIndex();
    std::shared_ptr<Device> device = cmdBuffer->getDevice();
    std::shared_ptr<Queue> queue = device->getQueueForFamily(queueFamilyIndex);
    std::shared_ptr<Fence> fence = cmdBuffer->getFence();
    fence->reset();
    queue->submit(std::move(cmdBuffer), 0, nullptr, nullptr, fence);
    fence->wait();
}
} // namespace magma
