/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
    accelerationStructureReference = VK_NULL_HANDLE;
}

void AccelerationStructureInstance::setAccelerationStructure(std::shared_ptr<const AccelerationStructure> accelerationStructure)
{
    accelerationStructureReference = accelerationStructure->getReferenceHandle();
}

AccelerationStructureInstanceBuffer::AccelerationStructureInstanceBuffer(std::shared_ptr<Device> device, uint32_t instanceCount,
    VkBufferCreateFlags flags /* 0 */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    RayTracingBuffer(device, 
        sizeof(AccelerationStructureInstance) * instanceCount,
        flags, sharing, allocator),
    stagingBuffer(std::make_shared<SrcTransferBuffer>(std::move(device),
        sizeof(AccelerationStructureInstance) * instanceCount, nullptr,
        flags, sharing, std::move(allocator))),
    instances(nullptr),
    instanceCount(instanceCount)
{
    static_assert(sizeof(AccelerationStructureInstance) == sizeof(VkAccelerationStructureInstanceKHR), "invalid structure size");
    instances = stagingBuffer->getMemory()->map<AccelerationStructureInstance>();
    if (!instances)
        throw exception::MemoryMapFailed("failed to map staging buffer of acceleration structure instances");
    for (uint32_t instanceIndex = 0; instanceIndex < instanceCount; ++instanceIndex)
    {   
        new (&instances[instanceIndex]) AccelerationStructureInstance();
        instances[instanceIndex].setInstanceIndex(instanceIndex);
    }
}

AccelerationStructureInstanceBuffer::~AccelerationStructureInstanceBuffer()
{
    if (stagingBuffer->getMemory()->hostMapped())
        stagingBuffer->getMemory()->unmap();
}

bool AccelerationStructureInstanceBuffer::update(std::shared_ptr<CommandBuffer> cmdBuffer,
    uint32_t firstInstance, uint32_t instanceCount)
{
    if (firstInstance + instanceCount > getInstanceCount())
        return false;
    // Copy staging buffer to instance buffer
    VkBufferCopy region;
    region.srcOffset = firstInstance * sizeof(AccelerationStructureInstance);
    region.dstOffset = region.srcOffset;
    region.size = sizeof(AccelerationStructureInstance) * instanceCount;
    cmdBuffer->copyBuffer(stagingBuffer, shared_from_this(), region);
    // Make sure the copy to instance buffer is finished before triggering the acceleration structure build
    constexpr magma::MemoryBarrier copyTransferBarrier(
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
