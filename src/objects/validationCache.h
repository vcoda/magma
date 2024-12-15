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
#pragma once
#include "model/nondispatchable.h"

namespace magma
{
    class Device;
    class IAllocator;

    /* Validation cache objects allow the result of internal
       validation to be reused, both within a single application
       run and between multiple runs. Reuse within a single run
       is achieved by passing the same validation cache object
       when creating supported Vulkan objects. Reuse across runs
       of an application is achieved by retrieving validation cache
       contents in one run of an application, saving the contents,
       and using them to preinitialize a validation cache on a
       subsequent run. The contents of the validation cache objects
       are managed by the validation layers. Applications can
       manage the host memory consumed by a validation cache object
       and control the amount of data retrieved from a validation
       cache object. */

#ifdef VK_EXT_validation_cache
    class ValidationCache : public NonDispatchable<VkValidationCacheEXT>
    {
    public:
        struct Header;
        explicit ValidationCache(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit ValidationCache(std::shared_ptr<Device> device,
            std::size_t dataSize,
            const void *cacheData,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        template<class Type>
        explicit ValidationCache(std::shared_ptr<Device> device,
            const std::vector<Type>& cacheData,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain()):
            ValidationCache(std::move(device), cacheData.size() * sizeof(Type),
                cacheData.data(), std::move(allocator), extendedInfo) {}
        ~ValidationCache();
        std::vector<uint8_t> getData() const;
        void mergeCaches(const std::vector<lent_ptr<const ValidationCache>>& caches);
    };

    struct ValidationCache::Header
    {
        uint32_t size = 0;
        VkValidationCacheHeaderVersionEXT version =
            VK_VALIDATION_CACHE_HEADER_VERSION_ONE_EXT;
        uint8_t cacheUUID[VK_UUID_SIZE] = {};
    };
#endif // VK_EXT_validation_cache
} // namespace magma
