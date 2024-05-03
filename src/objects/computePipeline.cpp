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
#include "computePipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "pipelineLibrary.h"
#include "device.h"
#include "../shaders/pipelineShaderStage.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
ComputePipeline::ComputePipeline(std::shared_ptr<Device> device_,
    const PipelineShaderStage& shaderStage,
    std::shared_ptr<PipelineLayout> layout_,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<PipelineCache> pipelineCache /* nullptr */,
#ifdef VK_KHR_pipeline_library
    std::shared_ptr<PipelineLibrary> pipelineLibrary /* nullptr */,
#endif
    std::shared_ptr<ComputePipeline> basePipeline_ /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Pipeline(VK_PIPELINE_BIND_POINT_COMPUTE, std::move(device_), std::move(layout_), std::move(basePipeline_), std::move(allocator), 1)
{
    VkComputePipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = extendedInfo.headNode();
    pipelineInfo.flags = flags;
    if (basePipeline)
        pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    pipelineInfo.stage = shaderStage;
    pipelineInfo.layout = *layout;
    pipelineInfo.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    pipelineInfo.basePipelineIndex = -1;
#ifdef VK_AMD_pipeline_compiler_control
    VkPipelineCompilerControlCreateInfoAMD pipelineCompilerControlInfo;
    if (device->extensionEnabled(VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME))
    {
        pipelineCompilerControlInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD;
        pipelineCompilerControlInfo.pNext = nullptr;
        pipelineCompilerControlInfo.compilerControlFlags = compilerControlFlags;
        linkNode(pipelineInfo, pipelineCompilerControlInfo);
    }
#endif // VK_AMD_pipeline_compiler_control
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackCreateInfoEXT pipelineCreationFeedbackInfo;
    if (device->extensionEnabled(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME))
    {
        stageCreationFeedbacks.resize(1);
        pipelineCreationFeedbackInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT;
        pipelineCreationFeedbackInfo.pNext = nullptr;
        pipelineCreationFeedbackInfo.pPipelineCreationFeedback = &creationFeedback;
        pipelineCreationFeedbackInfo.pipelineStageCreationFeedbackCount = 1;
        pipelineCreationFeedbackInfo.pPipelineStageCreationFeedbacks = stageCreationFeedbacks.data();
        linkNode(pipelineInfo, pipelineCreationFeedbackInfo);
    }
#endif // VK_EXT_pipeline_creation_feedback
#ifdef VK_KHR_pipeline_library
    VkPipelineLibraryCreateInfoKHR pipelineLibraryInfo;
    if (pipelineLibrary && device->extensionEnabled(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME))
    {
        pipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
        pipelineLibraryInfo.pNext = nullptr;
        pipelineLibraryInfo.libraryCount = pipelineLibrary->getLibraryCount();
        pipelineLibraryInfo.pLibraries = pipelineLibrary->getLibraries();
        linkNode(pipelineInfo, pipelineLibraryInfo);
    }
#endif // VK_KHR_pipeline_library
    const VkResult result = vkCreateComputePipelines(getNativeDevice(), MAGMA_OPTIONAL_HANDLE(pipelineCache),
        1, &pipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    if (result != VK_SUCCESS)
    {
    #ifdef MAGMA_DEBUG
        std::cerr << shaderStage << std::endl << *layout << std::endl;
    #endif
        MAGMA_HANDLE_RESULT(result, "failed to create compute pipeline");
    }
    hash = core::hash(flags);
    hash = core::hashCombine(hash, shaderStage.getHash());
    hash = core::hashCombine(hash, layout->getHash());
}

ComputePipeline::ComputePipeline(VkPipeline handle_,
    std::shared_ptr<Device> device,
    std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline,
    std::shared_ptr<IAllocator> allocator,
#ifdef VK_EXT_pipeline_creation_feedback
    VkPipelineCreationFeedbackEXT creationFeedback,
    const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks,
#endif
    hash_t hash):
    Pipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, std::move(device), std::move(layout), std::move(basePipeline), std::move(allocator),
        1,
    #ifdef VK_EXT_pipeline_creation_feedback
        creationFeedback, stageCreationFeedbacks,
    #endif
        hash)
{
    handle = handle_;
}
} // namespace magma
