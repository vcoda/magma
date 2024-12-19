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
#include "pipelineCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
PipelineCache::PipelineCache(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    PipelineCache(std::move(device), 0, nullptr, std::move(allocator), 0, extendedInfo)
{}

PipelineCache::PipelineCache(std::shared_ptr<Device> device,
    std::size_t dataSize, const void *initialData,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkPipelineCacheCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_PIPELINE_CACHE, std::move(device), std::move(allocator))
{
    VkPipelineCacheCreateInfo pipelineCacheInfo;
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = extendedInfo.headNode();
    pipelineCacheInfo.flags = flags;
    pipelineCacheInfo.initialDataSize = dataSize;
    pipelineCacheInfo.pInitialData = initialData;
    const VkResult result = vkCreatePipelineCache(getNativeDevice(), &pipelineCacheInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create pipeline cache");
}

PipelineCache::~PipelineCache()
{
    vkDestroyPipelineCache(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

std::vector<uint8_t> PipelineCache::getData() const
{
    std::size_t dataSize = 0;
    VkResult result = vkGetPipelineCacheData(getNativeDevice(), handle, &dataSize, nullptr);
    std::vector<uint8_t> cacheData;
    if (dataSize)
    {
        cacheData.resize(dataSize);
        result = vkGetPipelineCacheData(getNativeDevice(), handle, &dataSize, cacheData.data());
    }
    MAGMA_HANDLE_RESULT(result, "failed to get data of pipeline cache");
    return cacheData;
}

void PipelineCache::mergeCache(lent_ptr<const PipelineCache> srcCache)
{
    const VkResult result = vkMergePipelineCaches(getNativeDevice(), handle, 1, srcCache->getHandleAddress());
    MAGMA_HANDLE_RESULT(result, "failed to merge pipeline cache");
}

void PipelineCache::mergeCaches(const std::vector<lent_ptr<const PipelineCache>>& srcCaches)
{
    MAGMA_VLA(VkPipelineCache, dereferencedSrcCaches, srcCaches.size());
    for (auto const& cache: srcCaches)
        dereferencedSrcCaches.put(*cache);
    const VkResult result = vkMergePipelineCaches(getNativeDevice(), handle, dereferencedSrcCaches.count(), dereferencedSrcCaches);
    MAGMA_HANDLE_RESULT(result, "failed to merge pipeline caches");
}
} // namespace magma
