/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "leanCommandBuffer.h"
#include "device.h"
#include "commandPool.h"
#include "framebuffer.h"
#include "imagelessFramebuffer.h"
#include "renderPass.h"
#include "imageView.h"
#include "bottomLevelStructure.h"
#include "topLevelStructure.h"
#include "clusterAccelerationStructure.h"
#include "../shaders/shaderBindingTable.h"
#include "../raytracing/accelerationStructureGeometry.h"
#include "../raytracing/accelerationStructureTriangleCluster.h"
#include "../exceptions/errorResult.h"

namespace magma
{
LeanCommandBuffer::LeanCommandBuffer(VkCommandBuffer handle, const CommandPool *cmdPool) noexcept:
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER, handle),
    device(cmdPool->getDevice()->getHandle()),
    cmdPool(cmdPool->getHandle())
{}

LeanCommandBuffer::LeanCommandBuffer(VkCommandBufferLevel level, const CommandPool *cmdPool):
    Dispatchable(VK_OBJECT_TYPE_COMMAND_BUFFER),
    device(cmdPool->getDevice()->getHandle()),
    cmdPool(cmdPool->getHandle())
{
    VkCommandBufferAllocateInfo cmdBufferAllocateInfo;
    cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocateInfo.pNext = nullptr;
    cmdBufferAllocateInfo.commandPool = *cmdPool;
    cmdBufferAllocateInfo.level = level;
    cmdBufferAllocateInfo.commandBufferCount = 1;
    const VkResult result = vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &handle);
    MAGMA_HANDLE_RESULT(result, VK_COMMAND_BUFFER_LEVEL_PRIMARY == level
        ? "failed to allocate primary command buffer"
        : "failed to allocate secondary command buffer");
}

LeanCommandBuffer::~LeanCommandBuffer()
{   // Release only if hasn't been freed through command pool
    if (handle)
        vkFreeCommandBuffers(device, cmdPool, 1, &handle);
}

VkResult LeanCommandBuffer::begin(VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = nullptr;
    cmdBufferBeginInfo.flags = flags;
    cmdBufferBeginInfo.pInheritanceInfo = nullptr;
    return vkBeginCommandBuffer(handle, &cmdBufferBeginInfo);
}

VkResult LeanCommandBuffer::beginInherited(const RenderPass *renderPass, uint32_t subpass, const Framebuffer *framebuffer,
    bool occlusionQueryEnable /* false */, VkQueryControlFlags queryFlags /* 0 */, VkQueryPipelineStatisticFlags pipelineStatistics /* 0 */,
    VkCommandBufferUsageFlags flags /* 0 */, const StructureChain& extendedInfo /* default */) noexcept
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    VkCommandBufferInheritanceInfo cmdBufferInheritanceInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = nullptr;
    cmdBufferBeginInfo.flags = flags | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBufferBeginInfo.pInheritanceInfo = &cmdBufferInheritanceInfo;
    cmdBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufferInheritanceInfo.pNext = extendedInfo.headNode();
    cmdBufferInheritanceInfo.renderPass = *renderPass;
    cmdBufferInheritanceInfo.subpass = subpass;
    // NOTE: Specifying the exact framebuffer that the secondary command buffer will be
    // executed with may result in better performance at command buffer execution time.
    cmdBufferInheritanceInfo.framebuffer = core::dereference(framebuffer);
    cmdBufferInheritanceInfo.occlusionQueryEnable = MAGMA_BOOLEAN(occlusionQueryEnable);
    cmdBufferInheritanceInfo.queryFlags = queryFlags;
    cmdBufferInheritanceInfo.pipelineStatistics = pipelineStatistics;
    return vkBeginCommandBuffer(handle, &cmdBufferBeginInfo);
}

void LeanCommandBuffer::beginRenderPass(const RenderPass *renderPass, Framebuffer *framebuffer, uint32_t clearValueCount, const VkClearValue *clearValues,
    const VkRect2D& renderArea, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea = renderArea;
    renderPassBeginInfo.clearValueCount = clearValueCount;
    renderPassBeginInfo.pClearValues = clearValues;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
}

