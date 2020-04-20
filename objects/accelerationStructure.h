/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "resource.h"

namespace magma
{
    class Geometry;

    /* Acceleration structures are an opaque structure that can be built by the implementation
       to more efficiently perform spatial queries on the provided geometric data.
       Acceleration structure is either a top-level acceleration structure containing
       a set of bottom-level acceleration structures or a bottom-level acceleration structure
       containing either a set of axis-aligned bounding boxes for custom geometry or a set of triangles. */

#ifdef VK_NV_ray_tracing
    class AccelerationStructure : public NonDispatchableResource<AccelerationStructure, VkAccelerationStructureNV>
    {
    public:
        ~AccelerationStructure();
        void bindMemory(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            VkDeviceSize offset /* 0 */);
        VkAccelerationStructureTypeNV getType() const noexcept { return type; }
        const VkAccelerationStructureInfoNV& getInfo() const noexcept { return info; }
        std::shared_ptr<DeviceMemory> getMemory() const noexcept { return memory; }
        VkMemoryRequirements2 getMemoryRequirements() const;
        uint64_t getStructureHandle() const;

    protected:
        explicit AccelerationStructure(std::shared_ptr<Device> device,
            VkAccelerationStructureTypeNV type,
            uint32_t instanceCount,
            const std::list<Geometry>& geometries,
            VkBuildAccelerationStructureFlagsNV flags,
            VkDeviceSize compactedSize,
            std::shared_ptr<IAllocator> allocator);

    private:
        VkAccelerationStructureTypeNV type;
        VkAccelerationStructureInfoNV info;
    };

    class TopLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit TopLevelAccelerationStructure(std::shared_ptr<Device> device,
            uint32_t instanceCount,
            const std::list<Geometry>& geometries,
            VkBuildAccelerationStructureFlagsNV flags = 0,
            VkDeviceSize compactedSize = 0,
            std::shared_ptr<IAllocator> allocator = nullptr):
            AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_NV,
                instanceCount, std::move(geometries), flags, compactedSize, std::move(allocator))
        {}
    };

    class BottomLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
            uint32_t instanceCount,
            const std::list<Geometry>& geometries,
            VkBuildAccelerationStructureFlagsNV flags = 0,
            VkDeviceSize compactedSize = 0,
            std::shared_ptr<IAllocator> allocator = nullptr):
            AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_NV,
                instanceCount, std::move(geometries), flags, compactedSize, std::move(allocator))
        {}
    };
#endif // VK_NV_ray_tracing
} // namespace magma
