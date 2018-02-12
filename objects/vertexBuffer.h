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
#pragma once
#include "buffer.h"
#include "device.h"

namespace magma
{
    class CommandBuffer;

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(std::shared_ptr<const Device> device,
            const void *data, VkDeviceSize size,
            uint32_t vertexCount,
            VkBufferCreateFlags flags = 0);
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const void *data, VkDeviceSize size,
            uint32_t vertexCount,  
            VkBufferCreateFlags flags = 0);

        template <typename VertexType>
        VertexBuffer(std::shared_ptr<const Device> device,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0):
            VertexBuffer(device, vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()), 
                static_cast<uint32_t>(vertices.size()), flags)
        {}

        template <typename VertexType>
        VertexBuffer(std::shared_ptr<CommandBuffer> copyCmdBuffer,
            const std::vector<VertexType>& vertices,
            VkBufferCreateFlags flags = 0):
            VertexBuffer(copyCmdBuffer, vertices.data(), static_cast<VkDeviceSize>(sizeof(VertexType) * vertices.size()), 
                static_cast<uint32_t>(vertices.size()), flags)
        {}

        uint32_t getVertexCount() const { return vertexCount; }

    protected:
        VertexBuffer(std::shared_ptr<const Device> device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags,
            VkMemoryPropertyFlags memoryFlags);

    private:
        uint32_t vertexCount;
    };
} // namespace magma
