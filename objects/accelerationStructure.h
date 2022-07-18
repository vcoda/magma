/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../misc/geometry.h"

namespace magma
{
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
        bool topLevel() const noexcept { return VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_NV == info.type; }
        bool bottomLevel() const noexcept { return VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_NV == info.type; }
        const VkAccelerationStructureInfoNV& getInfo() const noexcept { return info; }
        std::shared_ptr<DeviceMemory> getMemory() const noexcept { return memory; }
        VkMemoryRequirements getObjectMemoryRequirements() const;
        VkMemoryRequirements getBuildScratchMemoryRequirements() const;
        VkMemoryRequirements getUpdateScratchMemoryRequirements() const;
        uint64_t getReferenceHandle() const;
        virtual void bindMemory(std::shared_ptr<DeviceMemory> memory,
            VkDeviceSize offset = 0) override;
#ifdef VK_KHR_device_group
        virtual void bindMemoryDeviceGroup(std::shared_ptr<DeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            VkDeviceSize offset = 0) override;
#endif
        virtual void onDefragment() override;

    protected:
        explicit AccelerationStructure(std::shared_ptr<Device> device,
            VkAccelerationStructureTypeNV type,
            uint32_t instanceCount,
            const std::list<Geometry>& geometries,
            VkBuildAccelerationStructureFlagsNV flags,
            VkDeviceSize compactedSize,
            float memoryPriority,
            std::shared_ptr<Allocator> allocator);

    private:
        VkMemoryRequirements2 getMemoryRequirements(VkAccelerationStructureMemoryRequirementsTypeNV type) const;

        VkAccelerationStructureInfoNV info;
    };

    /* Top-level acceleration structure containing instance data referring to bottom-level acceleration structures.*/

    class TopLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit TopLevelAccelerationStructure(std::shared_ptr<Device> device,
            uint32_t instanceCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBuildAccelerationStructureFlagsNV flags = 0,
            VkDeviceSize compactedSize = 0,
            float memoryPriority = 0.f):
            AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_NV,
                instanceCount, std::list<Geometry>{}, flags, compactedSize, memoryPriority, std::move(allocator))
        {}
    };

    /* Bottom-level acceleration structure containing the AABBs or geometry to be intersected. */

    class BottomLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
            const std::list<Geometry>& geometries,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBuildAccelerationStructureFlagsNV flags = 0,
            VkDeviceSize compactedSize = 0,
            float memoryPriority = 0.f):
            AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_NV,
                0, geometries, flags, compactedSize, memoryPriority, std::move(allocator))
        {}
    };
#endif // VK_NV_ray_tracing
} // namespace magma