#ifdef VK_KHR_device_group
VkResult LeanCommandBuffer::beginDeviceGroup(uint32_t deviceMask, VkCommandBufferUsageFlags flags /* 0 */) noexcept
{
    VkCommandBufferBeginInfo cmdBufferBeginInfo;
    VkDeviceGroupCommandBufferBeginInfo cmdBufferBeginDeviceGroupInfo;
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.pNext = &cmdBufferBeginDeviceGroupInfo;
    cmdBufferBeginInfo.flags = flags;
    cmdBufferBeginInfo.pInheritanceInfo = nullptr;
    cmdBufferBeginDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginDeviceGroupInfo.pNext = nullptr;
    cmdBufferBeginDeviceGroupInfo.deviceMask = deviceMask;
    return vkBeginCommandBuffer(handle, &cmdBufferBeginInfo);
}

void LeanCommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask, const RenderPass *renderPass, Framebuffer *framebuffer, uint32_t clearValueCount, const VkClearValue *clearValues,
    uint32_t deviceRenderAreaCount /* 0 */, const VkRect2D *deviceRenderAreas /* nullptr */, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkDeviceGroupRenderPassBeginInfo renderPassBeginDeviceGroupInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = &renderPassBeginDeviceGroupInfo;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea.offset = VkOffset2D{0, 0};
    renderPassBeginInfo.renderArea.extent = (0 == deviceRenderAreaCount) ? framebuffer->getExtent() : VkExtent2D{0, 0};
    renderPassBeginInfo.clearValueCount = clearValueCount;
    renderPassBeginInfo.pClearValues = clearValues;
    renderPassBeginDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    renderPassBeginDeviceGroupInfo.pNext = nullptr;
    renderPassBeginDeviceGroupInfo.deviceMask = deviceMask;
    renderPassBeginDeviceGroupInfo.deviceRenderAreaCount = deviceRenderAreaCount;
    renderPassBeginDeviceGroupInfo.pDeviceRenderAreas = deviceRenderAreas;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
}
#endif // VK_KHR_device_group

// inline void LeanCommandBuffer::setDeviceMask(uint32_t deviceMask) noexcept;
// inline void LeanCommandBuffer::dispatchBase
// inline void LeanCommandBuffer::setLineStipple
// inline void LeanCommandBuffer::pushDescriptorSet
// inline void LeanCommandBuffer::drawMulti
// inline void LeanCommandBuffer::drawMultiInstanced
// inline void LeanCommandBuffer::drawMultiIndexed
// inline void LeanCommandBuffer::drawMultiIndexedInstanced
// inline void LeanCommandBuffer::drawIndirectCount
// inline void LeanCommandBuffer::drawIndexedIndirectCount

#ifdef VK_EXT_conditional_rendering
void LeanCommandBuffer::beginConditionalRendering(const Buffer *buffer, VkDeviceSize offset /* 0 */, bool inverted /* false */) noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT);
    VkConditionalRenderingBeginInfoEXT conditionalRenderingBeginInfo;
    conditionalRenderingBeginInfo.sType = VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT;
    conditionalRenderingBeginInfo.pNext = nullptr;
    conditionalRenderingBeginInfo.buffer = *buffer;
    conditionalRenderingBeginInfo.offset = offset;
    conditionalRenderingBeginInfo.flags = inverted ? VK_CONDITIONAL_RENDERING_INVERTED_BIT_EXT : 0;
    MAGMA_DEVICE_EXTENSION(vkCmdBeginConditionalRenderingEXT);
    vkCmdBeginConditionalRenderingEXT(handle, &conditionalRenderingBeginInfo);
}
#endif // VK_EXT_conditional_rendering

// inline void LeanCommandBuffer::endConditionalRendering
// inline void LeanCommandBuffer::bindTransformFeedbackBuffer
// inline void LeanCommandBuffer::bindTransformFeedbackBuffers

