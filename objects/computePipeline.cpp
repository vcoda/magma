/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"

namespace magma
{
ComputePipeline::ComputePipeline(std::shared_ptr<Device> device, std::shared_ptr<PipelineCache> cache,
    const PipelineShaderStage& stage,
    std::shared_ptr<PipelineLayout> layout /* nullptr */,
    std::shared_ptr<ComputePipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Pipeline(std::move(device), std::move(basePipeline), std::move(cache), std::move(allocator))
{
    VkComputePipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    if (this->basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    info.stage = stage;
    if (layout)
        this->layout = std::move(layout);
    else
        this->layout =  std::make_shared<PipelineLayout>(this->device);
    info.layout = *this->layout;
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(this->basePipeline);
    info.basePipelineIndex = -1;
    const VkResult create = vkCreateComputePipelines(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(this->cache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create compute pipeline");
    hash = internal::hashArgs(
        info.sType,
        info.flags);
    internal::hashCombine(hash, stage.hash());
    if (this->layout)
        internal::hashCombine(hash, this->layout->getHash());
}
} // namespace magma
