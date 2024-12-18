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
#include "computePipelineBatch.h"
#include "computePipeline.h"
#include "pipelineLayout.h"
#include "pipelineLibrary.h"
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ComputePipelineBatch::ComputePipelineBatch(std::shared_ptr<Device> device) noexcept:
    BasePipelineBatch<ComputePipeline, VkComputePipelineCreateInfo>(std::move(device))
{}

uint32_t ComputePipelineBatch::batchPipeline(const PipelineShaderStage& shaderStage,
    variant_ptr<PipelineLayout> layout, std::shared_ptr<ComputePipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */, const StructureChain& extendedInfo /* default */)
{
    stages.emplace_front(1, shaderStage);
    if (!layout)
        layout = std::make_unique<PipelineLayout>(device);
    basePipelines.push_front(basePipeline);
    VkComputePipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stage = stages.front().front();
    pipelineInfo.layout = *layout;
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipelines.front());
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_AMD_pipeline_compiler_control
    if (device->extensionEnabled(VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME))
    {
        VkPipelineCompilerControlCreateInfoAMD pipelineCompilerControlInfo;
        pipelineCompilerControlInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD;
        pipelineCompilerControlInfo.pNext = nullptr;
        pipelineCompilerControlInfo.compilerControlFlags = compilerControlFlags;
        pipelineCompilerControlInfos.push_front(pipelineCompilerControlInfo);
        linkNode(pipelineInfo, pipelineCompilerControlInfos.front());
    }
#endif // VK_AMD_pipeline_compiler_control
#ifdef VK_EXT_pipeline_creation_feedback
    if (device->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        creationFeedbacks.push_front(VkPipelineCreationFeedbackEXT());
        stageCreationFeedbacks.emplace_front(1);
        VkPipelineCreationFeedbackCreateInfoEXT creationFeedbackInfo;
        creationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        creationFeedbackInfo.pNext = nullptr;
        creationFeedbackInfo.pPipelineCreationFeedback = &creationFeedbacks.front();
        creationFeedbackInfo.pipelineStageCreationFeedbackCount = 1;
        creationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.front().data();
        creationFeedbackInfos.push_front(creationFeedbackInfo);
        linkNode(pipelineInfo, creationFeedbackInfos.front());
    }
#endif // VK_EXT_pipeline_creation_feedback
    pipelineInfos.push_back(pipelineInfo);
    hash_t hash = core::hash(flags);
    hash = core::hashCombine(hash, shaderStage.getHash());
    hash = core::hashCombine(hash, layout->getHash());
    hashes.push_front(hash);
    layouts.push_front(std::move(layout));
    return core::countof(pipelineInfos) - 1;
}

void ComputePipelineBatch::buildPipelines(lent_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    lent_ptr<const PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */)
{
#ifdef VK_KHR_pipeline_library
    if (device->extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
        linkPipelineLibrary(std::move(pipelineLibrary));
#endif // VK_KHR_pipeline_library
    std::vector<VkPipeline> handles(pipelineInfos.size(), VK_NULL_HANDLE);
    const VkResult result = vkCreateComputePipelines(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        core::countof(pipelineInfos), pipelineInfos.data(), allocator.get(), handles.data());
    postCreate();
    if (VK_SUCCESS == result)
    {
        auto handle = handles.cbegin();
        auto layout = layouts.begin();
    #ifdef VK_EXT_pipeline_creation_feedback
        auto creationFeedback = creationFeedbacks.cbegin();
        auto stageFeedbacks = stageCreationFeedbacks.cbegin();
    #endif // VK_EXT_pipeline_creation_feedback
        auto hash = hashes.cbegin();
        while (handle != handles.cend())
        {
            pipelines.emplace_front(ComputePipeline::makeUnique(
                *handle++, device, std::move(*layout++), allocator,
            #ifdef VK_EXT_pipeline_creation_feedback
                creationFeedbacks.empty() ? VkPipelineCreationFeedbackEXT{} : *creationFeedback++,
                stageCreationFeedbacks.empty() ? std::vector<VkPipelineCreationFeedbackEXT>{} : *stageFeedbacks++,
            #endif // VK_EXT_pipeline_creation_feedback
                *hash++));
        }
    }
    postBuild();
    MAGMA_HANDLE_RESULT(result, "failed to create multiple compute pipelines");
}
} // namespace magma
