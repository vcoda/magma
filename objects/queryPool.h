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
    class QueryPool : public NonDispatchable<VkQueryPool>
    {
    public:
        ~QueryPool();
        virtual std::vector<uint64_t> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const noexcept;
        uint32_t getQueryCount() const noexcept { return queryCount; }

    protected:
        QueryPool(VkQueryType queryType,
            std::shared_ptr<const Device> device,
            uint32_t queryCount,
            VkQueryPipelineStatisticFlags pipelineStatistics,
            std::shared_ptr<IAllocator> allocator);

    protected:
        uint32_t queryCount;
    };

    class OcclusionQuery : public QueryPool
    {
    public:
        OcclusionQuery(std::shared_ptr<const Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    class PipelineStatisticsQuery : public QueryPool
    {
    public:
        PipelineStatisticsQuery(std::shared_ptr<const Device> device,
            VkQueryPipelineStatisticFlags pipelineStatistics,
            std::shared_ptr<IAllocator> allocator = nullptr);
        VkQueryPipelineStatisticFlags getStatisticFlags() const { return pipelineStatistics; }

    private:
        VkQueryPipelineStatisticFlags pipelineStatistics;
    };

    class TimestampQuery : public QueryPool
    {
    public:
        TimestampQuery(std::shared_ptr<const Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma
