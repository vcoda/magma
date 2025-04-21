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
#include "baseUniformBuffer.h"
#include "device.h"
#include "physicalDevice.h"
#include "../exceptions/errorHandler.h"

namespace magma
{
BaseUniformBuffer::BaseUniformBuffer(std::shared_ptr<Device> device_,
    VkDeviceSize size, std::size_t typeSize, uint32_t arraySize,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags,
    const Initializer& optional, const Sharing& sharing,
    std::shared_ptr<Allocator> allocator, bool mappedPersistently):
    Buffer(std::move(device_), size, 0, // flags
        usage, memoryFlags, optional, sharing, std::move(allocator)),
    typeSize(typeSize),
    arraySize(arraySize),
    persistent(mappedPersistently),
    mapOffset(0)
{
    if (persistent)
    {
        if (!memory->map())
            MAGMA_ERROR("failed to map uniform buffer persistently");
    }
    const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
    const VkPhysicalDeviceProperties& properties = physicalDevice->getProperties();
    const VkPhysicalDeviceLimits& limits = properties.limits;
    nonCoherentAtomSize = limits.nonCoherentAtomSize;
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
    mapOffset = firstIndex * getAlignment();
    if (persistent)
        return (char *)memory->getMapPointer() + mapOffset;
    const VkDeviceSize mapSize = arraySize * getAlignment();
    MAGMA_ASSERT(mapSize <= memory->getSize());
    return memory->map(mapOffset, mapSize);
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
        {
            memory->unmap();
            mapOffset = 0;
        }
    }
}
} // namespace magma
