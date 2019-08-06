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
#include "validationCache.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"
#include "../helpers/stackArray.h"

namespace magma
{
ValidationCache::ValidationCache(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    ValidationCache(std::move(device), 0, nullptr, std::move(allocator))
{}

ValidationCache::ValidationCache(std::shared_ptr<Device> device,
    size_t dataSize, const void *cacheData,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_VALIDATION_CACHE_EXT, std::move(device), std::move(allocator))
{
    MAGMA_DEVICE_EXTENSION(vkCreateValidationCacheEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    VkValidationCacheCreateInfoEXT info;
    info.sType = VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT;
    info.pNext = nullptr;
    info.flags = 0;
    info.initialDataSize = dataSize;
    info.pInitialData = cacheData;
    const VkResult create = vkCreateValidationCacheEXT(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create validation cache");
}

ValidationCache::~ValidationCache()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyValidationCacheEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    vkDestroyValidationCacheEXT(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

std::vector<uint8_t> ValidationCache::getData() const
{
    MAGMA_DEVICE_EXTENSION(vkGetValidationCacheDataEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    size_t dataSize;
    const VkResult getSize = vkGetValidationCacheDataEXT(MAGMA_HANDLE(device), handle, &dataSize, nullptr);
    MAGMA_THROW_FAILURE(getSize, "failed to get validation cache size");
    std::vector<uint8_t> data(dataSize);
    const VkResult getData = vkGetValidationCacheDataEXT(MAGMA_HANDLE(device), handle, &dataSize, data.data());
    MAGMA_THROW_FAILURE(getData, "failed to get validation cache data");
    return data;
}

void ValidationCache::mergeCaches(const std::vector<std::shared_ptr<const ValidationCache>>& caches)
{
    MAGMA_DEVICE_EXTENSION(vkMergeValidationCachesEXT, VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
    MAGMA_STACK_ARRAY(VkValidationCacheEXT, dereferencedCaches, caches.size());
    for (const auto& cache : caches)
        dereferencedCaches.put(*cache);
    const VkResult merge = vkMergeValidationCachesEXT(MAGMA_HANDLE(device), handle, MAGMA_COUNT(dereferencedCaches), dereferencedCaches);
    MAGMA_THROW_FAILURE(merge, "failed to merge validation caches");
}
} // namespace magma
