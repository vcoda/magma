/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../exceptions/errorResult.h"

namespace magma
{
PipelineCache::PipelineCache(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    PipelineCache(std::move(device), 0, nullptr, std::move(allocator))
{}

PipelineCache::PipelineCache(std::shared_ptr<Device> device,
    std::size_t dataSize, const void *cacheData,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_CACHE, std::move(device), std::move(allocator))
{
    VkPipelineCacheCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.initialDataSize = dataSize;
    info.pInitialData = cacheData;
    const VkResult create = vkCreatePipelineCache(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create pipeline cache");
}

PipelineCache::~PipelineCache()
{
    vkDestroyPipelineCache(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

std::vector<uint8_t> PipelineCache::getData() const
{
    std::size_t dataSize;
    const VkResult getSize = vkGetPipelineCacheData(MAGMA_HANDLE(device), handle, &dataSize, nullptr);
    MAGMA_THROW_FAILURE(getSize, "failed to get pipeline cache size");
    std::vector<uint8_t> data(dataSize);
    const VkResult getData = vkGetPipelineCacheData(MAGMA_HANDLE(device), handle, &dataSize, data.data());
    MAGMA_THROW_FAILURE(getData, "failed to get pipeline cache data");
    return data;
}

void PipelineCache::mergeCaches(const std::vector<std::shared_ptr<const PipelineCache>>& caches)
{
    MAGMA_STACK_ARRAY(VkPipelineCache, dereferencedCaches, caches.size());
    for (const auto& cache : caches)
        dereferencedCaches.put(*cache);
    const VkResult merge = vkMergePipelineCaches(MAGMA_HANDLE(device), handle, MAGMA_COUNT(dereferencedCaches), dereferencedCaches);
    MAGMA_THROW_FAILURE(merge, "failed to merge pipeline caches");
}
} // namespace magma
