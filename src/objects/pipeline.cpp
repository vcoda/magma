/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "../misc/pipelineExecutable.h"
#include "../misc/extension.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_AMD_pipeline_compiler_control
VkPipelineCompilerControlFlagsAMD Pipeline::compilerControlFlags = 0;
#endif

Pipeline::Pipeline(VkPipelineBindPoint bindPoint, std::shared_ptr<Device> device_, core::variant_ptr<PipelineLayout> layout_,
    std::shared_ptr<Pipeline> basePipeline, std::shared_ptr<IAllocator> allocator, uint32_t stageCount,
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackEXT creationFeedback /* {} */,
    const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks /* {} */,
#endif // VK_EXT_pipeline_creation_feedback
    hash_t hash /* 0 */):
    NonDispatchable<VkPipeline>(VK_OBJECT_TYPE_PIPELINE, std::move(device_), std::move(allocator)),
    bindPoint(bindPoint),
    stageCount(stageCount),
    layout(std::move(layout_)),
    basePipeline(std::move(basePipeline)),
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedback(creationFeedback),
    stageCreationFeedbacks(stageCreationFeedbacks),
#endif // VK_EXT_pipeline_creation_feedback
    hash(hash)
{
    if (!layout)
    {   // Layout must be a valid VkPipelineLayout handle
        layout = std::make_unique<PipelineLayout>(device);
    }
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#ifdef VK_KHR_pipeline_executable_properties
std::vector<std::shared_ptr<PipelineExecutable>> Pipeline::getExecutables() const
{
    VkPipelineInfoKHR pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INFO_KHR;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.pipeline = handle;
    uint32_t executableCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetPipelineExecutablePropertiesKHR, VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME);
    VkResult result = vkGetPipelineExecutablePropertiesKHR(getNativeDevice(), &pipelineInfo, &executableCount, nullptr);
    std::vector<VkPipelineExecutablePropertiesKHR> executableProperties;
    if (executableCount)
    {
        VkPipelineExecutablePropertiesKHR properties = {};
        properties.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_PROPERTIES_KHR;
        executableProperties.resize(executableCount, properties);
        result = vkGetPipelineExecutablePropertiesKHR(getNativeDevice(), &pipelineInfo, &executableCount, executableProperties.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get properties of pipeline executables");
    std::vector<std::shared_ptr<PipelineExecutable>> executables;
    uint32_t index = 0;
    for (auto const& properties: executableProperties)
        executables.emplace_back(PipelineExecutable::makeShared(shared_from_this(), properties, index++));
    return executables;
}
#endif // VK_KHR_pipeline_executable_properties

#ifdef VK_AMD_shader_info
VkShaderStatisticsInfoAMD Pipeline::getShaderStatistics(VkShaderStageFlagBits stage) const
{
    std::size_t infoSize = sizeof(VkShaderStatisticsInfoAMD);
    VkShaderStatisticsInfoAMD shaderStatisticInfo;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    const VkResult result = vkGetShaderInfoAMD(getNativeDevice(), handle, stage, VK_SHADER_INFO_TYPE_STATISTICS_AMD, &infoSize, &shaderStatisticInfo);
    MAGMA_HANDLE_RESULT(result, "failed to get shader statistics");
    return shaderStatisticInfo;
}

std::vector<uint8_t> Pipeline::getShaderBinary(VkShaderStageFlagBits stage) const
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t binarySize;
    VkResult result = vkGetShaderInfoAMD(getNativeDevice(), handle, stage, VK_SHADER_INFO_TYPE_BINARY_AMD, &binarySize, nullptr);
    if (VK_SUCCESS == result)
    {
        std::vector<uint8_t> binary(binarySize);
        result = vkGetShaderInfoAMD(getNativeDevice(), handle, stage, VK_SHADER_INFO_TYPE_BINARY_AMD, &binarySize, binary.data());
        if (VK_SUCCESS == result)
            return binary;
    }
    return {};
}

std::string Pipeline::getShaderDisassembly(VkShaderStageFlagBits stage) const
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t disassemblySize;
    VkResult result = vkGetShaderInfoAMD(getNativeDevice(), handle, stage, VK_SHADER_INFO_TYPE_DISASSEMBLY_AMD, &disassemblySize, nullptr);
    if (VK_SUCCESS == result)
    {   // May be large enough, so allocate in the heap
        std::vector<char> disassembly(disassemblySize, '\0');
        result = vkGetShaderInfoAMD(getNativeDevice(), handle, stage, VK_SHADER_INFO_TYPE_DISASSEMBLY_AMD, &disassemblySize, disassembly.data());
        if (VK_SUCCESS == result)
            return std::string(&disassembly[0]);
    }
    return std::string();
}
#endif // VK_AMD_shader_info
} // namespace magma