#ifdef VK_EXT_transform_feedback
void LeanCommandBuffer::beginTransformFeedback(uint32_t firstCounterBuffer, uint32_t counterBufferCount, const Buffer **counterBuffers,
    const VkDeviceSize *counterBufferOffsets /* nullptr */) noexcept
{
    auto dereferencedCounterBuffers = stackalloc(VkBuffer, counterBufferCount);
    for (uint32_t i = 0; i < counterBufferCount; ++i)
    {
        MAGMA_ASSERT(counterBuffers[i]->getUsage() & VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT);
        dereferencedCounterBuffers[i] = *counterBuffers[i];
    }
    MAGMA_DEVICE_EXTENSION(vkCmdBeginTransformFeedbackEXT);
    vkCmdBeginTransformFeedbackEXT(handle, firstCounterBuffer, counterBufferCount, dereferencedCounterBuffers, counterBufferOffsets);
}

void LeanCommandBuffer::endTransformFeedback(uint32_t firstCounterBuffer, uint32_t counterBufferCount, Buffer **counterBuffers,
    const VkDeviceSize *counterBufferOffsets /* nullptr */) noexcept
{
    auto dereferencedCounterBuffers = stackalloc(VkBuffer, counterBufferCount);
    for (uint32_t i = 0; i < counterBufferCount; ++i)
    {
        MAGMA_ASSERT(counterBuffers[i]->getUsage() & VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT);
        dereferencedCounterBuffers[i] = *counterBuffers[i];
    }
    MAGMA_DEVICE_EXTENSION(vkCmdEndTransformFeedbackEXT);
    vkCmdEndTransformFeedbackEXT(handle, firstCounterBuffer, counterBufferCount, dereferencedCounterBuffers, counterBufferOffsets);
}
#endif // VK_EXT_transform_feedback

// inline void LeanCommandBuffer::beginQueryIndexed
// inline void LeanCommandBuffer::endQueryIndexed
// inline void LeanCommandBuffer::drawIndirectByteCount

#ifdef VK_KHR_imageless_framebuffer
void LeanCommandBuffer::beginRenderPass(const RenderPass *renderPass, ImagelessFramebuffer *framebuffer,
    uint32_t attachmentCount, ImageView **attachments, uint32_t clearValueCount, const VkClearValue *clearValues,
    const VkRect2D& renderArea, VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    auto dereferencedAttachments = stackalloc(VkImageView, attachmentCount);
    for (uint32_t i = 0; i < attachmentCount; ++i)
        dereferencedAttachments[i] = *attachments[i];
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkRenderPassAttachmentBeginInfoKHR renderPassBeginAttachmentInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = &renderPassBeginAttachmentInfo;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea = renderArea;
    renderPassBeginInfo.clearValueCount = clearValueCount;
    renderPassBeginInfo.pClearValues = clearValues;
    renderPassBeginAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    renderPassBeginAttachmentInfo.pNext = nullptr;
    renderPassBeginAttachmentInfo.attachmentCount = attachmentCount;
    renderPassBeginAttachmentInfo.pAttachments = dereferencedAttachments;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
}

