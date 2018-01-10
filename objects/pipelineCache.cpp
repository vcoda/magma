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
#include "pipelineCache.h"
#include "device.h"
#include "../shared.h"

namespace magma
{
PipelineCache::PipelineCache(std::shared_ptr<const Device> device):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT, device)
{
    VkPipelineCacheCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.initialDataSize = 0;
    info.pInitialData = nullptr;
    const VkResult create = vkCreatePipelineCache(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create pipeline cache");
}

PipelineCache::~PipelineCache()
{
    vkDestroyPipelineCache(*device, handle, nullptr);
}
} // namespace magma
