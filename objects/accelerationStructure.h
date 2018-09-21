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
#include <vector>
#include "handle.h"

namespace magma
{
    class Geometry;

    /* Acceleration structures are an opaque structure that can be built by the implementation
       to more efficiently perform spatial queries on the provided geometric data.
       Acceleration structure is either a top-level acceleration structure containing
       a set of bottom-level acceleration structures or a bottom-level acceleration structure
       containing either a set of axis-aligned bounding boxes for custom geometry or a set of triangles. */

    class AccelerationStructure : public NonDispatchable<VkAccelerationStructureNVX>
    {
    public:
        AccelerationStructure(std::shared_ptr<Device> device,
            VkDeviceSize compactedSize,
            uint32_t instanceCount,
            const std::vector<std::shared_ptr<Geometry>>& geometries,
            VkBuildAccelerationStructureFlagsNVX flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~AccelerationStructure();
    };
} // namespace magma
