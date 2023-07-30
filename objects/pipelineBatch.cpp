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
#include "pipelineBatch.h"
#include "../shaders/pipelineShaderStage.h"

namespace magma
{
void PipelineBatch::postCreate()
{   // Free storage that had to be preserved until vkCreate*Pipelines() call
    stages.clear();
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedbackInfos.clear();
#endif
}

void PipelineBatch::postBuild()
{   // Free storage that had to be preserved until objects are constructed
    layouts.clear();
    basePipelines.clear();
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedbacks.clear();
#endif
    hashes.clear();
}

void PipelineBatch::collectShaderStageInfos() const
{
    std::size_t stageCount = 0;
    for (const auto& shaderStages: stages)
        stageCount += shaderStages.size();
    shaderStageInfos.clear();
    shaderStageInfos.reserve(stageCount);
    for (const auto& shaderStages: stages)
    {   // Copy to array of Vulkan structures due to alignment
        for (const auto& stage: shaderStages)
            shaderStageInfos.push_back(stage);
    }
}
} // namespace magma
