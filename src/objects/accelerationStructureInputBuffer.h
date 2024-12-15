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
#pragma once
#include "buffer.h"

namespace magma
{
    /* An array of vertices, indices, AABBs or geometry instances
       used as a read-only input to an acceleration structure build. */

#ifdef VK_KHR_acceleration_structure
    class AccelerationStructureInputBuffer : public Buffer
    {
    public:
        explicit AccelerationStructureInputBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
        explicit AccelerationStructureInputBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());

    protected:
        AccelerationStructureInputBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
