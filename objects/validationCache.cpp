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
#include "validationCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"

namespace magma
{
#ifdef VK_EXT_validation_cache
ValidationCache::ValidationCache(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    ValidationCache(std::move(device), 0, nullptr, std::move(allocator), extendedInfo)
{}

ValidationCache::ValidationCache(std::shared_ptr<Device> device,
    std::size_t dataSize, const void *cacheData,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_VALIDATION_CACHE_EXT, std::move(device), std::move(allocator))
{
    VkValidationCacheCreateInfoEXT validationCacheInfo;
    validationCacheInfo.sType = VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT;
    validationCacheInfo.pNext = extendedInfo.chainNodes();
    validationCacheInfo.flags = 0;
    validationCacheInfo.initialDataSize = dataSize;
    validationCacheInfo.pInitialData = cacheData;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateValidationCacheEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    const VkResult result = vkCreateValidationCacheEXT(MAGMA_HANDLE(device), &validationCacheInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create validation cache");
}

ValidationCache::~ValidationCache()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyValidationCacheEXT);
    vkDestroyValidationCacheEXT(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

std::vector<uint8_t> ValidationCache::getData() const
{
    std::vector<uint8_t> cacheData;
    std::size_t dataSize = 0;
    MAGMA_DEVICE_EXTENSION(vkGetValidationCacheDataEXT);
    VkResult result = vkGetValidationCacheDataEXT(MAGMA_HANDLE(device), handle, &dataSize, nullptr);
    if (dataSize > 0)
    {
        cacheData.resize(dataSize);
        result = vkGetValidationCacheDataEXT(MAGMA_HANDLE(device), handle, &dataSize, cacheData.data());
    }
    MAGMA_THROW_FAILURE(result, "failed to get validation cache data");
    return cacheData;
}

void ValidationCache::mergeCaches(const std::vector<std::shared_ptr<const ValidationCache>>& caches)
{
    MAGMA_STACK_ARRAY(VkValidationCacheEXT, dereferencedCaches, caches.size());
    for (const auto& cache : caches)
        dereferencedCaches.put(*cache);
    MAGMA_DEVICE_EXTENSION(vkMergeValidationCachesEXT);
    const VkResult result = vkMergeValidationCachesEXT(MAGMA_HANDLE(device), handle, MAGMA_COUNT(dereferencedCaches), dereferencedCaches);
    MAGMA_THROW_FAILURE(result, "failed to merge validation caches");
}
#endif // VK_EXT_validation_cache
} // namespace magma
