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
#include "queryPool.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
QueryPool::QueryPool(VkQueryType queryType, std::shared_ptr<Device> device, uint32_t queryCount,
    VkQueryPipelineStatisticFlags pipelineStatistics, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_QUERY_POOL, std::move(device), std::move(allocator)),
    queryType(queryType),
    queryCount(queryCount)
{
    VkQueryPoolCreateInfo queryInfo;
    queryInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    queryInfo.pNext = nullptr;
    queryInfo.flags = 0;
    queryInfo.queryType = queryType;
    queryInfo.queryCount = queryCount;
    queryInfo.pipelineStatistics = pipelineStatistics;
    const VkResult result = vkCreateQueryPool(MAGMA_HANDLE(device), &queryInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create query pool");
}

QueryPool::~QueryPool()
{
    vkDestroyQueryPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

template<typename Type>
inline std::vector<Type> QueryPool::getQueryResults(uint32_t firstQuery, uint32_t queryCount, VkQueryResultFlags flags) const
{
    constexpr VkDeviceSize stride = sizeof(Type);
    std::vector<Type> data(queryCount);
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, firstQuery, queryCount, sizeof(Type) * data.size(), data.data(), stride, flags);
    if (!MAGMA_SUCCEEDED(result))
        data.clear();
    return data;
}

OcclusionQuery::OcclusionQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_OCCLUSION, std::move(device), queryCount, 0, std::move(allocator))
{}

std::vector<uint64_t> OcclusionQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResultWithAvailability<uint64_t>> OcclusionQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<QueryResultWithAvailability<uint64_t>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

PipelineStatisticsQuery::PipelineStatisticsQuery(std::shared_ptr<Device> device,
    VkQueryPipelineStatisticFlags pipelineStatistics,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_PIPELINE_STATISTICS, std::move(device), 1, pipelineStatistics, std::move(allocator)),
    pipelineStatistics(pipelineStatistics)
{}

std::vector<uint64_t> PipelineStatisticsQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResultWithAvailability<uint64_t>> PipelineStatisticsQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<QueryResultWithAvailability<uint64_t>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

TimestampQuery::TimestampQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TIMESTAMP, std::move(device), queryCount, 0, std::move(allocator))
{}

std::vector<uint64_t> TimestampQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResultWithAvailability<uint64_t>> TimestampQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<QueryResultWithAvailability<uint64_t>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

#ifdef VK_EXT_transform_feedback
TransformFeedbackStreamQuery::TransformFeedbackStreamQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT, std::move(device), queryCount, 0, std::move(allocator))
{}

std::vector<TransformFeedbackStreamQuery::Result> TransformFeedbackStreamQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<Result>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResultWithAvailability<TransformFeedbackStreamQuery::Result>> TransformFeedbackStreamQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<QueryResultWithAvailability<Result>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}
#endif // VK_EXT_transform_feedback

#ifdef VK_NV_ray_tracing
AccelerationStructureCompactedSizeQuery::AccelerationStructureCompactedSizeQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_NV, std::move(device), queryCount, 0, std::move(allocator))
{}
#endif // VK_NV_ray_tracing
} // namespace magma
