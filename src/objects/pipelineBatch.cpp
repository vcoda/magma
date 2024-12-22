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
#include "device.h"
#include "pipelineBatch.h"

namespace magma
{
PipelineBatch::PipelineBatch(std::shared_ptr<Device> device) noexcept:
    device(std::move(device))
{}

std::future<void> PipelineBatch::buildPipelinesAsync(lent_ptr<PipelineCache> pipelineCache_ /* nullptr */,
#ifdef VK_KHR_pipeline_library
    lent_ptr<const PipelineLibrary> pipelineLibrary_,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
    PipelineCache *pipelineCache = pipelineCache_.get();
#ifdef VK_KHR_pipeline_library
    const PipelineLibrary *pipelineLibrary = pipelineLibrary_.get();
#endif
    return std::async(std::launch::async,
        [this, pipelineCache,
        #ifdef VK_KHR_pipeline_library
            pipelineLibrary,
        #endif
            allocator]()
        {
            buildPipelines(pipelineCache,
            #ifdef VK_KHR_pipeline_library
                pipelineLibrary,
            #endif
                std::move(allocator));
        });
}

VkDevice PipelineBatch::getNativeDevice() const noexcept
{
    return device->getHandle();
}

void PipelineBatch::collectShaderStageInfos() const
{
    std::size_t stageCount = 0;
    for (auto const& shaderStages: stages)
        stageCount += shaderStages.size();
    shaderStageInfos.clear();
    shaderStageInfos.reserve(stageCount);
    for (auto const& shaderStages: stages)
    {   // Copy to array of Vulkan structures due to alignment
        for (auto const& stage: shaderStages)
            shaderStageInfos.push_back(stage);
    }
}
} // namespace magma
