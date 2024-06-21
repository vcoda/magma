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
#include "commandBuffer.h"
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
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    VkAccelerationStructureCreateFlagsKHR flags, const std::forward_list<AccelerationStructureGeometry>& geometries,
    std::shared_ptr<Allocator> allocator, const StructureChain& extendedInfo):
    NonDispatchableResource(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, device, 0, sharing, allocator),
    structureType(structureType),
    buildType(buildType),
    buildFlags(buildFlags),
    flags(flags),
    updateScratchSize(0),
    buildScratchSize(0)
{
    const size_t geometryCount = std::distance(geometries.begin(), geometries.end());
    MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
    MAGMA_STACK_ARRAY(uint32_t, primitiveCounts, geometryCount);
    for (auto const& geometry: geometries)
    {
        geometryPointers.put(&geometry);
        primitiveCounts.put(geometry.primitiveCount);
    }
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = structureType;
    buildGeometryInfo.flags = buildFlags;
    buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.dstAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.geometryCount = geometryPointers.size();
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.hostAddress = nullptr;
    VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo = {VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR};
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetAccelerationStructureBuildSizesKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    vkGetAccelerationStructureBuildSizesKHR(getNativeDevice(), buildType, &buildGeometryInfo, primitiveCounts, &buildSizesInfo);
    // Allocate buffer where the acceleration structure will be stored
    buffer = std::make_unique<AccelerationStructureStorageBuffer>(std::move(device),
        buildSizesInfo.accelerationStructureSize, std::move(allocator));
    VkAccelerationStructureCreateInfoKHR accelerationStructureInfo;
    accelerationStructureInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
    accelerationStructureInfo.pNext = extendedInfo.headNode();
    accelerationStructureInfo.createFlags = flags;
    accelerationStructureInfo.buffer = buffer->getHandle();
    accelerationStructureInfo.offset = 0ull;
    accelerationStructureInfo.size = buildSizesInfo.accelerationStructureSize;
    accelerationStructureInfo.type = structureType;
    accelerationStructureInfo.deviceAddress = 0ull;
    // Create the acceleration structure
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateAccelerationStructureKHR, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
    const VkResult result = vkCreateAccelerationStructureKHR(getNativeDevice(), &accelerationStructureInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create acceleration structure");
    size = buildSizesInfo.accelerationStructureSize;
    buildScratchSize = buildSizesInfo.buildScratchSize;
    updateScratchSize = buildSizesInfo.updateScratchSize;
}

AccelerationStructure::~AccelerationStructure()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyAccelerationStructureKHR);
    vkDestroyAccelerationStructureKHR(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
VkDeviceAddress AccelerationStructure::getDeviceAddress() const noexcept
{
    VkAccelerationStructureDeviceAddressInfoKHR deviceAddressInfo;
    deviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
    deviceAddressInfo.pNext = nullptr;
    deviceAddressInfo.accelerationStructure = handle;
    MAGMA_DEVICE_EXTENSION(vkGetAccelerationStructureDeviceAddressKHR);
    return vkGetAccelerationStructureDeviceAddressKHR(getNativeDevice(), &deviceAddressInfo);
}
#endif // VK_KHR_buffer_device_address || VK_EXT_buffer_device_address

VkDeviceSize AccelerationStructure::getProperty(VkQueryType queryType) const noexcept
{
    MAGMA_ASSERT((VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR == queryType) ||
        (VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR == queryType));
#ifdef VK_KHR_ray_tracing_maintenance1
    MAGMA_ASSERT((VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SIZE_KHR == queryType) ||
        (VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_BOTTOM_LEVEL_POINTERS_KHR == queryType));
#endif // VK_KHR_ray_tracing_maintenance1
    VkDeviceSize property = 0;
    MAGMA_DEVICE_EXTENSION(vkWriteAccelerationStructuresPropertiesKHR);
    const VkResult result = vkWriteAccelerationStructuresPropertiesKHR(getNativeDevice(),
        1, &handle, queryType, sizeof(VkDeviceSize), &property, sizeof(VkDeviceSize));
    MAGMA_ASSERT(MAGMA_SUCCEEDED(result));
    return property;
}

void AccelerationStructure::build(const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges, void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
    const size_t geometryCount = std::distance(geometries.begin(), geometries.end());
    MAGMA_ASSERT(buildRanges.size() >= geometryCount);
    MAGMA_ASSERT(VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR == buildType);
    MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
    for (auto const& geometry: geometries)
        geometryPointers.put(&geometry);
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = structureType;
    buildGeometryInfo.flags = buildFlags;
    buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.dstAccelerationStructure = handle;
    buildGeometryInfo.geometryCount = geometryPointers.size();
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.hostAddress = scratchBuffer;
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos[] = {buildRanges.data()};
    MAGMA_DEVICE_EXTENSION(vkBuildAccelerationStructuresKHR);
    const VkResult result = vkBuildAccelerationStructuresKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), 1, &buildGeometryInfo, buildRangeInfos);
    MAGMA_HANDLE_RESULT(result, "failed to build acceleration structure");
}

