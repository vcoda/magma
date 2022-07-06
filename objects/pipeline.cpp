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
#include "pipeline.h"
#include "device.h"
#include "pipelineLayout.h"
#include "../shaders/pipelineShaderStage.h"
#include "../allocator/allocator.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Pipeline::Pipeline(VkPipelineBindPoint bindPoint, std::shared_ptr<Device> device, std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline, std::shared_ptr<IAllocator> allocator,
    hash_t hash /* 0 */):
    NonDispatchable<VkPipeline>(VK_OBJECT_TYPE_PIPELINE, std::move(device), std::move(allocator)),
    bindPoint(bindPoint),
    layout(std::move(layout)),
    basePipeline(std::move(basePipeline)),
    hash(hash)
{
    if (!this->layout)
    {   // Layout must be a valid VkPipelineLayout handle
        this->layout = std::make_shared<PipelineLayout>(this->device);
    }
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#ifdef VK_AMD_shader_info
VkShaderStatisticsInfoAMD Pipeline::getShaderStatistics(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t infoSize = sizeof(VkShaderStatisticsInfoAMD);
    VkShaderStatisticsInfoAMD info;
    const VkResult result = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_STATISTICS_AMD, &infoSize, &info);
    MAGMA_THROW_FAILURE(result, "failed to get shader statistics");
    return info;
}

std::vector<uint8_t> Pipeline::getShaderBinary(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t binarySize;
    VkResult result = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_BINARY_AMD, &binarySize, nullptr);
    if (VK_SUCCESS == result)
    {
        std::vector<uint8_t> binary(binarySize);
        result = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_BINARY_AMD, &binarySize, binary.data());
        if (VK_SUCCESS == result)
            return binary;
    }
    return {};
}

std::string Pipeline::getShaderDisassembly(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t disassemblySize;
    VkResult result = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_DISASSEMBLY_AMD, &disassemblySize, nullptr);
    if (VK_SUCCESS == result)
    {
        std::vector<char> disassembly(disassemblySize, '\0'); // May be large enough, so avoid stack allocation
        result = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_DISASSEMBLY_AMD, &disassemblySize, disassembly.data());
        if (VK_SUCCESS == result)
            return std::string(&disassembly[0]);
    }
    return std::string();
}
#endif // VK_AMD_shader_info

void Pipelines::gatherShaderStageInfos() const
{
    std::size_t stageCount = 0;
    for (const auto& shaderStages : stages)
        stageCount += shaderStages.size();
    shaderStageInfos.clear();
    shaderStageInfos.reserve(stageCount);
    for (const auto& shaderStages : stages)
    {   // Copy to array of Vulkan structures due to alignment
        for (const auto& stage : shaderStages)
            shaderStageInfos.push_back(stage);
    }
}
} // namespace magma