#ifdef VK_KHR_device_group
void LeanCommandBuffer::beginDeviceGroupRenderPass(uint32_t deviceMask, const RenderPass *renderPass, ImagelessFramebuffer *framebuffer,
    uint32_t attachmentCount, ImageView **attachments, uint32_t clearValueCount, const VkClearValue *clearValues,
    uint32_t deviceRenderAreaCount /* 0 */, const VkRect2D *deviceRenderAreas /* nullptr */,
    VkSubpassContents contents /* VK_SUBPASS_CONTENTS_INLINE */) noexcept
{
    auto dereferencedAttachments = stackalloc(VkImageView, attachmentCount);
    for (uint32_t i = 0; i < attachmentCount; ++i)
        dereferencedAttachments[i] = *attachments[i];
    VkRenderPassBeginInfo renderPassBeginInfo;
    VkRenderPassAttachmentBeginInfoKHR renderPassBeginAttachmentInfo;
    VkDeviceGroupRenderPassBeginInfo renderPassBeginDeviceGroupInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = &renderPassBeginAttachmentInfo;
    renderPassBeginInfo.renderPass = *renderPass;
    renderPassBeginInfo.framebuffer = *framebuffer;
    renderPassBeginInfo.renderArea.offset = VkOffset2D{0, 0};
    renderPassBeginInfo.renderArea.extent = (0 == deviceRenderAreaCount) ? framebuffer->getExtent() : VkExtent2D{0, 0};
    renderPassBeginInfo.clearValueCount = clearValueCount;
    renderPassBeginInfo.pClearValues = clearValues;
    renderPassBeginAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO_KHR;
    renderPassBeginAttachmentInfo.pNext = &renderPassBeginDeviceGroupInfo;
    renderPassBeginAttachmentInfo.attachmentCount = attachmentCount;
    renderPassBeginAttachmentInfo.pAttachments = dereferencedAttachments;
    renderPassBeginDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO;
    renderPassBeginDeviceGroupInfo.pNext = nullptr;
    renderPassBeginDeviceGroupInfo.deviceMask = deviceMask;
    renderPassBeginDeviceGroupInfo.deviceRenderAreaCount = deviceRenderAreaCount;
    renderPassBeginDeviceGroupInfo.pDeviceRenderAreas = deviceRenderAreas;
    vkCmdBeginRenderPass(handle, &renderPassBeginInfo, contents);
}
#endif // VK_KHR_device_group
#endif // VK_KHR_imageless_framebuffer

// inline void LeanCommandBuffer::buildAccelerationStructure
// inline void LeanCommandBuffer::updateAccelerationStructure
// inline void LeanCommandBuffer::buildAccelerationStructure
// inline void LeanCommandBuffer::updateAccelerationStructure

#ifdef VK_KHR_acceleration_structure
void LeanCommandBuffer::updateAccelerationStructureIndirect(AccelerationStructure *accelerationStructure, uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries,
    const uint32_t *maxPrimitiveCounts, const Buffer *indirectBuildRanges, Buffer *scratchBuffer) const noexcept
{
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = accelerationStructure->getType();
    buildGeometryInfo.flags = accelerationStructure->getBuildFlags();
    buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
    buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    buildGeometryInfo.dstAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.geometryCount = geometryCount;
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometries;
    buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
    VkDeviceAddress indirectDeviceAddress = indirectBuildRanges->getDeviceAddress();
    uint32_t indirectStride = sizeof(VkAccelerationStructureBuildRangeInfoKHR);
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructuresIndirectKHR);
    vkCmdBuildAccelerationStructuresIndirectKHR(handle, 1, &buildGeometryInfo, &indirectDeviceAddress, &indirectStride, &maxPrimitiveCounts);
}

void LeanCommandBuffer::copyAccelerationStructure(const AccelerationStructure *srcAccelerationStructure, AccelerationStructure *dstAccelerationStructure) const noexcept
{
    VkCopyAccelerationStructureInfoKHR copyInfo;
    copyInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
    copyInfo.pNext = nullptr;
    copyInfo.src = *srcAccelerationStructure;
    copyInfo.dst = *dstAccelerationStructure;
    copyInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureKHR);
    vkCmdCopyAccelerationStructureKHR(handle, &copyInfo);
}

void LeanCommandBuffer::compactAccelerationStructure(const AccelerationStructure *srcAccelerationStructure, AccelerationStructure *dstAccelerationStructure) const noexcept
{
    VkCopyAccelerationStructureInfoKHR copyInfo;
    copyInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
    copyInfo.pNext = nullptr;
    copyInfo.src = *srcAccelerationStructure;
    copyInfo.dst = *dstAccelerationStructure;
    copyInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureKHR);
    vkCmdCopyAccelerationStructureKHR(handle, &copyInfo);
}

void LeanCommandBuffer::copyAccelerationStructureToBuffer(const AccelerationStructure *srcAccelerationStructure, Buffer *dstBuffer) const noexcept
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = *srcAccelerationStructure;
    copyMemoryInfo.dst.deviceAddress = dstBuffer->getDeviceAddress();
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureToMemoryKHR);
    vkCmdCopyAccelerationStructureToMemoryKHR(handle, &copyMemoryInfo);
}