bool AccelerationStructure::update(const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */) noexcept
{
    const size_t geometryCount = std::distance(geometries.begin(), geometries.end());
    MAGMA_ASSERT(buildRanges.size() >= geometryCount);
    MAGMA_ASSERT(VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR == buildType);
    MAGMA_STACK_ARRAY(const VkAccelerationStructureGeometryKHR*, geometryPointers, geometryCount);
    for (auto const& geometry: geometries)
        geometryPointers.put(&geometry);
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = structureType;
    buildGeometryInfo.flags = buildFlags;
    buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
    buildGeometryInfo.srcAccelerationStructure = handle;
    buildGeometryInfo.dstAccelerationStructure = handle;
    buildGeometryInfo.geometryCount = geometryPointers.size();
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometryPointers;
    buildGeometryInfo.scratchData.hostAddress = scratchBuffer;
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos[] = {buildRanges.data()};
    MAGMA_DEVICE_EXTENSION(vkBuildAccelerationStructuresKHR);
    const VkResult result = vkBuildAccelerationStructuresKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), 1, &buildGeometryInfo, buildRangeInfos);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::clone(std::shared_ptr<AccelerationStructure> dstAccelerationStructure,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */) const noexcept
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

bool AccelerationStructure::compact(std::shared_ptr<AccelerationStructure> dstAccelerationStructure,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */) const noexcept
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

bool AccelerationStructure::copyToBuffer(std::shared_ptr<Buffer> dstBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */,
    VkCopyAccelerationStructureModeKHR mode /* VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR */) const noexcept
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = handle;
    copyMemoryInfo.dst.deviceAddress = dstBuffer->getDeviceAddress();
    copyMemoryInfo.mode = mode;
    MAGMA_DEVICE_EXTENSION(vkCopyAccelerationStructureToMemoryKHR);
    const VkResult result = vkCopyAccelerationStructureToMemoryKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::copyToMemory(void *dstBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */,
    VkCopyAccelerationStructureModeKHR mode /* VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR */) const noexcept
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = handle;
    copyMemoryInfo.dst.hostAddress = dstBuffer;
    copyMemoryInfo.mode = mode;
    MAGMA_DEVICE_EXTENSION(vkCopyAccelerationStructureToMemoryKHR);
    const VkResult result = vkCopyAccelerationStructureToMemoryKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::copyFromBuffer(std::shared_ptr<const Buffer> srcBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */,
    VkCopyAccelerationStructureModeKHR mode /* VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR */) noexcept
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.deviceAddress = srcBuffer->getDeviceAddress();
    copyMemoryInfo.dst = handle;
    copyMemoryInfo.mode = mode;
    MAGMA_DEVICE_EXTENSION(vkCopyMemoryToAccelerationStructureKHR);
    const VkResult result = vkCopyMemoryToAccelerationStructureKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

bool AccelerationStructure::copyFromMemory(const void *srcBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */,
    VkCopyAccelerationStructureModeKHR mode /* VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR */) noexcept
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.hostAddress = srcBuffer;
    copyMemoryInfo.dst = handle;
    copyMemoryInfo.mode = mode;
    MAGMA_DEVICE_EXTENSION(vkCopyMemoryToAccelerationStructureKHR);
    const VkResult result = vkCopyMemoryToAccelerationStructureKHR(getNativeDevice(),
        MAGMA_OPTIONAL_HANDLE(deferredOperation), &copyMemoryInfo);
    return MAGMA_SUCCEEDED(result);
}

void AccelerationStructure::bindMemory(std::shared_ptr<IDeviceMemory> deviceMemory,
    VkDeviceSize offset /* 0 */)
{
    buffer->bindMemory(std::move(deviceMemory), offset);
}

#ifdef VK_KHR_device_group
void AccelerationStructure::bindMemoryDeviceGroup(std::shared_ptr<IDeviceMemory> deviceMemory,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions,
    VkDeviceSize offset /* 0 */)
{
    buffer->bindMemoryDeviceGroup(std::move(deviceMemory), deviceIndices, splitInstanceBindRegions, offset);
}
#endif // VK_KHR_device_group

void AccelerationStructure::onDefragment()
{
    buffer->onDefragment();
}

TopLevelAccelerationStructure::TopLevelAccelerationStructure(std::shared_ptr<Device> device,
    const AccelerationStructureGeometry& instances,
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkAccelerationStructureCreateFlagsKHR flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
        buildType, buildFlags, flags, {instances}, std::move(allocator), extendedInfo)
{}

BottomLevelAccelerationStructure::BottomLevelAccelerationStructure(std::shared_ptr<Device> device,
    const std::forward_list<AccelerationStructureGeometry>& geometries,
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkAccelerationStructureCreateFlagsKHR flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
        buildType, buildFlags, flags, geometries, std::move(allocator), extendedInfo)
{}

GenericAccelerationStructure::GenericAccelerationStructure(std::shared_ptr<Device> device,
    const std::forward_list<AccelerationStructureGeometry>& geometries,
    VkAccelerationStructureBuildTypeKHR buildType, VkBuildAccelerationStructureFlagsKHR buildFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkAccelerationStructureCreateFlagsKHR flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    AccelerationStructure(std::move(device), VK_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR,
        buildType, buildFlags, flags, geometries, std::move(allocator), extendedInfo)
{}

void GenericAccelerationStructure::build(VkAccelerationStructureTypeKHR structureType_,
    const std::forward_list<AccelerationStructureGeometry>& geometries,
    const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildRanges,
    void *scratchBuffer,
    std::shared_ptr<DeferredOperation> deferredOperation /* nullptr */)
{
#ifndef MAGMA_NO_EXCEPTIONS
    if (VK_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR == structureType_)
        throw std::invalid_argument("acceleration structure type must be specified at build time as either top or bottom");
#endif //  MAGMA_NO_EXCEPTIONS
    structureType = structureType_;
    AccelerationStructure::build(geometries, buildRanges, scratchBuffer, std::move(deferredOperation));
}
#endif // VK_KHR_acceleration_structure
} // namespace magma
