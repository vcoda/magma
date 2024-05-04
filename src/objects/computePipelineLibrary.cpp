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
#include "computePipelineLibrary.h"
#include "pipelineLayout.h"
#include "device.h"
#include "../shaders/pipelineShaderStage.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_pipeline_library
ComputePipelineLibrary::ComputePipelineLibrary(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */) noexcept:
    PipelineLibrary(std::move(device), std::move(allocator))
{}

void ComputePipelineLibrary::compileComputeShader(const PipelineShaderStage& shaderStage,
    std::shared_ptr<PipelineLayout> layout, VkPipelineCreateFlags flags /* 0 */)
{
    VkComputePipelineCreateInfo computePipelineInfo;
    computePipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineInfo.pNext = nullptr;
    computePipelineInfo.flags = flags | VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;
    computePipelineInfo.stage = shaderStage;
    computePipelineInfo.layout = *layout;
    computePipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    computePipelineInfo.basePipelineIndex = -1;
    VkPipeline handle = 0;
    const VkResult result = vkCreateComputePipelines(getNativeDevice(), VK_NULL_HANDLE,
        1, &computePipelineInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to compile compute pipeline");
    libraries.push_back(handle);
}
#endif // VK_KHR_pipeline_library
} // namespace magma