void LeanCommandBuffer::copyBufferToAccelerationStructure(const Buffer *srcBuffer, AccelerationStructure *dstAccelerationStructure) const noexcept
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.deviceAddress = srcBuffer->getDeviceAddress();
    copyMemoryInfo.dst = *dstAccelerationStructure;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyMemoryToAccelerationStructureKHR);
    vkCmdCopyMemoryToAccelerationStructureKHR(handle, &copyMemoryInfo);
}

void LeanCommandBuffer::serializeAccelerationStructure(const AccelerationStructure *srcAccelerationStructure, Buffer *dstBuffer,
    VkDeviceAddress bufferOffset /* 0 */) const noexcept
{
    VkCopyAccelerationStructureToMemoryInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src = *srcAccelerationStructure;
    copyMemoryInfo.dst.deviceAddress = dstBuffer->getDeviceAddress() + bufferOffset;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_SERIALIZE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyAccelerationStructureToMemoryKHR);
    vkCmdCopyAccelerationStructureToMemoryKHR(handle, &copyMemoryInfo);
}

void LeanCommandBuffer::deserializeAccelerationStructure(const Buffer *srcBuffer, AccelerationStructure *dstAccelerationStructure,
    VkDeviceAddress bufferOffset /* 0 */) const noexcept
{
    VkCopyMemoryToAccelerationStructureInfoKHR copyMemoryInfo;
    copyMemoryInfo.sType = VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR;
    copyMemoryInfo.pNext = nullptr;
    copyMemoryInfo.src.deviceAddress = srcBuffer->getDeviceAddress() + bufferOffset;
    copyMemoryInfo.dst = *dstAccelerationStructure;
    copyMemoryInfo.mode = VK_COPY_ACCELERATION_STRUCTURE_MODE_DESERIALIZE_KHR;
    MAGMA_DEVICE_EXTENSION(vkCmdCopyMemoryToAccelerationStructureKHR);
    vkCmdCopyMemoryToAccelerationStructureKHR(handle, &copyMemoryInfo);
}

void LeanCommandBuffer::writeAccelerationStructureProperties(const AccelerationStructure *accelerationStructure, QueryPool *queryPool, uint32_t firstQuery /* 0 */) const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkCmdWriteAccelerationStructuresPropertiesKHR);
    vkCmdWriteAccelerationStructuresPropertiesKHR(handle, 1, accelerationStructure->getHandleAddress(), queryPool->getType(), *queryPool, firstQuery);
}
#endif // VK_KHR_acceleration_structure

// inline void LeanCommandBuffer::setRayTracingPipelineStackSize

#ifdef VK_KHR_ray_tracing_pipeline
void LeanCommandBuffer::traceRays(const ShaderBindingTable& shaderBindingTable, uint32_t width, uint32_t height, uint32_t depth /* 1 */) const noexcept
{
    VkStridedDeviceAddressRegionKHR raygenShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_RAYGEN_BIT_KHR);
    VkStridedDeviceAddressRegionKHR missShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_MISS_BIT_KHR);
    VkStridedDeviceAddressRegionKHR hitShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR);
    VkStridedDeviceAddressRegionKHR callableShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_CALLABLE_BIT_KHR);
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysKHR);
    vkCmdTraceRaysKHR(handle,
        &raygenShaderBindingTable,
        &missShaderBindingTable,
        &hitShaderBindingTable,
        &callableShaderBindingTable,
        width, height, depth);
}

void LeanCommandBuffer::traceRaysIndirect(const ShaderBindingTable& shaderBindingTable, const Buffer *indirectBuffer) const noexcept
{
    MAGMA_ASSERT(indirectBuffer->getUsage() & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);
    VkStridedDeviceAddressRegionKHR raygenShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_RAYGEN_BIT_KHR);
    VkStridedDeviceAddressRegionKHR missShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_MISS_BIT_KHR);
    VkStridedDeviceAddressRegionKHR hitShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR);
    VkStridedDeviceAddressRegionKHR callableShaderBindingTable = shaderBindingTable.getDeviceAddressRegion(VK_SHADER_STAGE_CALLABLE_BIT_KHR);
    MAGMA_DEVICE_EXTENSION(vkCmdTraceRaysIndirectKHR);
    vkCmdTraceRaysIndirectKHR(handle,
        &raygenShaderBindingTable,
        &missShaderBindingTable,
        &hitShaderBindingTable,
        &callableShaderBindingTable,
        indirectBuffer->getDeviceAddress());
}
#endif // VK_KHR_ray_tracing_pipeline

