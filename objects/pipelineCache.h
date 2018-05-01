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
#pragma once
#include <vector>
#include "handle.h"

namespace magma
{
    class Device;
    class IAllocator;

    class PipelineCache : public NonDispatchable<VkPipelineCache>
    {
    public:
        struct Header
        {
            uint32_t size = 0;
            uint32_t version = 0;
            uint32_t vendorID = 0;
            uint32_t deviceID = 0;
            uint8_t cacheUUID[VK_UUID_SIZE] = {};
        };

    public:
        PipelineCache(std::shared_ptr<const Device> device,
            const std::vector<uint8_t>& cacheData = {},
            std::shared_ptr<IAllocator> allocator = nullptr);
        PipelineCache(std::shared_ptr<const Device> device,
            size_t dataSize,
            const void *cacheData,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~PipelineCache();
        std::vector<uint8_t> getData() const;
        void mergeCaches(const std::vector<std::shared_ptr<const PipelineCache>>& caches);
    };
} // namespace magma
