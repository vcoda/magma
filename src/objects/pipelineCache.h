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
#include "nondispatchable.h"

namespace magma
{
    class Device;
    class IAllocator;

    /* Pipeline cache objects allow the result of pipeline construction
       to be reused between pipelines and between runs of an application.
       Reuse between pipelines is achieved by passing the same pipeline
       cache object when creating multiple related pipelines. Reuse across
       runs of an application is achieved by retrieving pipeline cache
       contents in one run of an application, saving the contents, and
       using them to preinitialize a pipeline cache on a subsequent run.
       The contents of the pipeline cache objects are managed by the
       implementation. */

    class PipelineCache : public NonDispatchable<VkPipelineCache>
    {
    public:
        struct Header;
        explicit PipelineCache(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit PipelineCache(std::shared_ptr<Device> device,
            std::size_t dataSize,
            const void *initialData,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPipelineCacheCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~PipelineCache();
        std::vector<uint8_t> getData() const;
        void mergeCache(std::shared_ptr<const PipelineCache> srcCache);
        void mergeCaches(const std::vector<std::shared_ptr<const PipelineCache>>& srcCaches);
    };

    /* To enable applications to detect when previously retrieved
       data  is incompatible with the device, the pipeline cache
       data must begin with a valid pipeline cache header. Version 1
       of the pipeline cache header is defined as: */

    struct PipelineCache::Header
    {
        uint32_t size = 0;
        VkPipelineCacheHeaderVersion version =
            VK_PIPELINE_CACHE_HEADER_VERSION_ONE;
        uint32_t vendorID = 0;
        uint32_t deviceID = 0;
        uint8_t pipelineCacheUUID[VK_UUID_SIZE] = {};
    };

    static_assert(sizeof(PipelineCache::Header) == sizeof(VkPipelineCacheHeaderVersionOne),
        "pipeline cache header structure size mismatch");
} // namespace magma
