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
#include "buffer.h"
#include "device.h"
#include "deviceMemory.h"
#include "queue.h"
#include "fence.h"
#include "commandBuffer.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../core/copyMemory.h"

namespace magma
{
Buffer::Buffer(std::shared_ptr<Device> device, VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags, VkBufferCreateFlags flags,
    const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    NonDispatchableResource(VK_OBJECT_TYPE_BUFFER, device, sharing, allocator),
    flags(flags),
    usage(usage)
{
    VkBufferCreateInfo bufferInfo;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = flags;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharing.getMode();
    bufferInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    bufferInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    const VkResult result = vkCreateBuffer(MAGMA_HANDLE(device), &bufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create buffer");
    const VkMemoryRequirements memoryRequirements = getMemoryRequirements();
    std::shared_ptr<DeviceMemory> memory = std::make_shared<DeviceMemory>(
        std::move(device), memoryRequirements, memoryFlags,
        &handle, VK_OBJECT_TYPE_BUFFER, std::move(allocator));
    bindMemory(std::move(memory));
}

Buffer::~Buffer()
{
    vkDestroyBuffer(*device, handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void Buffer::realloc(VkDeviceSize newSize,
    std::shared_ptr<Allocator> allocator /* nullptr */)
{
    if (getSize() == newSize)
        return;
    VkBufferCreateInfo bufferInfo;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = flags;
    bufferInfo.size = newSize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharing.getMode();
    bufferInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    bufferInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    vkDestroyBuffer(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
    hostAllocator = MAGMA_HOST_ALLOCATOR(allocator);
    deviceAllocator = MAGMA_DEVICE_ALLOCATOR(allocator);
    const VkResult result = vkCreateBuffer(MAGMA_HANDLE(device), &bufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to reallocate buffer");
    memory->realloc(newSize, &handle, VK_OBJECT_TYPE_BUFFER, std::move(allocator));
    bindMemory(std::move(memory), offset);
}

VkMemoryRequirements Buffer::getMemoryRequirements() const noexcept
{
    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(MAGMA_HANDLE(device), handle, &memoryRequirements);
    return memoryRequirements;
}

VkDescriptorBufferInfo Buffer::getDescriptor() const noexcept
{
    VkDescriptorBufferInfo descriptor;
    descriptor.buffer = handle;
    descriptor.offset = 0;
    descriptor.range = VK_WHOLE_SIZE;
    return descriptor;
}

void Buffer::bindMemory(std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    memory->bind(&handle, VK_OBJECT_TYPE_BUFFER, offset);
    this->size = memory->getSize();
    this->offset = offset;
    this->memory = std::move(memory);
}

#ifdef VK_KHR_device_group
void Buffer::bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
    const std::vector<uint32_t>& deviceIndices,
    VkDeviceSize offset /* 0 */)
{
    VkBindBufferMemoryDeviceGroupInfoKHR deviceGroupBindInfo;
    deviceGroupBindInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO_KHR;
    deviceGroupBindInfo.pNext = nullptr;
    deviceGroupBindInfo.deviceIndexCount = MAGMA_COUNT(deviceIndices);
    deviceGroupBindInfo.pDeviceIndices = deviceIndices.data();
    VkBindBufferMemoryInfoKHR bindInfo;
    bindInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO_KHR;
    bindInfo.pNext = &deviceGroupBindInfo;
    bindInfo.buffer = handle;
    bindInfo.memory = *memory;
    bindInfo.memoryOffset = memory->getOffset() + offset;
    MAGMA_DEVICE_EXTENSION(vkBindBufferMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    const VkResult result = vkBindBufferMemory2KHR(MAGMA_HANDLE(device), 1, &bindInfo);
    MAGMA_THROW_FAILURE(result, "failed to bind buffer memory within device group");
    this->size = memory->getSize();
    this->offset = offset;
    this->memory = std::move(memory);
}
#endif // VK_KHR_device_group

void Buffer::onDefragment()
{
    VkBufferCreateInfo bufferInfo;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = flags;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharing.getMode();
    bufferInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    bufferInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    vkDestroyBuffer(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
    const VkResult result = vkCreateBuffer(MAGMA_HANDLE(device), &bufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to recreate defragmented buffer");
    bindMemory(std::move(memory), offset);
}

void Buffer::copyHost(const void *data, CopyMemoryFunction copyFn) noexcept
{
    void *buffer = memory->map();
    if (buffer)
    {
        if (!copyFn)
            copyFn = core::copyMemory;
        copyFn(buffer, data, static_cast<std::size_t>(size));
        memory->unmap();
    }
}

void Buffer::copyTransfer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const Buffer> srcBuffer,
    VkDeviceSize size /* 0 */,
    VkDeviceSize srcOffset /* 0 */,
    VkDeviceSize dstOffset /* 0 */)
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    if (!size)
        size = srcBuffer->getSize() - srcOffset;
    region.size = std::min(this->getSize(), size);
    // We couldn't call shared_from_this() from ctor, so use custom ref object w/ empty deleter
    const auto weakThis = std::shared_ptr<Buffer>(this, [](Buffer *) {});
    cmdBuffer->copyBuffer(srcBuffer, weakThis, region);
}
} // namespace magma
