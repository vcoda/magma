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
#include "../misc/extProcAddress.h"
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
    VkQueryPoolCreateInfo queryPoolInfo;
    queryPoolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    queryPoolInfo.pNext = nullptr;
    queryPoolInfo.flags = 0;
    queryPoolInfo.queryType = queryType;
    queryPoolInfo.queryCount = queryCount;
    queryPoolInfo.pipelineStatistics = pipelineStatistics;
    const VkResult result = vkCreateQueryPool(MAGMA_HANDLE(device), &queryPoolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create query pool");
}

QueryPool::~QueryPool()
{
    vkDestroyQueryPool(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#ifdef VK_EXT_host_query_reset
void QueryPool::reset(uint32_t firstQuery, uint32_t queryCount) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkResetQueryPoolEXT);
    if (vkResetQueryPoolEXT)
        vkResetQueryPoolEXT(MAGMA_HANDLE(device), handle, firstQuery, queryCount);
}
#endif // VK_EXT_host_query_reset

IntegerQueryPool::IntegerQueryPool(VkQueryType queryType, std::shared_ptr<Device> device, uint32_t queryCount,
    VkQueryControlFlags controlFlags, std::shared_ptr<IAllocator> allocator):
    QueryPool(queryType, std::move(device), queryCount, controlFlags, 0, std::move(allocator))
{}

OcclusionQuery::OcclusionQuery(std::shared_ptr<Device> device, uint32_t queryCount, bool precise,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    IntegerQueryPool(VK_QUERY_TYPE_OCCLUSION, std::move(device), queryCount, precise ? VK_QUERY_CONTROL_PRECISE_BIT : 0, std::move(allocator))
{}

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
    data.resize(count + 1, BadQueryResult<uint64_t>::value);
}

PipelineStatisticsQuery::Result PipelineStatisticsQuery::getResults(bool wait) const noexcept
{
    const size_t dataSize = sizeof(uint64_t) * data.size();
    const VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
    std::fill(data.begin(), data.end(), BadQueryResult<uint64_t>::value);
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, 0, 1, dataSize, data.data(), sizeof(uint64_t), flags);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    if (VK_SUCCESS == result)
    {
        Result statistics;
        spreadResults(data, statistics);
        return statistics;
    }
    return {}; // VK_NOT_READY
}

QueryPool::Result<PipelineStatisticsQuery::Result, uint64_t> PipelineStatisticsQuery::getResultsWithAvailability() const noexcept
{
    const size_t dataSize = sizeof(uint64_t) * data.size();
    const VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    std::fill(data.begin(), data.end(), BadQueryResult<uint64_t>::value);
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, 0, 1, dataSize, data.data(), sizeof(uint64_t), flags);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    if (VK_SUCCESS == result)
    {
        Result statistics;
        const uint32_t last = spreadResults(data, statistics);
        const uint64_t availability = static_cast<uint64_t>(data[last]);
        MAGMA_ASSERT(1 == availability); // Should always be 1 if result is VK_SUCCESS
        return QueryPool::Result<Result, uint64_t>{statistics, availability};
    }
    return QueryPool::Result<Result, uint64_t>(); // VK_NOT_READY
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
    IntegerQueryPool(VK_QUERY_TYPE_TIMESTAMP, std::move(device), queryCount, 0, std::move(allocator))
{}

#ifdef VK_EXT_mesh_shader
MeshPrimitivesQuery::MeshPrimitivesQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    IntegerQueryPool(VK_QUERY_TYPE_MESH_PRIMITIVES_GENERATED_EXT, std::move(device), queryCount, 0, std::move(allocator))
{}
#endif // VK_EXT_mesh_shader

#ifdef VK_EXT_transform_feedback
TransformFeedbackQuery::TransformFeedbackQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT, std::move(device), queryCount, 0, 0, std::move(allocator))
{}

std::vector<TransformFeedbackQuery::Result> TransformFeedbackQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<Result>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryPool::Result<TransformFeedbackQuery::Result, uint64_t>> TransformFeedbackQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryPool::Result<Result, uint64_t>>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}
#endif // VK_EXT_transform_feedback

#ifdef VK_NV_ray_tracing
AccelerationStructureCompactedSizeQuery::AccelerationStructureCompactedSizeQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    IntegerQueryPool(VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_NV, std::move(device), queryCount, 0, std::move(allocator))
{}
#endif // VK_NV_ray_tracing
} // namespace magma
