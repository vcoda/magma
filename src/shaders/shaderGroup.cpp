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
#include "shaderGroup.h"
#include "../objects/buffer.h"

namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
ShaderGroup::ShaderGroup() noexcept:
    stride(0),
    raygen(false)
{}

void ShaderGroup::calculateStride(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR properties) noexcept
{   // Minimal stride
    stride = core::alignUp(properties.shaderGroupHandleSize, properties.shaderGroupHandleAlignment);
    for (auto const& [groupIndex, shaderRecord]: shaderRecords)
    {   // Find the largest aligned size of group handle + embedded data
        MAGMA_UNUSED(groupIndex);
        uint32_t dataSize = core::alignUp(
            properties.shaderGroupHandleSize + (uint32_t)shaderRecord.embedded.size(),
            properties.shaderGroupHandleAlignment);
        stride = std::max(stride, dataSize);
    }
    if (raygen)
    {   // Raygen group must be aligned on shader group base alignment
        stride = core::alignUp(stride, properties.shaderGroupBaseAlignment);
    }
    MAGMA_ASSERT(stride < properties.maxShaderGroupStride);
}

std::vector<uint8_t> ShaderGroup::getBindingTableData(const std::vector<uint8_t>& shaderGroupHandles,
    const VkPhysicalDeviceRayTracingPipelinePropertiesKHR properties)
{
    std::vector<uint8_t> tableData(indices.size() * stride);
    uint8_t *data = tableData.data();
    for (uint32_t groupIndex: indices)
    {   // Copy shader group handle
        const uint32_t offset = groupIndex * properties.shaderGroupHandleSize;
        memcpy(data, &shaderGroupHandles[offset], properties.shaderGroupHandleSize);
        // Do we have embedded parameters?
        auto it = shaderRecords.find(groupIndex);
        if (it != shaderRecords.end())
        {   // Copy embedded data
            const ShaderRecord& shaderRecord = it->second;
            memcpy(data + properties.shaderGroupHandleSize,
                shaderRecord.embedded.data(),
                shaderRecord.embedded.size());
        }
        data += stride; // Next group
    }
    return tableData;
}

VkStridedDeviceAddressRegionKHR ShaderGroup::getRegion(uint32_t indexOffset) const noexcept
{
    VkStridedDeviceAddressRegionKHR region = {};
    if (shaderBindingTable)
    {
        region.deviceAddress = shaderBindingTable->getDeviceAddress() + indexOffset * stride;
        region.stride = stride;
        region.size = raygen ? stride : stride * MAGMA_COUNT(indices);
    }
    return region;
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
