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
#include "srcTransferBuffer.h"
#include "device.h"
#include "deviceMemory.h"
#include "managedDeviceMemory.h"
#include "queue.h"
#include "fence.h"
#include "commandBuffer.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../core/copyMemory.h"

namespace magma
{
Buffer::Buffer(std::shared_ptr<Device> device, VkDeviceSize size,
    VkBufferCreateFlags flags, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags,
    const Descriptor& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    NonDispatchableResource(VK_OBJECT_TYPE_BUFFER, device, sharing, allocator),
    flags(flags),
    usage(usage)
{
    VkBufferCreateInfo bufferInfo;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.flags = flags | optional.flags;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharing.getMode();
    bufferInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    bufferInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    const VkResult result = vkCreateBuffer(MAGMA_HANDLE(device), &bufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create buffer");
    const VkMemoryRequirements memoryRequirements = getMemoryRequirements();
    if (optional.lazy && !(memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
        memoryFlags |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
    std::shared_ptr<IDeviceMemory> memory;
    if (MAGMA_DEVICE_ALLOCATOR(allocator))
    {
        memory = std::make_shared<ManagedDeviceMemory>(
            std::move(device),
            memoryRequirements, memoryFlags, optional.memoryPriority,
            handle, VK_OBJECT_TYPE_BUFFER,
            MAGMA_HOST_ALLOCATOR(allocator),
            MAGMA_DEVICE_ALLOCATOR(allocator));
    }
    else
    {
        memory = std::make_shared<DeviceMemory>(
            std::move(device),
            memoryRequirements, memoryFlags, optional.memoryPriority,
            MAGMA_HOST_ALLOCATOR(allocator));
    }
    bindMemory(std::move(memory));
}

Buffer::~Buffer()
{
    vkDestroyBuffer(*device, handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void Buffer::realloc(VkDeviceSize newSize)
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
    const VkResult result = vkCreateBuffer(MAGMA_HANDLE(device), &bufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to reallocate buffer");
    memory->realloc(newSize, memory->getPriority(), handle, VK_OBJECT_TYPE_BUFFER);
    bindMemory(std::move(memory), offset);
}

VkMemoryRequirements Buffer::getMemoryRequirements() const noexcept
{
    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(MAGMA_HANDLE(device), handle, &memoryRequirements);
    return memoryRequirements;
}

#ifdef VK_KHR_get_memory_requirements2
VkMemoryRequirements Buffer::getMemoryRequirements2(void *memoryRequirements) const
{
    VkBufferMemoryRequirementsInfo2KHR bufferMemoryRequirementsInfo2;
    bufferMemoryRequirementsInfo2.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2_KHR;
    bufferMemoryRequirementsInfo2.pNext = nullptr;
    bufferMemoryRequirementsInfo2.buffer = handle;
    VkMemoryRequirements2KHR memoryRequirements2;
    memoryRequirements2.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR;
    memoryRequirements2.pNext = memoryRequirements;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetBufferMemoryRequirements2KHR, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
    vkGetBufferMemoryRequirements2KHR(MAGMA_HANDLE(device), &bufferMemoryRequirementsInfo2, &memoryRequirements2);
    return memoryRequirements2.memoryRequirements;
}
#endif // VK_KHR_get_memory_requirements2

VkDescriptorBufferInfo Buffer::getDescriptor() const noexcept
{
    VkDescriptorBufferInfo bufferDescriptorInfo;
    bufferDescriptorInfo.buffer = handle;
    bufferDescriptorInfo.offset = 0;
    bufferDescriptorInfo.range = VK_WHOLE_SIZE;
    return bufferDescriptorInfo;
}

#if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
VkDeviceAddress Buffer::getDeviceAddress() const
{
#ifdef VK_KHR_buffer_device_address
    MAGMA_DEVICE_EXTENSION(vkGetBufferDeviceAddressKHR);
    if (vkGetBufferDeviceAddressKHR)
    {
        VkBufferDeviceAddressInfoKHR bufferDeviceAddressInfo;
        bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
        bufferDeviceAddressInfo.pNext = nullptr;
        bufferDeviceAddressInfo.buffer = handle;
        return vkGetBufferDeviceAddressKHR(MAGMA_HANDLE(device), &bufferDeviceAddressInfo);
    }
#endif // VK_KHR_buffer_device_address
#ifdef VK_EXT_buffer_device_address
    VkBufferDeviceAddressInfoEXT bufferDeviceAddressInfo;
    bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_EXT;
    bufferDeviceAddressInfo.pNext = nullptr;
    bufferDeviceAddressInfo.buffer = handle;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetBufferDeviceAddressEXT, VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
    return vkGetBufferDeviceAddressEXT(MAGMA_HANDLE(device), &bufferDeviceAddressInfo);
#else
    return 0ull;
#endif // VK_EXT_buffer_device_address
}
#endif // VK_KHR_buffer_device_address || VK_EXT_buffer_device_address

void Buffer::bindMemory(std::shared_ptr<IDeviceMemory> memory_,
    VkDeviceSize offset_ /* 0 */)
{
    memory_->bind(handle, VK_OBJECT_TYPE_BUFFER, offset_);
    memory = std::move(memory_);
    offset = offset_;
    size = memory->getSize();
}

#ifdef VK_KHR_device_group
void Buffer::bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> memory_,
    const std::vector<uint32_t>& deviceIndices,
    VkDeviceSize offset_ /* 0 */)
{
    VkBindBufferMemoryInfoKHR bindMemoryInfo;
    VkBindBufferMemoryDeviceGroupInfoKHR bindMemoryDeviceGroupInfo;
    bindMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO_KHR;
    bindMemoryInfo.pNext = &bindMemoryDeviceGroupInfo;
    bindMemoryInfo.buffer = handle;
    bindMemoryInfo.memory = memory_->getNativeHandle();
    bindMemoryInfo.memoryOffset = memory_->getSuballocationOffset() + offset_;
    bindMemoryDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO_KHR;
    bindMemoryDeviceGroupInfo.pNext = nullptr;
    bindMemoryDeviceGroupInfo.deviceIndexCount = MAGMA_COUNT(deviceIndices);
    bindMemoryDeviceGroupInfo.pDeviceIndices = deviceIndices.data();
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindBufferMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    const VkResult result = vkBindBufferMemory2KHR(MAGMA_HANDLE(device), 1, &bindMemoryInfo);
    MAGMA_THROW_FAILURE(result, "failed to bind buffer memory within device group");
    memory = std::move(memory_);
    offset = offset_;
    size = memory->getSize();
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

void Buffer::copyHost(const void *srcBuffer,
    VkDeviceSize srcOffset /* 0 */,
    VkDeviceSize dstOffset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    CopyMemoryFunction copyFn /* nullptr */) noexcept
{
    void *dstBuffer = memory->map(dstOffset, size);
    if (dstBuffer)
    {
        if (!copyFn)
            copyFn = core::copyMemory;
        if (VK_WHOLE_SIZE == size)
        {
            MAGMA_ASSERT(0 == dstOffset);
            size = getSize();
        }
        copyFn(dstBuffer, (uint8_t *)srcBuffer + srcOffset, static_cast<std::size_t>(size));
        memory->unmap();
    }
}

void Buffer::copyTransfer(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    VkDeviceSize srcOffset /* 0 */,
    VkDeviceSize dstOffset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */)
{
    VkBufferCopy region;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    region.size = (VK_WHOLE_SIZE == size) ? getSize() : size;
    // We couldn't call shared_from_this() from ctor, so use custom ref object w/ empty deleter
    std::shared_ptr<Buffer> self = std::shared_ptr<Buffer>(this, [](Buffer *) {});
    cmdBuffer->copyBuffer(srcBuffer, self, region);
}
} // namespace magma
