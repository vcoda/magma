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
#include "accelerationStructure.h"

namespace magma
{
    /* Bottom-level acceleration structure containing the AABBs
       or geometry to be intersected. */

#ifdef VK_KHR_acceleration_structure
    class BottomLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
            const std::list<AccelerationStructureGeometry>& geometries,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR flags = 0,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        explicit BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
            VkDeviceSize deserializedSize,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR flags = 0,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        uint64_t getReference() const noexcept;
        void build(const std::list<AccelerationStructureGeometry>& geometries,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr);
        void build(const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr);
        void update(const std::list<AccelerationStructureGeometry>& geometries,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr);
        void update(const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr);

    private:
        VkResult rebuild(VkBuildAccelerationStructureModeKHR mode,
            const std::list<AccelerationStructureGeometry>& geometries,
            const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation);
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
