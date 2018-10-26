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
QueryPool::QueryPool(VkQueryType queryType, std::shared_ptr<Device> device, uint32_t queryCount,
    VkQueryPipelineStatisticFlags pipelineStatistics, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_QUERY_POOL, std::move(device), std::move(allocator)),
    queryType(queryType),
    queryCount(queryCount)
{
    VkQueryPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.queryType = queryType;
    info.queryCount = queryCount;
    info.pipelineStatistics = pipelineStatistics;
    const VkResult create = vkCreateQueryPool(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create query pool");
}

QueryPool::~QueryPool()
{
    vkDestroyQueryPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

std::vector<uint64_t> QueryPool::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT;
    if (wait)
        flags |= VK_QUERY_RESULT_WAIT_BIT;
    else
        flags |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    // TODO: VK_QUERY_RESULT_PARTIAL_BIT
    const VkDeviceSize stride = sizeof(uint64_t);
    std::vector<uint64_t> results(queryCount);
    const VkResult get = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle,
        firstQuery, queryCount,
        sizeof(uint64_t) * results.size(), results.data(), stride,
        flags);
    if (MAGMA_SUCCEEDED(get))
        return results;
    return {};
}

OcclusionQuery::OcclusionQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_OCCLUSION, std::move(device), queryCount, 0, std::move(allocator))
{}

PipelineStatisticsQuery::PipelineStatisticsQuery(std::shared_ptr<Device> device,
    VkQueryPipelineStatisticFlags pipelineStatistics,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_PIPELINE_STATISTICS, std::move(device), 1, pipelineStatistics, std::move(allocator)),
    pipelineStatistics(pipelineStatistics)
{}

TimestampQuery::TimestampQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TIMESTAMP, std::move(device), queryCount, 0, std::move(allocator))
{}

CompactedSizeQuery::CompactedSizeQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_COMPACTED_SIZE_NVX, std::move(device), queryCount, 0, std::move(allocator))
{}
} // namespace magma
