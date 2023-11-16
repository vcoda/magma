/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    /* Buffer for use as a read-only input to an acceleration structure build.. */

#ifdef VK_KHR_acceleration_structure
    class AccelerationStructureBuildInputBuffer : public Buffer
    {
    public:
        explicit AccelerationStructureBuildInputBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
    };

    /* Buffer is suitable for storage space for acceleration structure. */

    class AccelerationStructureStorageBuffer : public Buffer
    {
    public:
        explicit AccelerationStructureStorageBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Descriptor& optional = Descriptor(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
