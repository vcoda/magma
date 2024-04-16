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
#include "baseUniformbuffer.h"
#include "device.h"
#include "physicalDevice.h"

namespace magma
{
BaseUniformBuffer::BaseUniformBuffer(std::shared_ptr<Device> device,
    std::size_t typeSize, uint32_t arraySize,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags,
    const Initializer& optional, const Sharing& sharing,
    std::shared_ptr<Allocator> allocator, bool mappedPersistently):
    Buffer(std::move(device), typeSize * arraySize, 0, // flags
        usage, memoryFlags, optional, sharing, std::move(allocator)),
    typeSize((VkDeviceSize)typeSize),
    arraySize(arraySize),
    persistent(mappedPersistently)
{
    if (persistent)
        memory->map();
}

BaseUniformBuffer::~BaseUniformBuffer()
{
    if (persistent)
        memory->unmap();
}

void *BaseUniformBuffer::map() noexcept
{
    if (persistent)
        return memory->getMapPointer();
    return memory->map(0, VK_WHOLE_SIZE);
}

void *BaseUniformBuffer::mapRange(uint32_t firstIndex /* 0 */,
    uint32_t arraySize /* std::numeric_limits<uint32_t>::max() */) noexcept
{
    if ((0 == firstIndex) && (std::numeric_limits<uint32_t>::max() == arraySize))
        return map(); // Map whole buffer
    // Map range with bounds checking
    if (arraySize == std::numeric_limits<uint32_t>::max())
        arraySize = getArraySize() - firstIndex;
    if (firstIndex + arraySize > getArraySize())
        return nullptr;
    const VkDeviceSize offset = firstIndex * getAlignment();
    const VkDeviceSize size = arraySize * getAlignment();
    MAGMA_ASSERT(size <= memory->getSize());
    if (persistent)
        return (char *)memory->getMapPointer() + offset;
    return memory->map(offset, size);
}

void *BaseUniformBuffer::mapZeroMemory() noexcept
{
    if (void *const data = map())
        return memset(data, 0, (size_t)memory->getSize());
    return nullptr;
}

void BaseUniformBuffer::unmap() noexcept
{
    if (!persistent)
    {
        if (memory->mapped())
            memory->unmap();
    }
}
} // namespace magma