// inline void LeanCommandBuffer::traceRaysIndirect

#ifdef VK_KHR_acceleration_structure
void LeanCommandBuffer::rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode, BottomLevelAccelerationStructure *accelerationStructure,
    uint32_t geometryCount, const VkAccelerationStructureGeometryKHR **geometries, const VkAccelerationStructureBuildRangeInfoKHR *buildRanges,
    Buffer *scratchBuffer) const noexcept
{
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = accelerationStructure->getType();
    buildGeometryInfo.flags = accelerationStructure->getBuildFlags();
    buildGeometryInfo.mode = mode;
    if (VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR == buildGeometryInfo.mode)
        buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    else // VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR
        buildGeometryInfo.srcAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.dstAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.geometryCount = geometryCount;
    buildGeometryInfo.pGeometries = nullptr;
    buildGeometryInfo.ppGeometries = geometries;
    buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos = buildRanges;
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructuresKHR);
    vkCmdBuildAccelerationStructuresKHR(handle, 1, &buildGeometryInfo, &buildRangeInfos);
}

void LeanCommandBuffer::rebuildAccelerationStructure(VkBuildAccelerationStructureModeKHR mode, TopLevelAccelerationStructure *accelerationStructure,
    const AccelerationStructureInstances& instances, Buffer *scratchBuffer) const noexcept
{
    VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo;
    buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildGeometryInfo.pNext = nullptr;
    buildGeometryInfo.type = accelerationStructure->getType();
    buildGeometryInfo.flags = accelerationStructure->getBuildFlags();
    buildGeometryInfo.mode = mode;
    if (VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR == buildGeometryInfo.mode)
        buildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;
    else // VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR
        buildGeometryInfo.srcAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.dstAccelerationStructure = *accelerationStructure;
    buildGeometryInfo.geometryCount = 1; // If type is VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, geometryCount must be 1
    buildGeometryInfo.pGeometries = &instances;
    buildGeometryInfo.ppGeometries = nullptr;
    buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
    const VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo = {instances.primitiveCount};
    const VkAccelerationStructureBuildRangeInfoKHR *buildRangeInfos = &buildRangeInfo;
    MAGMA_DEVICE_EXTENSION(vkCmdBuildAccelerationStructuresKHR);
    vkCmdBuildAccelerationStructuresKHR(handle, 1, &buildGeometryInfo, &buildRangeInfos);
}
#endif // VK_KHR_acceleration_structure

