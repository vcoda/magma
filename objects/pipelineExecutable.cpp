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
#include "pipelineExecutable.h"
#include "pipeline.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_pipeline_executable_properties
PipelineExecutable::PipelineExecutable(std::shared_ptr<const Pipeline> pipeline,
    const VkPipelineExecutablePropertiesKHR& properties,
    uint32_t executableIndex) noexcept:
    device(pipeline->getDevice()),
    pipeline(std::move(pipeline)),
    properties(properties),
    executableIndex(executableIndex)
{}

std::vector<VkPipelineExecutableStatisticKHR> PipelineExecutable::getStatistics() const
{
    VkPipelineExecutableInfoKHR pipelineExecutableInfo;
    pipelineExecutableInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR;
    pipelineExecutableInfo.pNext = nullptr;
    pipelineExecutableInfo.pipeline = *pipeline;
    pipelineExecutableInfo.executableIndex = executableIndex;
    uint32_t statisticCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetPipelineExecutableStatisticsKHR, VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME);
    VkResult result = vkGetPipelineExecutableStatisticsKHR(MAGMA_HANDLE(device), &pipelineExecutableInfo, &statisticCount, nullptr);
    std::vector<VkPipelineExecutableStatisticKHR> statistics;
    if (statisticCount > 0)
    {
        VkPipelineExecutableStatisticKHR statistic = {};
        statistic.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_STATISTIC_KHR;
        statistics.resize(statisticCount, statistic);
        result = vkGetPipelineExecutableStatisticsKHR(MAGMA_HANDLE(device), &pipelineExecutableInfo, &statisticCount, statistics.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get statistics of pipeline executable");
    return statistics;
}

std::vector<VkPipelineExecutableInternalRepresentationKHR> PipelineExecutable::getInternalRepresentations() const
{
    VkPipelineExecutableInfoKHR pipelineExecutableInfo;
    pipelineExecutableInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR;
    pipelineExecutableInfo.pNext = nullptr;
    pipelineExecutableInfo.pipeline = *pipeline;
    pipelineExecutableInfo.executableIndex = executableIndex;
    uint32_t internalRepresentationCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetPipelineExecutableInternalRepresentationsKHR, VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME);
    VkResult result = vkGetPipelineExecutableInternalRepresentationsKHR(MAGMA_HANDLE(device), &pipelineExecutableInfo, &internalRepresentationCount, nullptr);
    std::vector<VkPipelineExecutableInternalRepresentationKHR> internalRepresentations;
    if (internalRepresentationCount > 0)
    {
        VkPipelineExecutableInternalRepresentationKHR internalRepresentation = {};
        internalRepresentation.sType = VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INTERNAL_REPRESENTATION_KHR;
        internalRepresentations.resize(internalRepresentationCount, internalRepresentation);
        result = vkGetPipelineExecutableInternalRepresentationsKHR(MAGMA_HANDLE(device), &pipelineExecutableInfo,
            &internalRepresentationCount, internalRepresentations.data());
        MAGMA_ASSERT(SUCCEEDED(result));
        for (auto& ir : internalRepresentations)
        {
            if (ir.dataSize > 0)
            {   // Allocate memory for internal representation data to be written by Vulkan
                data.emplace_back(new char[ir.dataSize]);
                ir.pData = data.back().get();
            }
        }
        result = vkGetPipelineExecutableInternalRepresentationsKHR(MAGMA_HANDLE(device), &pipelineExecutableInfo,
            &internalRepresentationCount, internalRepresentations.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get internal representations of pipeline executable");
    return internalRepresentations;
}
#endif // VK_KHR_pipeline_executable_properties
} // namespace magma
