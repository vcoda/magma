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

// VK_KHR_acceleration_structure Manual Page:
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_KHR_acceleration_structure.html

#pragma once
#ifdef VK_KHR_acceleration_structure
#include "resource.h"
#include "../misc/accelerationStructureGeometry.h"

namespace magma
{
    class Buffer;
#ifdef VK_KHR_deferred_host_operations
    class DeferredOperation;
#endif

    struct AccelerationStructureBuildRange : VkAccelerationStructureBuildRangeInfoKHR
    {
        AccelerationStructureBuildRange() = default;
        constexpr AccelerationStructureBuildRange(uint32_t primitiveCount,
            uint32_t primitiveOffset = 0,
            uint32_t firstVertex = 0,
            uint32_t transformOffset = 0);
    };

    /* Acceleration structures are opaque data structures
       that are built by the implementation to more efficiently
       perform spatial queries on the provided geometric data.
       For this extension, an acceleration structure is either
       a top-level acceleration structure containing a set of
       bottom-level acceleration structures or a bottom-level
       acceleration structure containing either a set of axis-
       aligned bounding boxes for custom geometry or a set of
       triangles. */

    class AccelerationStructure : public NonDispatchableResource<AccelerationStructure, VkAccelerationStructureKHR>
    {
    public:
        struct Header;

        ~AccelerationStructure();
        VkAccelerationStructureTypeKHR getType() const noexcept { return structureType; }
        VkAccelerationStructureCreateFlagsKHR getFlags() const noexcept { return flags; }
        VkAccelerationStructureBuildTypeKHR getBuildType() const noexcept { return buildType; }
        VkBuildAccelerationStructureFlagsKHR getBuildFlags() const noexcept { return buildFlags; }
        VkDeviceSize getStructureSize() const noexcept { return accelerationStructureSize; }
        VkDeviceSize getBuildScratchSize() const noexcept { return buildScratchSize; }
        VkDeviceSize getUpdateScratchSize() const noexcept { return updateScratchSize; }
        VkDeviceAddress getDeviceAddress() const noexcept;
        VkDeviceSize getProperty(VkQueryType queryType) const noexcept;
        bool topLevel() const noexcept;
        bool bottomLevel() const noexcept;
        bool hostBuild() const noexcept;
        bool deviceBuild() const noexcept;
        void build(const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<AccelerationStructureBuildRange>& buildRanges,
            void *scratchBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr);
        bool update(const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<AccelerationStructureBuildRange>& buildRanges,
            void *scratchBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr) noexcept;
        bool clone(std::shared_ptr<AccelerationStructure> dstAccelerationStructure,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr) const noexcept;
        bool compact(std::shared_ptr<AccelerationStructure> dstAccelerationStructure,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr) const noexcept;
        bool copyToBuffer(std::shared_ptr<Buffer> dstBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr,
            VkCopyAccelerationStructureModeKHR mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR) const noexcept;
        bool copyToMemory(void *dstBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr,
            VkCopyAccelerationStructureModeKHR mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR) const noexcept;
        bool copyFromBuffer(std::shared_ptr<const Buffer> srcBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr,
            VkCopyAccelerationStructureModeKHR mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR) noexcept;
        bool copyFromMemory(const void *srcBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation = nullptr,
            VkCopyAccelerationStructureModeKHR mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR) noexcept;
        bool serialize(void *data) const noexcept;
        bool deserialize(const void *data) noexcept;

    protected:
        AccelerationStructure(std::shared_ptr<Device> device,
            VkAccelerationStructureTypeKHR structureType,
            const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<uint32_t>& maxPrimitiveCounts,
            VkAccelerationStructureCreateFlagsKHR createFlags,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator,
            const StructureChain& extendedInfo);

        VkAccelerationStructureTypeKHR structureType;

    private:
        const VkAccelerationStructureCreateFlagsKHR flags;
        const VkAccelerationStructureBuildTypeKHR buildType;
        const VkBuildAccelerationStructureFlagsKHR buildFlags;
        VkDeviceSize accelerationStructureSize;
        VkDeviceSize buildScratchSize;
        VkDeviceSize updateScratchSize;
        std::unique_ptr<Buffer> buffer;
    };

    /* The defined header structure for the serialized data consists of:
        * VK_UUID_SIZE bytes of data matching VkPhysicalDeviceIDProperties::driverUUID.
        * VK_UUID_SIZE bytes of data identifying the compatibility for comparison
          using vkGetDeviceAccelerationStructureCompatibilityKHR.
        * A 64-bit integer of the total size matching the value queried using
          VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR.
        * A 64-bit integer of the deserialized size to be passed in to
          VkAccelerationStructureCreateInfoKHR::size.
        * A 64-bit integer of the count of the number of acceleration structure
          handles following. This value matches the value queried using
          VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_BOTTOM_LEVEL_POINTERS_KHR.
          This will be zero for a bottom-level acceleration structure. For
          top-level acceleration structures this number is implementation-
          dependent; the number of and ordering of the handles may not match
          the instance descriptions which were used to build the acceleration structure. */

    struct AccelerationStructure::Header
    {
        uint8_t driverUUID[VK_UUID_SIZE];
        uint8_t compatibilityUUID[VK_UUID_SIZE];
        uint64_t serializedSize = 0ull; // Including header
        uint64_t deserializedSize = 0ull;
        uint64_t bottomLevelAccelerationStructurePointerCount = 0ull;
        VkDeviceAddress bottomLevelAccelerationStructurePointers[1]; // Array of D3D12_GPU_VIRTUAL_ADDRESS in Direct3D 12
    };

    /* Top-level acceleration structure containing instance
       data referring to bottom-level acceleration structures. */

    class TopLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit TopLevelAccelerationStructure(std::shared_ptr<Device> device,
            const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<uint32_t>& maxPrimitiveCounts,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR createFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
    };

    /* Bottom-level acceleration structure containing the AABBs
       or geometry to be intersected. */

    class BottomLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
            const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<uint32_t>& maxPrimitiveCounts,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR createFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
    };

    /* Generic acceleration structure is intended to be used
       by API translation layers. In these cases, the acceleration
       structure type is not known at creation time, but must be
       specified at build time as either top or bottom. */

    class GenericAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit GenericAccelerationStructure(std::shared_ptr<Device> device,
            const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<uint32_t>& maxPrimitiveCounts,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR createFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
        void build(VkAccelerationStructureTypeKHR structureType,
            const std::vector<AccelerationStructureGeometry>& geometries,
            const std::vector<AccelerationStructureBuildRange>& buildRanges,
            void *scratchBuffer,
            std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */);
    };
} // namespace magma

#include "accelerationStructure.inl"
#endif // VK_KHR_acceleration_structure
