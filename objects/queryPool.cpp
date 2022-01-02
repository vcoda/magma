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
#include <nmmintrin.h>
#include "queryPool.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
QueryPool::QueryPool(VkQueryType queryType, std::shared_ptr<Device> device, uint32_t queryCount,
    VkQueryControlFlags controlFlags, VkQueryPipelineStatisticFlags pipelineStatistics,
    std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_QUERY_POOL, std::move(device), std::move(allocator)),
    queryType(queryType),
    controlFlags(controlFlags),
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
inline std::vector<Type> QueryPool::getQueryResults(uint32_t firstQuery, uint32_t queryCount, VkQueryResultFlags flags) const noexcept
{
    std::vector<Type> data;
    try {
        data.resize(queryCount, {MAGMA_INVALID_QUERY_RESULT});
    } catch (...) {
        return {};
    }
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, firstQuery, queryCount,
        sizeof(Type) * data.size(), data.data(), sizeof(Type), flags);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    return data;
}

OcclusionQuery::OcclusionQuery(std::shared_ptr<Device> device, uint32_t queryCount, bool precise,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_OCCLUSION, std::move(device), queryCount, (precise ? VK_QUERY_CONTROL_PRECISE_BIT : 0), 0, std::move(allocator))
{}

std::vector<uint64_t> OcclusionQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResult<uint64_t>> OcclusionQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint64_t>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

PipelineStatisticsQuery::PipelineStatisticsQuery(std::shared_ptr<Device> device, VkQueryPipelineStatisticFlags flags,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_PIPELINE_STATISTICS, std::move(device), 1, 0, flags, std::move(allocator)),
    flags(flags)
{   // Pipeline statistics queries write one integer value for each bit
    // that is enabled in the pipelineStatistics when the pool is created.
    const int count = _mm_popcnt_u32(flags);
    // If VK_QUERY_RESULT_WITH_AVAILABILITY_BIT is used, the final element
    // of each query's result is an integer indicating whether the query's result
    // is available, with any non-zero value indicating that it is available.
    data.resize(count + 1, MAGMA_INVALID_QUERY_RESULT);
}

PipelineStatisticsQuery::Result PipelineStatisticsQuery::getResults(bool wait) const noexcept
{
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, 0, 1, sizeof(uint64_t) * data.size(), data.data(), sizeof(uint64_t),
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    if (VK_SUCCESS == result)
    {
        Result statistics;
        spreadResults(data, statistics);
        return statistics;
    }
    return {}; // VK_NOT_READY
}

QueryResult<PipelineStatisticsQuery::Result> PipelineStatisticsQuery::getResultsWithAvailability() const noexcept
{
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, 0, 1, sizeof(uint64_t) * data.size(), data.data(), sizeof(uint64_t),
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    if (VK_SUCCESS == result)
    {
        Result statistics;
        const uint32_t last = spreadResults(data, statistics);
        const int64_t availability = static_cast<int64_t>(data[last]);
        MAGMA_ASSERT(1 == availability); // Should always be 1 if result is VK_SUCCESS
        return QueryResult<Result>{statistics, availability};
    }
    return {}; // VK_NOT_READY
}

uint32_t PipelineStatisticsQuery::spreadResults(const std::vector<uint64_t>& data, Result& result) const noexcept
{
    memset(&result, 0, sizeof(Result));
    uint32_t i = 0;
    if (flags & VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT)
        result.inputAssemblyVertices = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT)
        result.inputAssemblyPrimitives = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT)
        result.vertexShaderInvocations = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT)
        result.geometryShaderInvocations = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT)
        result.geometryShaderPrimitives = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT)
        result.clippingInvocations = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT)
        result.clippingPrimitives = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT)
        result.fragmentShaderInvocations = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT)
        result.tesselationControlShaderPatches = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT)
        result.tesselationEvaluationShaderInvocations = data[i++];
    if (flags & VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT)
        result.computeShaderInvocations = data[i++];
    return i;
}

TimestampQuery::TimestampQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TIMESTAMP, std::move(device), queryCount, 0, 0, std::move(allocator))
{}

std::vector<uint64_t> TimestampQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResult<uint64_t>> TimestampQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint64_t>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

#ifdef VK_EXT_transform_feedback
TransformFeedbackStreamQuery::TransformFeedbackStreamQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT, std::move(device), queryCount, 0, 0, std::move(allocator))
{}

std::vector<TransformFeedbackStreamQuery::Result> TransformFeedbackStreamQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<Result>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResult<TransformFeedbackStreamQuery::Result>> TransformFeedbackStreamQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<Result>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}
#endif // VK_EXT_transform_feedback

#ifdef VK_NV_ray_tracing
AccelerationStructureCompactedSizeQuery::AccelerationStructureCompactedSizeQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_NV, std::move(device), queryCount, 0, 0, std::move(allocator))
{}

std::vector<uint64_t> AccelerationStructureCompactedSizeQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryResult<uint64_t>> AccelerationStructureCompactedSizeQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint64_t>>(firstQuery, queryCount,
        VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}
#endif // VK_NV_ray_tracing
} // namespace magma
