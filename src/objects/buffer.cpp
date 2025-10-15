/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "commandBuffer.h"
#include "../misc/extension.h"
#include "../misc/finish.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Buffer::Buffer(std::shared_ptr<Device> device, VkDeviceSize size,
    VkBufferCreateFlags flags_, VkBufferUsageFlags usage_, VkMemoryPropertyFlags memoryFlags,
    const Initializer& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    NonDispatchable<VkBuffer>(VK_OBJECT_TYPE_BUFFER, device, MAGMA_HOST_ALLOCATOR(allocator)),
    Resource(size, sharing),
    flags(flags_| optional.flags),
    usage(usage_| (optional.srcTransfer ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT : 0) |
    #if defined(VK_KHR_buffer_device_address)
        (optional.deviceAddress ? VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR : 0) |
    #elif defined(VK_EXT_buffer_device_address)
        (optional.deviceAddress ? VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT : 0) |
    #endif
        (optional.storage ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : 0))
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
    const VkResult result = vkCreateBuffer(getNativeDevice(), &bufferInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create buffer");
    postCreate(handle);
    if (optional.aliasingMemory)
        return;
    // Prepare memory requirements
    VkMemoryRequirements memoryRequirements;
    StructureChain extendedMemoryInfo;
#if defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_dedicated_allocation)
    if (extensionEnabled(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME) &&
        extensionEnabled(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME))
    {
        VkMemoryDedicatedRequirementsKHR dedicatedRequirements = {};
        dedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR;
        memoryRequirements = getMemoryRequirements2(&dedicatedRequirements);
        if (dedicatedRequirements.prefersDedicatedAllocation ||
            dedicatedRequirements.requiresDedicatedAllocation)
        {   // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_KHR_dedicated_allocation.html
            VkMemoryDedicatedAllocateInfoKHR dedicatedAllocateInfo;
            dedicatedAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR;
            dedicatedAllocateInfo.pNext = nullptr;
            dedicatedAllocateInfo.image = VK_NULL_HANDLE;
            dedicatedAllocateInfo.buffer = handle;
            extendedMemoryInfo.linkNode(dedicatedAllocateInfo);
        }
    }
    else
#endif // VK_KHR_get_memory_requirements2 && VK_KHR_dedicated_allocation
    {
        memoryRequirements = getMemoryRequirements();
    }
#ifdef VK_KHR_device_group
    VkMemoryAllocateFlagsInfoKHR memoryAllocateFlagsInfo = {};
    memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
    if (extensionEnabled(VK_KHR_DEVICE_GROUP_EXTENSION_NAME))
    {
        memoryAllocateFlagsInfo.flags |= VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR;
        memoryAllocateFlagsInfo.deviceMask = optional.deviceMask;
    }
#endif // VK_KHR_device_group
#ifdef VK_KHR_buffer_device_address
    if (extensionEnabled(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
    {
        if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR)
            memoryAllocateFlagsInfo.flags |= VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
        if (optional.deviceAddressCaptureReplay)
            memoryAllocateFlagsInfo.flags |= VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_KHR;
    }
#endif // VK_KHR_buffer_device_address
#ifdef VK_KHR_device_group
    if (memoryAllocateFlagsInfo.flags)
        extendedMemoryInfo.linkNode(memoryAllocateFlagsInfo);
#endif // VK_KHR_device_group
#ifdef VK_EXT_memory_priority
    if (extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        VkMemoryPriorityAllocateInfoEXT memoryPriorityAllocateInfo;
        memoryPriorityAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        memoryPriorityAllocateInfo.pNext = nullptr;
        memoryPriorityAllocateInfo.priority = optional.memoryPriority;
        extendedMemoryInfo.linkNode(memoryPriorityAllocateInfo);
    }
#endif // VK_EXT_memory_priority
    if (optional.lazilyAllocated)
    {   // Memory types must not have both VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT
        // and VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT set.
        if (!(memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
            memoryFlags |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
    }
    // Allocate and bind buffer memory
    std::unique_ptr<IDeviceMemory> bufferMemory = allocateMemory(handle,
        memoryRequirements, memoryFlags, extendedMemoryInfo,
        std::move(device), std::move(allocator));
    bindMemory(std::move(bufferMemory));
}

Buffer::~Buffer()
{
    preDestroy(handle);
    vkDestroyBuffer(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}

VkMemoryRequirements Buffer::getMemoryRequirements() const noexcept
{
    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(getNativeDevice(), handle, &memoryRequirements);
    return memoryRequirements;
}

#ifdef VK_KHR_get_memory_requirements2
VkMemoryRequirements Buffer::getMemoryRequirements2(void *memoryRequirements) const
{
    VkMemoryRequirements2KHR memoryRequirements2;
    VkBufferMemoryRequirementsInfo2KHR bufferMemoryRequirementsInfo2;
    memoryRequirements2.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR;
    memoryRequirements2.pNext = memoryRequirements;
    memoryRequirements2.memoryRequirements = {};
    bufferMemoryRequirementsInfo2.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2_KHR;
    bufferMemoryRequirementsInfo2.pNext = nullptr;
    bufferMemoryRequirementsInfo2.buffer = handle;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetBufferMemoryRequirements2KHR, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
    vkGetBufferMemoryRequirements2KHR(getNativeDevice(), &bufferMemoryRequirementsInfo2, &memoryRequirements2);
    return memoryRequirements2.memoryRequirements;
}
#endif // VK_KHR_get_memory_requirements2

VkSparseMemoryBind Buffer::getSparseMemoryBind(VkDeviceSize bufferOffset, VkDeviceSize size, VkDeviceSize memoryOffset) const noexcept
{
    VkSparseMemoryBind sparseMemoryBind;
    sparseMemoryBind.resourceOffset = bufferOffset;
    sparseMemoryBind.size = size;
    sparseMemoryBind.memory = memory->getNativeHandle();
    sparseMemoryBind.memoryOffset = memoryOffset;
    sparseMemoryBind.flags = 0;
    return sparseMemoryBind;
}

VkDescriptorBufferInfo Buffer::getDescriptor() const noexcept
{
    VkDescriptorBufferInfo bufferDescriptorInfo;
    bufferDescriptorInfo.buffer = handle;
    bufferDescriptorInfo.offset = 0;
    bufferDescriptorInfo.range = VK_WHOLE_SIZE;
    return bufferDescriptorInfo;
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
    vkDestroyBuffer(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
    const VkResult result = vkCreateBuffer(getNativeDevice(), &bufferInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to reallocate buffer");
    // Reallocate buffer memory
    StructureChain extendedMemoryInfo;
    VkMemoryRequirements memoryRequirements;
#if defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_dedicated_allocation)
    if (extensionEnabled(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME) &&
        extensionEnabled(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME))
    {
        VkMemoryDedicatedRequirementsKHR dedicatedRequirements = {};
        dedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR;
        memoryRequirements = getMemoryRequirements2(&dedicatedRequirements);
        if (dedicatedRequirements.prefersDedicatedAllocation ||
            dedicatedRequirements.requiresDedicatedAllocation)
        {
            VkMemoryDedicatedAllocateInfoKHR dedicatedAllocateInfo;
            dedicatedAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR;
            dedicatedAllocateInfo.pNext = nullptr;
            dedicatedAllocateInfo.image = VK_NULL_HANDLE;
            dedicatedAllocateInfo.buffer = handle;
            extendedMemoryInfo.linkNode(dedicatedAllocateInfo);
        }
    }
    else
#endif // VK_KHR_get_memory_requirements2 && VK_KHR_dedicated_allocation
    {
        memoryRequirements = getMemoryRequirements();
    }
#ifdef VK_KHR_device_group
    if (extensionEnabled(VK_KHR_DEVICE_GROUP_EXTENSION_NAME))
    {
        VkMemoryAllocateFlagsInfoKHR memoryAllocateFlagsInfo;
        memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
        memoryAllocateFlagsInfo.pNext = nullptr;
        memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT_KHR;
        memoryAllocateFlagsInfo.deviceMask = memory->getDeviceMask();
        extendedMemoryInfo.linkNode(memoryAllocateFlagsInfo);
    }
#endif // VK_KHR_device_group
#ifdef VK_EXT_memory_priority
    if (extensionEnabled(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
    {
        VkMemoryPriorityAllocateInfoEXT memoryPriorityAllocateInfo;
        memoryPriorityAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
        memoryPriorityAllocateInfo.pNext = nullptr;
        memoryPriorityAllocateInfo.priority = memory->getPriority();
        extendedMemoryInfo.linkNode(memoryPriorityAllocateInfo);
    }
#endif // VK_EXT_memory_priority
    memory->realloc(handle, memoryRequirements, extendedMemoryInfo);
    bindMemory(std::move(memory), offset);
}

void Buffer::bindMemory(std::unique_ptr<IDeviceMemory> deviceMemory,
    VkDeviceSize offset_ /* 0 */)
{
    MAGMA_ASSERT(deviceMemory->getSize() >= getSize());
    deviceMemory->bind(handle, VK_OBJECT_TYPE_BUFFER, offset_);
    memory = std::move(deviceMemory);
    offset = offset_;
}

#ifdef VK_KHR_device_group
void Buffer::bindMemoryDeviceGroup(std::unique_ptr<IDeviceMemory> deviceMemory,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& /* splitInstanceBindRegions */,
    VkDeviceSize offset_ /* 0 */)
{
    MAGMA_ASSERT(deviceMemory->getSize() >= getSize());
    deviceMemory->bindDeviceGroup(handle, VK_OBJECT_TYPE_BUFFER, deviceIndices, {/* unused */}, offset_);
    memory = std::move(deviceMemory);
    offset = offset_;
}
#endif // VK_KHR_device_group

#if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
VkDeviceAddress Buffer::getDeviceAddress() const noexcept
{
#ifdef VK_KHR_buffer_device_address
    MAGMA_DEVICE_EXTENSION(vkGetBufferDeviceAddressKHR);
    if (vkGetBufferDeviceAddressKHR)
    {
        VkBufferDeviceAddressInfoKHR bufferDeviceAddressInfo;
        bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
        bufferDeviceAddressInfo.pNext = nullptr;
        bufferDeviceAddressInfo.buffer = handle;
        return vkGetBufferDeviceAddressKHR(getNativeDevice(), &bufferDeviceAddressInfo);
    }
#endif // VK_KHR_buffer_device_address
#ifdef VK_EXT_buffer_device_address
    MAGMA_DEVICE_EXTENSION(vkGetBufferDeviceAddressEXT);
    if (vkGetBufferDeviceAddressEXT)
    {
        VkBufferDeviceAddressInfoEXT bufferDeviceAddressInfo;
        bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_EXT;
        bufferDeviceAddressInfo.pNext = nullptr;
        bufferDeviceAddressInfo.buffer = handle;
        return vkGetBufferDeviceAddressEXT(getNativeDevice(), &bufferDeviceAddressInfo);
    }
#endif // VK_EXT_buffer_device_address
    return MAGMA_NULL;
}
#endif // VK_KHR_buffer_device_address || VK_EXT_buffer_device_address

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
    IAllocator *allocator = MAGMA_OPTIONAL(hostAllocator);
    preDestroy(handle);
    vkDestroyBuffer(getNativeDevice(), handle, allocator);
    handle = VK_NULL_HANDLE;
    const VkResult result = vkCreateBuffer(getNativeDevice(), &bufferInfo, allocator, &handle);
    MAGMA_HANDLE_RESULT(result, "failed to recreate defragmented buffer");
    postCreate(handle);
    bindMemory(std::move(memory), offset);
}

VkDeviceSize Buffer::hostCopy(const void *srcBuffer, VkDeviceSize srcBufferSize,
    VkDeviceSize srcOffset /* 0 */,
    VkDeviceSize dstOffset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    CopyMemoryFn copyMem /* nullptr */) noexcept
{
    MAGMA_ASSERT(srcBuffer);
    if (!srcBuffer || !srcBufferSize || !size)
        return 0;
    MAGMA_ASSERT(srcOffset < srcBufferSize);
    MAGMA_ASSERT(dstOffset < getSize());
    if ((srcOffset >= srcBufferSize) || (dstOffset >= getSize()))
        return 0;
    const VkDeviceSize srcRemaining = srcBufferSize - srcOffset;
    const VkDeviceSize dstRemaining = getSize() - dstOffset;
    if (VK_WHOLE_SIZE == size)
        size = dstRemaining;
    else
        size = std::min(size, dstRemaining);
    const VkDeviceSize safeCopySize = std::min(size, srcRemaining);
    MAGMA_ASSERT(safeCopySize);
    void *dstBuffer = memory->map(dstOffset, safeCopySize);
    if (dstBuffer)
    {
        if (!copyMem)
            copyMem = std::memcpy;
        copyMem(dstBuffer, (uint8_t *)srcBuffer + srcOffset, static_cast<std::size_t>(safeCopySize));
        memory->unmap();
        return safeCopySize;
    }
    return 0;
}

VkDeviceSize Buffer::transferCopy(lent_ptr<CommandBuffer> cmdBuffer, lent_ptr<const Buffer> srcBuffer,
    VkDeviceSize srcOffset /* 0 */,
    VkDeviceSize dstOffset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept
{
    MAGMA_ASSERT(srcBuffer);
    const VkDeviceSize srcBufferSize = srcBuffer->getSize();
    MAGMA_ASSERT(srcOffset < srcBufferSize);
    MAGMA_ASSERT(dstOffset < getSize());
    if ((srcOffset >= srcBufferSize) || (dstOffset >= getSize()))
        return 0;
    const VkDeviceSize srcRemaining = srcBufferSize - srcOffset;
    const VkDeviceSize dstRemaining = getSize() - dstOffset;
    if (VK_WHOLE_SIZE == size)
        size = dstRemaining;
    else
        size = std::min(size, dstRemaining);
    const VkDeviceSize safeCopySize = std::min(size, srcRemaining);
    MAGMA_ASSERT(safeCopySize);
    cmdBuffer->copyBuffer(std::move(srcBuffer), this, srcOffset, dstOffset, safeCopySize);
    return safeCopySize;
}

VkDeviceSize Buffer::stagingCopy(lent_ptr<CommandBuffer> cmdBuffer, const void *srcBuffer, VkDeviceSize srcBufferSize,
    VkDeviceSize srcOffset /* 0 */,
    VkDeviceSize dstOffset /* 0 */,
    VkDeviceSize size /* VK_WHOLE_SIZE */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    CopyMemoryFn copyMem /* nullptr */)
{
    MAGMA_ASSERT(srcBuffer);
    if (!srcBuffer || !srcBufferSize || !size)
        return 0;
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    if (cmdBuffer->reset())
    {
        if (cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
        {
            const VkDeviceSize srcRemaining = srcBufferSize - srcOffset;
            if (VK_WHOLE_SIZE == size)
                size = srcRemaining;
            else
                size = std::min(size, srcRemaining);
            // Allocate temporary staging buffer
            auto stagingBuffer = std::make_unique<SrcTransferBuffer>(device, size, std::move(allocator));
            stagingBuffer->hostCopy(srcBuffer, srcBufferSize, srcOffset, 0, size, std::move(copyMem));
            // Copy host -> device
            const VkDeviceSize numberOfBytesCopied = transferCopy(cmdBuffer.get(),
                stagingBuffer.get(), 0, dstOffset, size);
            cmdBuffer->end();
            // Block until execution is complete
            finish(std::move(cmdBuffer));
            return numberOfBytesCopied;
        }
    }
    return 0;
}
} // namespace magma
