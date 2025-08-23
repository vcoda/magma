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
#include "shaderBindingTable.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/commandBuffer.h"
#include "../objects/rayTracingPipeline.h"

namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
class SBTBuffer : public Buffer
{
public:
    SBTBuffer(lent_ptr<CommandBuffer> cmdBuffer, const std::vector<uint8_t>& buffer, std::shared_ptr<Allocator> allocator):
        Buffer(cmdBuffer->getDevice(), buffer.size(), 0, // flags
            VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            Initializer(), Sharing(), allocator)
    {
        stagingCopy(std::move(cmdBuffer), buffer.data(), buffer.size(),
            0, 0, VK_WHOLE_SIZE, std::move(allocator));
    }
};

ShaderBindingTable::ShaderBindingTable(lent_ptr<const RayTracingPipeline> pipeline,
    lent_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<Allocator> allocator /* nullptr */)
{
    build(std::move(pipeline), std::move(cmdBuffer), std::move(allocator));
}

void ShaderBindingTable::build(lent_ptr<const RayTracingPipeline> pipeline,
    lent_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<Allocator> allocator /* nullptr */,
    const std::vector<VkRayTracingPipelineCreateInfoKHR>& librariesInfo /*= {} */)
{
    // Get the total number of groups and handle index position
    uint32_t totalGroupCount = 0;
    std::vector<uint32_t> groupCountPerInput;
    groupCountPerInput.reserve(1 + librariesInfo.size());
    if (true) // TODO
    {
        setupIndices(pipeline->getShaderStages(), pipeline->getShaderGroups());
        groupCountPerInput.push_back((uint32_t)pipeline->getShaderGroups().size());
        totalGroupCount += (uint32_t)pipeline->getShaderGroups().size();
        for(const auto& lib : librariesInfo)
        {
            groupCountPerInput.push_back(lib.groupCount);
            totalGroupCount += lib.groupCount;
        }
    }
    else
    {
        // TODO:
    }
    const std::shared_ptr<Device>& device = cmdBuffer->getDevice();
    const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
    const VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingPipelineProperties = physicalDevice->getRayTracingPipelineProperties();
    const std::vector<uint8_t> shaderGroupHandles = pipeline->getShaderGroupHandles();
    for (auto& [stage, group]: groups)
    {
        MAGMA_UNUSED(stage);
        group.calculateStride(rayTracingPipelineProperties);
        const std::vector<uint8_t> bindingTableData = group.getBindingTableData(shaderGroupHandles, rayTracingPipelineProperties);
        group.shaderBindingTable = std::make_unique<SBTBuffer>(cmdBuffer.get(), bindingTableData, allocator);
    }
}

VkStridedDeviceAddressRegionKHR ShaderBindingTable::getDeviceAddressRegion(VkShaderStageFlagBits stage,
    uint32_t indexOffset /* 0 */) const noexcept
{
    auto it = groups.find(stage);
    if (it != groups.end())
        return it->second.getRegion(indexOffset);
    return {};
}

void ShaderBindingTable::setupIndices(const std::vector<PipelineShaderStage>& shaderStages,
    const std::vector<RayTracingShaderGroup>& shaderGroups)
{
    uint32_t groupOffset = 0;
    // Finding the handle index position of each group type
    // in ray tracing pipeline. If the pipeline was created like:
    // raygen, miss, hit, miss, hit, hit, the result will be:
    // raygen[0], miss[1, 3], hit[2, 4, 5], callable[].
    uint32_t groupIndex = 0;
    for (auto const& group: shaderGroups)
    {
        VkShaderStageFlagBits stage;
        if (VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR == group.type)
        {
            const uint32_t generalShader = group.generalShader;
            MAGMA_ASSERT(generalShader < shaderStages.size());
            stage = shaderStages[generalShader].stage;
        }
        else // *HIT_GROUP_KHR
        {
            stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        }
        auto& shaderGroup = groups[stage];
        shaderGroup.addGroupIndex(groupIndex + groupOffset);
        if (VK_SHADER_STAGE_RAYGEN_BIT_KHR == stage)
            shaderGroup.raygen = true;
        ++groupIndex;
    }
    groupOffset += core::countof(shaderGroups);
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