#ifdef VK_NV_cluster_acceleration_structure
void LeanCommandBuffer::buildClusterAccelerationStructureIndirect(VkClusterAccelerationStructureOpTypeNV opType,
    ClusterAccelerationStructure *accelerationStructure, Buffer *scratchBuffer) const noexcept
{
    const uint32_t maxAccelerationStructureCount = accelerationStructure->getMaxAccelerationStructureCount();
    VkClusterAccelerationStructureCommandsInfoNV clusterAccelerationStructureCommandsInfo;
    VkClusterAccelerationStructureMoveObjectsInputNV moveObjects;
    clusterAccelerationStructureCommandsInfo.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_COMMANDS_INFO_NV;
    clusterAccelerationStructureCommandsInfo.pNext = nullptr;
    clusterAccelerationStructureCommandsInfo.input.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV;
    clusterAccelerationStructureCommandsInfo.input.pNext = nullptr;
    clusterAccelerationStructureCommandsInfo.input.maxAccelerationStructureCount = maxAccelerationStructureCount;
    clusterAccelerationStructureCommandsInfo.input.flags = accelerationStructure->getBuildFlags();
    clusterAccelerationStructureCommandsInfo.input.opType = opType;
    clusterAccelerationStructureCommandsInfo.input.opMode = accelerationStructure->getOpMode();
    if (VK_CLUSTER_ACCELERATION_STRUCTURE_OP_TYPE_MOVE_OBJECTS_NV == opType)
    {
        moveObjects.sType = VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_MOVE_OBJECTS_INPUT_NV;
        moveObjects.pNext = nullptr;
        moveObjects.type = accelerationStructure->getType();
        moveObjects.noMoveOverlap = VK_FALSE; // old clusters can overlap themselves
        moveObjects.maxMovedBytes = 0; // TODO
        clusterAccelerationStructureCommandsInfo.input.opInput.pMoveObjects = &moveObjects;
    }
    else
    {
        clusterAccelerationStructureCommandsInfo.input.opInput = accelerationStructure->getOpInput();
    }
    if (VK_CLUSTER_ACCELERATION_STRUCTURE_OP_MODE_IMPLICIT_DESTINATIONS_NV == clusterAccelerationStructureCommandsInfo.input.opMode)
        clusterAccelerationStructureCommandsInfo.dstImplicitData = accelerationStructure->getImplicitData()->getDeviceAddress();
    else // OP_MODE_EXPLICIT_DESTINATIONS_NV or OP_MODE_COMPUTE_SIZES_NV
        clusterAccelerationStructureCommandsInfo.dstImplicitData = MAGMA_NULL;
    clusterAccelerationStructureCommandsInfo.scratchData = scratchBuffer->getDeviceAddress();
    if (VK_CLUSTER_ACCELERATION_STRUCTURE_OP_MODE_COMPUTE_SIZES_NV == clusterAccelerationStructureCommandsInfo.input.opMode)
        clusterAccelerationStructureCommandsInfo.dstAddressesArray = VkStridedDeviceAddressRegionKHR{};
    else
    {   // VK_CLUSTER_ACCELERATION_STRUCTURE_OP_MODE_IMPLICIT_DESTINATIONS_NV - the addresses are saved
        // VK_CLUSTER_ACCELERATION_STRUCTURE_OP_MODE_EXPLICIT_DESTINATIONS_NV - the addresses are read from
        clusterAccelerationStructureCommandsInfo.dstAddressesArray.deviceAddress = accelerationStructure->getAddresses()->getDeviceAddress();
        clusterAccelerationStructureCommandsInfo.dstAddressesArray.stride = sizeof(VkDeviceAddress);
        clusterAccelerationStructureCommandsInfo.dstAddressesArray.size = maxAccelerationStructureCount * sizeof(VkDeviceAddress);
    }
    clusterAccelerationStructureCommandsInfo.dstSizesArray.deviceAddress = accelerationStructure->getSizes()->getDeviceAddress();
    clusterAccelerationStructureCommandsInfo.dstSizesArray.stride = sizeof(uint32_t);
    clusterAccelerationStructureCommandsInfo.dstSizesArray.size = maxAccelerationStructureCount * sizeof(uint32_t);
    clusterAccelerationStructureCommandsInfo.srcInfosArray.deviceAddress = accelerationStructure->getSrcInfosArray()->getDeviceAddress();
    clusterAccelerationStructureCommandsInfo.srcInfosArray.stride = getClusterAccelerationStructureSize(opType);
    clusterAccelerationStructureCommandsInfo.srcInfosArray.size = maxAccelerationStructureCount * clusterAccelerationStructureCommandsInfo.srcInfosArray.stride;
    MAGMA_ASSERT(accelerationStructure->getSrcInfosArray()->getSize() >= clusterAccelerationStructureCommandsInfo.srcInfosArray.size);
    clusterAccelerationStructureCommandsInfo.srcInfosCount = MAGMA_NULL;
    clusterAccelerationStructureCommandsInfo.addressResolutionFlags = 0;
    MAGMA_DEVICE_EXTENSION(vkCmdBuildClusterAccelerationStructureIndirectNV);
    vkCmdBuildClusterAccelerationStructureIndirectNV(handle, &clusterAccelerationStructureCommandsInfo);
}
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
