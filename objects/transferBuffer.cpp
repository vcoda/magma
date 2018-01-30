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
#include "transferBuffer.h"
#include "../helpers/alignedMemcpy.h"

namespace magma
{
SourceTransferBuffer::SourceTransferBuffer(std::shared_ptr<const Device> device,
    VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */):
    Buffer(device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, flags,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{}

SourceTransferBuffer::SourceTransferBuffer(std::shared_ptr<const Device> device,
    const void *data, VkDeviceSize size,
    VkBufferCreateFlags flags /* 0 */):
    SourceTransferBuffer(device, size, flags)
{   
    if (void *buffer = memory->map(0, size))
    {
        helpers::alignedMemcpy(buffer, data, static_cast<size_t>(size));
        memory->unmap();
    }
}

SourceTransferBuffer::SourceTransferBuffer(std::shared_ptr<const Device> device,
    const std::vector<uint8_t>& data,
    VkBufferCreateFlags flags /* 0 */):
    SourceTransferBuffer(device, data.data(), static_cast<VkDeviceSize>(data.size()), flags)
{}
} // namespace magma
