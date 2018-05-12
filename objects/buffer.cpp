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
#include "buffer.h"
#include "device.h"
#include "deviceMemory.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
Buffer::Buffer(std::shared_ptr<const Device> device, VkDeviceSize size, VkBufferUsageFlags usage,
    VkBufferCreateFlags flags, std::shared_ptr<IAllocator> allocator, VkMemoryPropertyFlags memoryFlags):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, device, allocator),
    size(size)
{
    VkBufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.size = size;
    info.usage = usage;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    const VkResult create = vkCreateBuffer(*device, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create buffer");
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(*device, handle, &memoryRequirements);
    std::shared_ptr<DeviceMemory> memory(std::make_shared<DeviceMemory>(device, memoryRequirements.size, memoryFlags));
    bindMemory(memory);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(*device, handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

void Buffer::bindMemory(std::shared_ptr<DeviceMemory> memory,
    VkDeviceSize offset /* 0 */)
{
    const VkResult bind = vkBindBufferMemory(*device, handle, *memory, offset);
    MAGMA_THROW_FAILURE(bind, "failed to bind buffer memory");
    this->memory = std::move(memory);
}

VkMemoryRequirements Buffer::getMemoryRequirements() const noexcept
{
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(*device, handle, &memoryRequirements);
    return memoryRequirements;
}

VkDescriptorBufferInfo Buffer::getDescriptor() const noexcept
{
    VkDescriptorBufferInfo info;
    info.buffer = handle;
    info.offset = 0;
    info.range = VK_WHOLE_SIZE;
    return info;
}
} // namespace magma
