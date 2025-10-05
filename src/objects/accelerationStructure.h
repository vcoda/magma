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
#include "model/nondispatchable.h"
#include "resource.h"
#include "queryPool.h"

namespace magma
{
    class Buffer;
#ifdef VK_KHR_acceleration_structure
    struct AccelerationStructureGeometry;
    struct AccelerationStructureInstances;
    struct AccelerationStructureBuildRange;
#endif
#ifdef VK_KHR_deferred_host_operations
    class DeferredOperation;
#endif

    /* Acceleration structures are opaque data structures that
       are built by the implementation to more efficiently
       perform spatial queries on the provided geometric data.
       For this extension, an acceleration structure is either
       a top-level acceleration structure containing a set of
       bottom-level acceleration structures or a bottom-level
       acceleration structure containing either a set of axis-
       aligned bounding boxes for custom geometry or a set of
       triangles. */

#ifdef VK_KHR_acceleration_structure
    class AccelerationStructure : public Resource,
        public NonDispatchable<VkAccelerationStructureKHR>
    {
    public:
        ~AccelerationStructure();
        VkAccelerationStructureTypeKHR getType() const noexcept { return structureType; }
        VkAccelerationStructureCreateFlagsKHR getFlags() const noexcept { return flags; }
        VkAccelerationStructureBuildTypeKHR getBuildType() const noexcept { return buildType; }
        VkBuildAccelerationStructureFlagsKHR getBuildFlags() const noexcept { return buildFlags; }
        uint32_t getGeometryCount() const noexcept { return geometryCount; }
        Family getFamily() const noexcept override final { return Family::AccelerationStructure; }
        VkDeviceSize getBuildScratchSize() const noexcept { return buildScratchSize; }
        VkDeviceSize getUpdateScratchSize() const noexcept { return updateScratchSize; }
        VkDeviceSize getProperty(AccelerationStructureQuery::Type queryType) const noexcept;
        VkMemoryRequirements getMemoryRequirements() const noexcept override;
    #ifdef VK_KHR_get_memory_requirements2
        VkMemoryRequirements getMemoryRequirements2(void *memoryRequirements) const override;
    #endif
        void bindMemory(std::unique_ptr<IDeviceMemory> memory,
            VkDeviceSize offset = 0) override;
    #ifdef VK_KHR_device_group
        void bindMemoryDeviceGroup(std::unique_ptr<IDeviceMemory> memory,
            const std::vector<uint32_t>& deviceIndices,
            const std::vector<VkRect2D>& splitInstanceBindRegions = {},
            VkDeviceSize offset = 0) override;
    #endif // VK_KHR_device_group
        VkDeviceAddress getDeviceAddress() const noexcept override;
        bool copy(lent_ptr<AccelerationStructure> dstAccelerationStructure,
            lent_ptr<DeferredOperation> deferredOperation = nullptr) const noexcept;
        bool compact(lent_ptr<AccelerationStructure> dstAccelerationStructure,
            lent_ptr<DeferredOperation> deferredOperation = nullptr) const noexcept;
        bool copyTo(void *dstBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr) const noexcept;
        bool copyFrom(const void *srcBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr) noexcept;
        bool serialize(void *dstBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr) const noexcept;
        bool deserialize(const void *srcBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr) noexcept;
        void onDefragment() override;

    protected:
        AccelerationStructure(std::shared_ptr<Device> device,
            VkAccelerationStructureTypeKHR structureType,
            VkAccelerationStructureCreateFlagsKHR flags,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            const std::list<AccelerationStructureGeometry>& geometries,
            std::shared_ptr<Allocator> allocator,
            const Sharing& sharing,
            const StructureChain& extendedInfo);
        AccelerationStructure(std::shared_ptr<Device> device,
            VkAccelerationStructureTypeKHR structureType,
            VkAccelerationStructureCreateFlagsKHR flags,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            VkDeviceSize deserializedSize,
            std::shared_ptr<Allocator> allocator,
            const Sharing& sharing,
            const StructureChain& extendedInfo);

        VkAccelerationStructureTypeKHR structureType;
        const VkAccelerationStructureCreateFlagsKHR flags;
        const VkAccelerationStructureBuildTypeKHR buildType;
        const VkBuildAccelerationStructureFlagsKHR buildFlags;
        uint32_t geometryCount;
        VkDeviceSize buildScratchSize;
        VkDeviceSize updateScratchSize;
        std::unique_ptr<Buffer> buffer;
    };

    /* Generic acceleration structure is intended to be used
       by API translation layers. In these cases, the acceleration
       structure type is not known at creation time, but must be
       specified at build time as either top or bottom. */

    class GenericAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit GenericAccelerationStructure(std::shared_ptr<Device> device,
            const std::list<AccelerationStructureGeometry>& geometries,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR createFlags = 0,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
