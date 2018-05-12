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
#include "queryPool.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
QueryPool::QueryPool(VkQueryType queryType, std::shared_ptr<const Device> device, uint32_t queryCount,
    VkQueryPipelineStatisticFlags pipelineStatistics, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT, device, allocator),
    queryCount(queryCount)
{
    VkQueryPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.queryType = queryType;
    info.queryCount = queryCount;
    info.pipelineStatistics = pipelineStatistics;
    const VkResult create = vkCreateQueryPool(*device, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create query pool");
}

QueryPool::~QueryPool()
{
    vkDestroyQueryPool(*device, handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

std::vector<uint64_t> QueryPool::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const
{
    VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT;
    if (wait)
        flags |= VK_QUERY_RESULT_WAIT_BIT;
    else
        flags |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    // TODO: VK_QUERY_RESULT_PARTIAL_BIT
    const VkDeviceSize stride = sizeof(uint64_t);
    std::vector<uint64_t> results(queryCount);
    const VkResult get = vkGetQueryPoolResults(*device, handle, 
        firstQuery, queryCount,
        sizeof(uint64_t) * results.size(), results.data(), stride, 
        flags);
    if (MAGMA_SUCCEEDED(get))
        return std::move(results);
    return std::vector<uint64_t>();
}

OcclusionQuery::OcclusionQuery(std::shared_ptr<const Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_OCCLUSION, device, queryCount, 0, allocator)
{}

PipelineStatisticsQuery::PipelineStatisticsQuery(std::shared_ptr<const Device> device, 
    VkQueryPipelineStatisticFlags pipelineStatistics,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_PIPELINE_STATISTICS, device, 1, pipelineStatistics, allocator),
    pipelineStatistics(pipelineStatistics)
{}

TimestampQuery::TimestampQuery(std::shared_ptr<const Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TIMESTAMP, device, queryCount, 0, allocator)
{}
} // namespace magma
