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
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    ValidationCache(std::move(device), 0, nullptr, std::move(allocator))
{}

ValidationCache::ValidationCache(std::shared_ptr<Device> device,
    std::size_t dataSize, const void *cacheData,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_VALIDATION_CACHE_EXT, std::move(device), std::move(allocator))
{
    MAGMA_DEVICE_EXTENSION(vkCreateValidationCacheEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    VkValidationCacheCreateInfoEXT cacheInfo;
    cacheInfo.sType = VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT;
    cacheInfo.pNext = nullptr;
    cacheInfo.flags = 0;
    cacheInfo.initialDataSize = dataSize;
    cacheInfo.pInitialData = cacheData;
    const VkResult result = vkCreateValidationCacheEXT(MAGMA_HANDLE(device), &cacheInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create validation cache");
}

ValidationCache::~ValidationCache()
{
    MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDestroyValidationCacheEXT); // Do not throw exception
    if (vkDestroyValidationCacheEXT)
        vkDestroyValidationCacheEXT(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

std::vector<uint8_t> ValidationCache::getData() const
{
    MAGMA_DEVICE_EXTENSION(vkGetValidationCacheDataEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    std::size_t dataSize;
    VkResult result = vkGetValidationCacheDataEXT(MAGMA_HANDLE(device), handle, &dataSize, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to get validation cache size");
    std::vector<uint8_t> data(dataSize);
    result = vkGetValidationCacheDataEXT(MAGMA_HANDLE(device), handle, &dataSize, data.data());
    MAGMA_THROW_FAILURE(result, "failed to get validation cache data");
    return data;
}

void ValidationCache::mergeCaches(const std::vector<std::shared_ptr<const ValidationCache>>& caches)
{
    MAGMA_DEVICE_EXTENSION(vkMergeValidationCachesEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    MAGMA_STACK_ARRAY(VkValidationCacheEXT, dereferencedCaches, caches.size());
    for (const auto& cache : caches)
        dereferencedCaches.put(*cache);
    const VkResult result = vkMergeValidationCachesEXT(MAGMA_HANDLE(device), handle, MAGMA_COUNT(dereferencedCaches), dereferencedCaches);
    MAGMA_THROW_FAILURE(result, "failed to merge validation caches");
}
#endif // VK_EXT_validation_cache
} // namespace magma
