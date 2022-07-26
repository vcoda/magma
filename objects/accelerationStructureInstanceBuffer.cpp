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
#include "pch.h"
#pragma hdrstop
#include "accelerationStructureInstanceBuffer.h"
#include "accelerationStructure.h"
#include "deviceMemory.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
AccelerationStructureInstance::AccelerationStructureInstance() noexcept
{
    constexpr float identity[3][4] = {
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f}
    };
    memcpy(transform.matrix, identity, sizeof(identity));
    instanceCustomIndex = 0;
    mask = 0xFF;
    instanceShaderBindingTableRecordOffset = 0;
    flags = 0;
    accelerationStructureReference = 0;
}

void AccelerationStructureInstance::setAccelerationStructure(std::shared_ptr<const AccelerationStructure> accelerationStructure)
{
    accelerationStructureReference = accelerationStructure->getReferenceHandle();
}

AccelerationStructureInstanceBuffer::AccelerationStructureInstanceBuffer(std::shared_ptr<Device> device, uint32_t instanceCount,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    VkBufferCreateFlags flags /* 0 */,
    bool persistentlyMapped /* false */,
    float memoryPriority /* MAGMA_MEMORY_PRIORITY */,
    const Sharing& sharing /* default */):
    RayTracingBuffer(device, sizeof(AccelerationStructureInstance) * instanceCount, allocator, flags, memoryPriority, sharing),
    instanceCount(instanceCount),
    persistent(persistentlyMapped),
    stagingBuffer(std::make_shared<SrcTransferBuffer>(std::move(device),
        sizeof(AccelerationStructureInstance) * instanceCount, nullptr,
        std::move(allocator), 0, memoryPriority, sharing)),
    instances(nullptr)
{
    static_assert(sizeof(AccelerationStructureInstance) == sizeof(VkAccelerationStructureInstanceNV), "invalid structure size");
    if (persistentlyMapped)
    {
        instances = stagingBuffer->getMemory()->map<AccelerationStructureInstance>();
        if (!instances)
            throw exception::MemoryMapFailed("failed to map staging buffer of acceleration structure instances");
        for (uint32_t instanceIndex = 0; instanceIndex < instanceCount; ++instanceIndex)
        {   // Call constructor for each instance
            new (&instances[instanceIndex]) AccelerationStructureInstance();
        }
    } else
    {
        instanceArray.resize(instanceCount);
        instances = instanceArray.data();
    }
    for (uint32_t instanceIndex = 0; instanceIndex < instanceCount; ++instanceIndex)
        instances[instanceIndex].setInstanceIndex(instanceIndex);
}

AccelerationStructureInstanceBuffer::~AccelerationStructureInstanceBuffer()
{
    if (persistentlyMapped())
        stagingBuffer->getMemory()->unmap();
}

bool AccelerationStructureInstanceBuffer::update(std::shared_ptr<CommandBuffer> cmdBuffer,
    uint32_t firstInstance, uint32_t instanceCount)
{
    MAGMA_ASSERT(firstInstance + instanceCount > getInstanceCount());
    if (firstInstance + instanceCount > getInstanceCount())
        return false;
    const VkDeviceSize offset = sizeof(AccelerationStructureInstance) * firstInstance;
    const std::size_t size = sizeof(AccelerationStructureInstance) * instanceCount;
    if (!persistentlyMapped())
    {
        void *data = stagingBuffer->getMemory()->map(offset, size);
        MAGMA_ASSERT(data);
        if (!data)
            return false;
        // Copy data to staging buffer
        memcpy(data, &instances[firstInstance], size);
        stagingBuffer->getMemory()->unmap();
    }
    // Copy staging buffer to instance buffer
    VkBufferCopy region;
    region.srcOffset = offset;
    region.dstOffset = region.srcOffset;
    region.size = size;
    cmdBuffer->copyBuffer(stagingBuffer, shared_from_this(), region);
    // Make sure the copy to instance buffer is finished before triggering the acceleration structure build
    constexpr MemoryBarrier copyTransferBarrier(
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV);
    cmdBuffer->pipelineBarrier(
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
        copyTransferBarrier);
    return true;
}
#endif // VK_NV_ray_tracing
} // namespace magma
