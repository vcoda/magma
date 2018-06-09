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
#include "srcTransferBuffer.h"
#include "deviceMemory.h"
#include "../mem/copyMemory.h"
#include "../helpers/mapScoped.h"

namespace magma
{
SrcTransferBuffer::SrcTransferBuffer(std::shared_ptr<const Device> device,
    VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Buffer(std::move(device), size, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        flags, std::move(allocator),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{}

SrcTransferBuffer::SrcTransferBuffer(std::shared_ptr<const Device> device,
    const void *data, VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    SrcTransferBuffer(std::move(device), size, flags, std::move(allocator))
{   
    if (data)
    {
        if (!copyFn)
            copyFn = copyMemory;
        helpers::mapScoped<void>(shared_from_this(), [&copyFn, data, size](void *buffer) 
        {
            copyFn(buffer, data, static_cast<size_t>(size));
        });
    }
}
} // namespace magma
