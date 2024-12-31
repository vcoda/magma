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
#include "pipelineExecutable.h"
#include "../exceptions/errorResult.h"
#include "../misc/extension.h"

namespace magma
{
#ifdef VK_KHR_pipeline_executable_properties
PipelineExecutable::PipelineExecutable(VkDevice device, VkPipeline pipeline,
    const VkPipelineExecutablePropertiesKHR& properties, uint32_t executableIndex) noexcept:
    device(device),
    pipeline(pipeline),
    properties(properties),
    executableIndex(executableIndex)
{}

std::vector<VkPipelineExecutableStatisticKHR> PipelineExecutable::getStatistics() const
{
    VkPipelineExecutableInfoKHR pipelineExecutableInfo;
    pipelineExecutableInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR;
    pipelineExecutableInfo.pNext = nullptr;
    pipelineExecutableInfo.pipeline = pipeline;
    pipelineExecutableInfo.executableIndex = executableIndex;
    uint32_t statisticCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetPipelineExecutableStatisticsKHR, VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME);
    VkResult result = vkGetPipelineExecutableStatisticsKHR(device, &pipelineExecutableInfo, &statisticCount, nullptr);
    std::vector<VkPipelineExecutableStatisticKHR> pipelineExecutableStatistics;
    if (statisticCount > 0)
    {
        VkPipelineExecutableStatisticKHR pipelineExecutableStatistic = {};
        pipelineExecutableStatistic.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_STATISTIC_KHR;
        pipelineExecutableStatistics.resize(statisticCount, pipelineExecutableStatistic);
        result = vkGetPipelineExecutableStatisticsKHR(device, &pipelineExecutableInfo, &statisticCount, pipelineExecutableStatistics.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get statistics of pipeline executable");
    return pipelineExecutableStatistics;
}

std::vector<VkPipelineExecutableInternalRepresentationKHR> PipelineExecutable::getInternalRepresentations() const
{
    VkPipelineExecutableInfoKHR pipelineExecutableInfo;
    pipelineExecutableInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR;
    pipelineExecutableInfo.pNext = nullptr;
    pipelineExecutableInfo.pipeline = pipeline;
    pipelineExecutableInfo.executableIndex = executableIndex;
    uint32_t internalRepresentationCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetPipelineExecutableInternalRepresentationsKHR, VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME);
    VkResult result = vkGetPipelineExecutableInternalRepresentationsKHR(device, &pipelineExecutableInfo, &internalRepresentationCount, nullptr);
    std::vector<VkPipelineExecutableInternalRepresentationKHR> internalRepresentations;
    if (internalRepresentationCount > 0)
    {
        VkPipelineExecutableInternalRepresentationKHR pipelineExecutableInternalRepresentation = {};
        pipelineExecutableInternalRepresentation.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INTERNAL_REPRESENTATION_KHR;
        internalRepresentations.resize(internalRepresentationCount, pipelineExecutableInternalRepresentation);
        result = vkGetPipelineExecutableInternalRepresentationsKHR(device, &pipelineExecutableInfo,
            &internalRepresentationCount, internalRepresentations.data());
        MAGMA_ASSERT(MAGMA_SUCCEEDED(result));
        if (data.empty())
        {   // Allocate memory for internal representation data to be written by Vulkan
            for (auto const& ir: internalRepresentations)
                data.emplace_back(ir.dataSize ? MAGMA_NEW char[ir.dataSize] : nullptr);
        }
        core::foreach(internalRepresentations, data,
            [](auto& ir, auto& data)
            {   // Assign cached pointer
                ir.pData = data.get();
            });
        result = vkGetPipelineExecutableInternalRepresentationsKHR(device, &pipelineExecutableInfo,
            &internalRepresentationCount, internalRepresentations.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get internal representations of pipeline executable");
    return internalRepresentations;
}
#endif // VK_KHR_pipeline_executable_properties
} // namespace magma
