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
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../exceptions/errorResult.h"

namespace magma
{
PipelineCache::PipelineCache(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    PipelineCache(std::move(device), 0, nullptr, std::move(allocator), extendedInfo)
{}

PipelineCache::PipelineCache(std::shared_ptr<Device> device,
    std::size_t dataSize, const void *cacheData,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_CACHE, std::move(device), std::move(allocator))
{
    VkPipelineCacheCreateInfo cacheInfo;
    cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cacheInfo.pNext = extendedInfo.chainNodes();
    cacheInfo.flags = 0;
    cacheInfo.initialDataSize = dataSize;
    cacheInfo.pInitialData = cacheData;
    const VkResult result = vkCreatePipelineCache(MAGMA_HANDLE(device), &cacheInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create pipeline cache");
}

PipelineCache::~PipelineCache()
{
    vkDestroyPipelineCache(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

std::vector<uint8_t> PipelineCache::getData() const
{
    std::size_t dataSize = 0;
    VkResult result = vkGetPipelineCacheData(MAGMA_HANDLE(device), handle, &dataSize, nullptr);
    std::vector<uint8_t> cacheData;
    if (dataSize)
    {
        cacheData.resize(dataSize);
        result = vkGetPipelineCacheData(MAGMA_HANDLE(device), handle, &dataSize, cacheData.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get data of pipeline cache");
    return cacheData;
}

void PipelineCache::mergeCache(std::shared_ptr<const PipelineCache> srcCache)
{
    const VkResult result = vkMergePipelineCaches(MAGMA_HANDLE(device), handle, 1, srcCache->getHandleAddress());
    MAGMA_HANDLE_RESULT(result, "failed to merge pipeline cache");
}

void PipelineCache::mergeCaches(const std::vector<std::shared_ptr<const PipelineCache>>& caches)
{
    MAGMA_STACK_ARRAY(VkPipelineCache, dereferencedCaches, caches.size());
    for (const auto& cache : caches)
        dereferencedCaches.put(*cache);
    const VkResult result = vkMergePipelineCaches(MAGMA_HANDLE(device), handle, MAGMA_COUNT(dereferencedCaches), dereferencedCaches);
    MAGMA_HANDLE_RESULT(result, "failed to merge pipeline caches");
}
} // namespace magma
