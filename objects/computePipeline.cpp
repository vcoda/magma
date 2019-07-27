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
#include "computePipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../internal/shared.h"

namespace magma
{
ComputePipeline::ComputePipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
    const PipelineShaderStage& stage,
    std::shared_ptr<const PipelineLayout> layout /* nullptr */,
    std::shared_ptr<const ComputePipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Pipeline(std::move(device), std::move(allocator))
{
    VkComputePipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    if (basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    info.stage = stage;
    if (layout)
        info.layout = *layout;
    else
    {
        defaultLayout = std::make_unique<PipelineLayout>(this->device);
        info.layout = *defaultLayout;
    }
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    info.basePipelineIndex = -1;
    const VkResult create = vkCreateComputePipelines(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create compute pipeline");
}
} // namespace magma
