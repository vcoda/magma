/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
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
#include "shaderBindingTable.h"
#include "device.h"
#include "physicalDevice.h"
#include "commandBuffer.h"
#include "../helpers/alignment.h"

namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
ShaderBindingTable::ShaderBindingTable(std::shared_ptr<CommandBuffer> cmdBuffer,
    const void *shaderGroupHandles, size_t shaderGroupSize, const VkShaderStageFlagBits *shaderStageFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Buffer(cmdBuffer->getDevice(),
        calculateAlignedSize(cmdBuffer->getDevice(), shaderGroupSize), 0, // flags
        VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        optional, sharing, allocator)
{
    const auto rayTracingPipelineProperties = device->getPhysicalDevice()->getRayTracingPipelineProperties();
    const uint32_t shaderGroupHandleSize = rayTracingPipelineProperties.shaderGroupHandleSize;
    const uint32_t shaderGroupBaseAlignment = helpers::aligned(shaderGroupHandleSize, rayTracingPipelineProperties.shaderGroupBaseAlignment);
    const uint32_t shaderGroupCount = (uint32_t)shaderGroupSize / shaderGroupHandleSize;
    MAGMA_STACK_ARRAY(uint8_t, shaderGroupHandlesWithBaseAlignment, static_cast<size_t>(getSize()));
    const uint8_t *srcHandle = reinterpret_cast<const uint8_t *>(shaderGroupHandles);
    uint8_t *dstHandle = shaderGroupHandlesWithBaseAlignment;
    for (uint32_t i = 0; i < shaderGroupCount; ++i)
    {   // Copy shader group handle with alignment
        memcpy(dstHandle, srcHandle, shaderGroupHandleSize);
        dstHandle += shaderGroupBaseAlignment;
        srcHandle += shaderGroupHandleSize;
    }
    stagedUpload(std::move(cmdBuffer), shaderGroupHandlesWithBaseAlignment, std::move(allocator));
    const VkDeviceAddress baseAddress = getDeviceAddress();
    for (uint32_t i = 0; i < shaderGroupCount; ++i)
        setStageData(shaderStageFlags[i], i, baseAddress, shaderGroupHandleSize, shaderGroupBaseAlignment);
}

ShaderBindingTable::ShaderBindingTable(std::shared_ptr<CommandBuffer> cmdBuffer,
    const std::vector<uint8_t>& shaderGroupHandles, const std::vector<VkShaderStageFlagBits>& shaderStageFlags,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    ShaderBindingTable(std::move(cmdBuffer), shaderGroupHandles.data(), shaderGroupHandles.size(), shaderStageFlags.data(),
        std::move(allocator), optional, sharing)
{}

VkDeviceSize ShaderBindingTable::calculateAlignedSize(const std::shared_ptr<Device>& device, size_t size) noexcept
{
    const auto rayTracingPipelineProperties = device->getPhysicalDevice()->getRayTracingPipelineProperties();
    MAGMA_ASSERT(size % rayTracingPipelineProperties.shaderGroupHandleSize == 0);
    const uint32_t shaderGroupCount = (uint32_t)size / rayTracingPipelineProperties.shaderGroupHandleSize;
    const uint32_t alignedShaderBindingTableSize = shaderGroupCount * rayTracingPipelineProperties.shaderGroupBaseAlignment;
    return alignedShaderBindingTableSize;
}

void ShaderBindingTable::setStageData(VkShaderStageFlagBits stage, uint32_t stageIndex, VkDeviceAddress baseAddress,
    uint32_t shaderGroupHandleSize, uint32_t shaderGroupBaseAlignment) noexcept
{
    VkStridedDeviceAddressRegionKHR *region;
    switch (stage)
    { // TODO: handle all possible shader stages
    case VK_SHADER_STAGE_RAYGEN_BIT_KHR: region = &raygen; break;
    case VK_SHADER_STAGE_MISS_BIT_KHR: region = &miss; break;
    case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: region = &hit; break;
    case VK_SHADER_STAGE_CALLABLE_BIT_KHR: region = &callable; break;
    default:
        MAGMA_ASSERT(false);
        return;
    }
    region->deviceAddress = baseAddress + shaderGroupBaseAlignment * stageIndex;
    MAGMA_ASSERT(region->deviceAddress % shaderGroupBaseAlignment == 0);
    region->stride = shaderGroupHandleSize;
    region->size = shaderGroupHandleSize;
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
