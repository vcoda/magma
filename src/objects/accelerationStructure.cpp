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
#include "pch.h"
#pragma hdrstop
#include "accelerationStructure.h"
#include "device.h"
#include "storageBuffer.h"
#include "deferredOperation.h"
#include "../raytracing/accelerationStructureGeometry.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_acceleration_structure
AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device, VkAccelerationStructureTypeKHR structureType,
    VkAccelerationStructureCreateFlagsKHR flags, VkAccelerationStructureBuildTypeKHR buildType,
    VkBuildAccelerationStructureFlagsKHR buildFlags, const std::list<AccelerationStructureGeometry>& geometries,
    std::shared_ptr<Allocator> allocator, const Sharing& sharing, const StructureChain& extendedInfo):
    NonDispatchable<VkAccelerationStructureKHR>(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, device, MAGMA_HOST_ALLOCATOR(allocator)),
    Resource(0, sharing),
    structureType(structureType),
    flags(flags),
    buildType(buildType),
    buildFlags(buildFlags),
    geometryCount(core::countof(geometries)),
    buildScratchSize(0ull),
    updateScratchSize(0ull)
{
    auto geometryPointers = stackalloc(const VkAccelerationStructureGeometryKHR*, geometryCount);
    auto maxPrimitiveCounts = stackalloc(uint32_t, geometryCount);
    uint32_t i = 0;
    for (auto const& geometry: geometries)
    {
        geometryPointers[i] = &geometry;
        maxPrimitiveCounts[i++] = geometry.primitiveCount;
    }
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = structureType;
    buildGeometryInfo.flags = buildFlags;
    buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.dstAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.geometryCount = geometryCount;
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.hostAddress = nullptr;
    VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetAccelerationStructureBuildSizesKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    vkGetAccelerationStructureBuildSizesKHR(getNativeDevice(), buildType, &buildGeometryInfo, maxPrimitiveCounts, &buildSizesInfo);
    // Allocate buffer where the acceleration structure will be stored
    buffer = std::make_unique<AccelerationStructureStorageBuffer>(std::move(device),
        buildSizesInfo.accelerationStructureSize, buildType, std::move(allocator), Buffer::Initializer(), sharing);
    VkAccelerationStructureCreateInfoKHR accelerationStructureInfo;
    accelerationStructureInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
    accelerationStructureInfo.pNext = extendedInfo.headNode();
    accelerationStructureInfo.createFlags = flags;
    accelerationStructureInfo.buffer = *buffer;
    accelerationStructureInfo.offset = 0ull;
    accelerationStructureInfo.size = buildSizesInfo.accelerationStructureSize;
    accelerationStructureInfo.type = structureType;
    accelerationStructureInfo.deviceAddress = MAGMA_NULL;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateAccelerationStructureKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    const VkResult result = vkCreateAccelerationStructureKHR(getNativeDevice(), &accelerationStructureInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create acceleration structure");
    size = buildSizesInfo.accelerationStructureSize;
    buildScratchSize = buildSizesInfo.buildScratchSize;
    updateScratchSize = buildSizesInfo.updateScratchSize;
}

AccelerationStructure::AccelerationStructure(std::shared_ptr<Device> device, VkAccelerationStructureTypeKHR structureType,
    VkAccelerationStructureCreateFlagsKHR flags, VkAccelerationStructureBuildTypeKHR buildType,
    VkBuildAccelerationStructureFlagsKHR buildFlags, VkDeviceSize deserializedSize,
    std::shared_ptr<Allocator> allocator, const Sharing& sharing, const StructureChain& extendedInfo):
    NonDispatchable<VkAccelerationStructureKHR>(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, device, MAGMA_HOST_ALLOCATOR(allocator)),
    Resource(0, sharing),
    structureType(structureType),
    flags(flags),
    buildType(buildType),
    buildFlags(buildFlags),
    geometryCount(1),
    buildScratchSize(deserializedSize),
    updateScratchSize(deserializedSize)
{
    buffer = std::make_unique<AccelerationStructureStorageBuffer>(std::move(device),
        deserializedSize, buildType, std::move(allocator), Buffer::Initializer(), sharing);
    VkAccelerationStructureCreateInfoKHR accelerationStructureInfo;
    accelerationStructureInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
    accelerationStructureInfo.pNext = extendedInfo.headNode();
    accelerationStructureInfo.createFlags = flags;
    accelerationStructureInfo.buffer = *buffer;
    accelerationStructureInfo.offset = 0ull;
    accelerationStructureInfo.size = deserializedSize;
    accelerationStructureInfo.type = structureType;
    accelerationStructureInfo.deviceAddress = MAGMA_NULL;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateAccelerationStructureKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    const VkResult result = vkCreateAccelerationStructureKHR(getNativeDevice(), &accelerationStructureInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create acceleration structure");
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureKHR);
    vkDestroyAccelerationStructureKHR(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}

VkDeviceSize AccelerationStructure::getProperty(AccelerationStructureQuery::Type queryType) const noexcept
{
    auto queryTypeVk = [queryType]() -> VkQueryType
    {
        switch (queryType)
        {
        case AccelerationStructureQuery::Type::CompactedSize: return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
        case AccelerationStructureQuery::Type::SerializationSize: return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR;
    #ifdef VK_KHR_ray_tracing_maintenance1
        case AccelerationStructureQuery::Type::Size: return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SIZE_KHR;
        case AccelerationStructureQuery::Type::BottomLevelPointers: return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_BOTTOM_LEVEL_POINTERS_KHR;
    #endif // VK_KHR_ray_tracing_maintenance1
        default: return VK_QUERY_TYPE_MAX_ENUM;
        }
    };
    VkDeviceSize property = 0ull;
    MAGMA_DEVICE_EXTENSION(vkWriteAccelerationStructuresPropertiesKHR);
    const VkResult result = vkWriteAccelerationStructuresPropertiesKHR(getNativeDevice(),
        1, &handle, queryTypeVk(), sizeof(VkDeviceSize), &property, sizeof(VkDeviceSize));
    MAGMA_ASSERT(VK_SUCCESS == result);
    MAGMA_UNUSED(result);
    return property;
}

VkMemoryRequirements AccelerationStructure::getMemoryRequirements() const noexcept
{
    return buffer->getMemoryRequirements();
}

#ifdef VK_KHR_get_memory_requirements2
VkMemoryRequirements AccelerationStructure::getMemoryRequirements2(void *memoryRequirements) const
{
    return buffer->getMemoryRequirements2(memoryRequirements);
}
#endif // VK_KHR_get_memory_requirements2

void AccelerationStructure::bindMemory(std::unique_ptr<IDeviceMemory> deviceMemory,
    VkDeviceSize offset /* 0 */)
{
    buffer->bindMemory(std::move(deviceMemory), offset);
}

#ifdef VK_KHR_device_group
void AccelerationStructure::bindMemoryDeviceGroup(std::unique_ptr<IDeviceMemory> deviceMemory,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions,
    VkDeviceSize offset /* 0 */)
{
    buffer->bindMemoryDeviceGroup(std::move(deviceMemory), deviceIndices, splitInstanceBindRegions, offset);
}
#endif // VK_KHR_device_group

VkDeviceAddress AccelerationStructure::getDeviceAddress() const noexcept
{
    VkAccelerationStructureDeviceAddressInfoKHR deviceAddressInfo;
    deviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
    deviceAddressInfo.pNext = nullptr;
    deviceAddressInfo.accelerationStructure = handle;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureDeviceAddressKHR);
    return vkGetAccelerationStructureDeviceAddressKHR(getNativeDevice(), &deviceAddressInfo);
}

bool AccelerationStructure::copy(lent_ptr<AccelerationStructure> dstAccelerationStructure,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */) const noexcept
{
    VkCopyAccelerationStructureInfoKHR copyInfo;
    copyInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
    copyInfo.pNext = nullptr;
    copyInfo.src = handle;
    copyInfo.dst = dstAccelerationStructure->handle;
    copyInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCopyAccelerationStructureKHR);
    const VkResult result = vkCopyAccelerationStructureKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::compact(lent_ptr<AccelerationStructure> dstAccelerationStructure,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */) const noexcept
{
    VkCopyAccelerationStructureInfoKHR copyInfo;
    copyInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
    copyInfo.pNext = nullptr;
    copyInfo.src = handle;
    copyInfo.dst = dstAccelerationStructure->handle;
    copyInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;
    MAGMA_DEVICE_EXTENSION(vkCopyAccelerationStructureKHR);
    const VkResult result = vkCopyAccelerationStructureKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::copyTo(void *dstBuffer,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */) const noexcept
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = handle;
    copyMemoryInfo.dst.hostAddress = dstBuffer;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCopyAccelerationStructureToMemoryKHR);
    const VkResult result = vkCopyAccelerationStructureToMemoryKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::copyFrom(const void *srcBuffer,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */) noexcept
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.hostAddress = srcBuffer;
    copyMemoryInfo.dst = handle;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCopyMemoryToAccelerationStructureKHR);
    const VkResult result = vkCopyMemoryToAccelerationStructureKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::serialize(void *dstBuffer,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */) const noexcept
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = handle;
    copyMemoryInfo.dst.hostAddress = dstBuffer;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_SERIALIZE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCopyAccelerationStructureToMemoryKHR);
    const VkResult result = vkCopyAccelerationStructureToMemoryKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::deserialize(const void *srcBuffer,
    lent_ptr<DeferredOperation> deferredOperation /* nullptr */) noexcept
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.hostAddress = srcBuffer;
    copyMemoryInfo.dst = handle;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_DESERIALIZE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCopyMemoryToAccelerationStructureKHR);
    const VkResult result = vkCopyMemoryToAccelerationStructureKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

void AccelerationStructure::onDefragment()
{
    buffer->onDefragment();
}

GenericAccelerationStructure::GenericAccelerationStructure(std::shared_ptr<Device> device,
    const std::list<AccelerationStructureGeometry>& geometries,
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkAccelerationStructureCreateFlagsKHR flags /* 0 */,
    const Sharing& sharing /* default */,
    const StructureChain& extendedInfo /* default */):
    AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR,
        flags, buildType, buildFlags, geometries, std::move(allocator), sharing, extendedInfo)
{}
#endif // VK_KHR_acceleration_structure
} // namespace magma
