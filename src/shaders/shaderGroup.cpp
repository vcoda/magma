/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
void ShaderGroup::calculateStride(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& properties) noexcept
{   // Minimal stride
    stride = core::alignUp(properties.shaderGroupHandleSize, properties.shaderGroupHandleAlignment);
    for (auto const& [groupIndex, shaderRecord]: shaderRecords)
    {   // Find the largest aligned size of group handle + embedded data
        MAGMA_UNUSED(groupIndex);
        MAGMA_ASSERT(groupIndex < indices.size());
        uint32_t dataSize = core::alignUp(
            properties.shaderGroupHandleSize + (uint32_t)shaderRecord.embedded.size(),
            properties.shaderGroupHandleAlignment);
        stride = std::max(stride, dataSize);
    }
    if (raygen)
    {   // Raygen group must be aligned on shader group base alignment
        stride = core::alignUp(stride, properties.shaderGroupBaseAlignment);
    }
    MAGMA_ASSERT(stride <= properties.maxShaderGroupStride);
}

std::vector<uint8_t> ShaderGroup::getBindingTableData(const std::vector<uint8_t>& shaderGroupHandles,
    const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& properties)
{
    std::vector<uint8_t> tableData(indices.size() * stride);
    uint8_t *data = tableData.data();
    uint32_t recordIndex = 0;
    for (uint32_t groupIndex: indices)
    {   // Copy shader group handle
        const uint32_t offset = groupIndex * properties.shaderGroupHandleSize;
        MAGMA_ASSERT(offset <= shaderGroupHandles.size());
        MAGMA_ASSERT(offset + properties.shaderGroupHandleSize <= shaderGroupHandles.size());
        memcpy(data, shaderGroupHandles.data() + offset, properties.shaderGroupHandleSize);
        // Embedded data is indexed by SBT record index
        auto it = shaderRecords.find(recordIndex);
        if (it != shaderRecords.end())
        {   // Copy embedded data
            const ShaderRecord& shaderRecord = it->second;
            MAGMA_ASSERT(properties.shaderGroupHandleSize + shaderRecord.embedded.size() <= stride);
            memcpy(data + properties.shaderGroupHandleSize,
                shaderRecord.embedded.data(),
                shaderRecord.embedded.size());
        }
        data += stride; // Next group
        ++recordIndex;
    }
    return tableData;
}

VkStridedDeviceAddressRegionKHR ShaderGroup::getRegion(uint32_t indexOffset) const noexcept
{
    if (indices.empty() || !shaderBindingTable)
        return {};
    MAGMA_ASSERT(!raygen || indexOffset < indices.size());
    MAGMA_ASSERT(raygen || (0 == indexOffset));
    VkStridedDeviceAddressRegionKHR region = {};
    region.deviceAddress = shaderBindingTable->getDeviceAddress() + indexOffset * stride;
    region.stride = stride;
    region.size = raygen ? stride : stride * indices.size();
    return region;
}

void ShaderGroup::clear() noexcept
{
    indices.clear();
    shaderBindingTable.reset();
    stride = 0;
    raygen = false;
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
