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
#include "pch.h"
#pragma hdrstop
#include "queryPool.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"
#include "../core/popCount.h"

namespace magma
{
// Static inline variables are introduced since C++17 only:
// https://en.cppreference.com/w/cpp/language/static#Static_data_members
constexpr uint32_t BadQueryResult<uint32_t>::value;
constexpr uint64_t BadQueryResult<uint64_t>::value;

QueryPool::QueryPool(VkQueryType queryType, std::shared_ptr<Device> device, uint32_t queryCount,
    VkQueryControlFlags controlFlags, VkQueryPipelineStatisticFlags pipelineStatistics,
    std::shared_ptr<IAllocator> allocator,
    const StructureChain& extendedInfo):
    NonDispatchable(VK_OBJECT_TYPE_QUERY_POOL, std::move(device), std::move(allocator)),
    queryType(queryType),
    controlFlags(controlFlags),
    queryCount(queryCount)
{
    VkQueryPoolCreateInfo queryPoolInfo;
    queryPoolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    queryPoolInfo.pNext = extendedInfo.headNode();
    queryPoolInfo.flags = 0;
    queryPoolInfo.queryType = queryType;
    queryPoolInfo.queryCount = queryCount;
    queryPoolInfo.pipelineStatistics = pipelineStatistics;
    const VkResult result = vkCreateQueryPool(getNativeDevice(), &queryPoolInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create query pool");
}

QueryPool::~QueryPool()
{
    vkDestroyQueryPool(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#ifdef VK_EXT_host_query_reset
void QueryPool::reset(uint32_t firstQuery, uint32_t queryCount) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkResetQueryPoolEXT);
    if (vkResetQueryPoolEXT)
        vkResetQueryPoolEXT(getNativeDevice(), handle, firstQuery, queryCount);
}
#endif // VK_EXT_host_query_reset

IntegerQueryPool::IntegerQueryPool(VkQueryType queryType, std::shared_ptr<Device> device, uint32_t queryCount,
    VkQueryControlFlags controlFlags, std::shared_ptr<IAllocator> allocator, const StructureChain& extendedInfo):
    QueryPool(queryType, std::move(device), queryCount, controlFlags, 0, std::move(allocator), extendedInfo)
{}

OcclusionQuery::OcclusionQuery(std::shared_ptr<Device> device, uint32_t queryCount, bool precise,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    IntegerQueryPool(VK_QUERY_TYPE_OCCLUSION, std::move(device), queryCount,
        precise ? VK_QUERY_CONTROL_PRECISE_BIT : 0, std::move(allocator), extendedInfo)
{}

PipelineStatisticsQuery::PipelineStatisticsQuery(std::shared_ptr<Device> device, VkQueryPipelineStatisticFlags flags,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    QueryPool(VK_QUERY_TYPE_PIPELINE_STATISTICS, std::move(device), 1, 0, flags, std::move(allocator), extendedInfo),
    flags(flags)
{   // Pipeline statistics queries write one integer value for each bit
    // that is enabled in the pipelineStatistics when the pool is created.
    const int count = core::popCount(flags);
    // If VK_QUERY_RESULT_WITH_AVAILABILITY_BIT is used, the final element
    // of each query's result is an integer indicating whether the query's result
    // is available, with any non-zero value indicating that it is available.
    data.resize(count + 1, BadQueryResult<uint64_t>::value);
}

PipelineStatisticsQuery::Result PipelineStatisticsQuery::getResults(bool wait) const
{
    const size_t dataSize = sizeof(uint64_t) * data.size();
    const VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
    std::fill(data.begin(), data.end(), BadQueryResult<uint64_t>::value);
    const VkResult result = vkGetQueryPoolResults(getNativeDevice(), handle, 0, 1, dataSize, data.data(), sizeof(uint64_t), flags);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    if (VK_SUCCESS == result)
    {
        Result statistics;
        spreadResults(data, statistics);
        return statistics;
    }
    return {}; // VK_NOT_READY
}

QueryPool::Result<PipelineStatisticsQuery::Result, uint64_t> PipelineStatisticsQuery::getResultsWithAvailability() const
{
    const size_t dataSize = sizeof(uint64_t) * data.size();
    const VkQueryResultFlags flags = VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    std::fill(data.begin(), data.end(), BadQueryResult<uint64_t>::value);
    const VkResult result = vkGetQueryPoolResults(getNativeDevice(), handle, 0, 1, dataSize, data.data(), sizeof(uint64_t), flags);
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
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    IntegerQueryPool(VK_QUERY_TYPE_TIMESTAMP, std::move(device), queryCount, 0, std::move(allocator), extendedInfo)
{}

#ifdef VK_KHR_performance_query
PerformanceQuery::PerformanceQuery(std::shared_ptr<Device> device, uint32_t queueFamilyIndex, const std::vector<uint32_t>& counterIndices,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    QueryPool(VK_QUERY_TYPE_PERFORMANCE_QUERY_KHR, std::move(device), 1, 0, 0, std::move(allocator),
        StructureChain(
            VkQueryPoolPerformanceCreateInfoKHR{
                VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR,
                nullptr,
                queueFamilyIndex,
                MAGMA_COUNT(counterIndices),
                counterIndices.data()
            })),
    queueFamilyIndex(queueFamilyIndex),
    counterIndices(counterIndices)
{}

uint32_t PerformanceQuery::getNumPasses() const
{
    return device->getPhysicalDevice()->getNumQueueFamilyPerformanceQueryPasses(queueFamilyIndex, counterIndices);
}

std::vector<VkPerformanceCounterResultKHR> PerformanceQuery::getResults(bool wait) const
{
    std::vector<VkPerformanceCounterResultKHR> counters(counterIndices.size(), VkPerformanceCounterResultKHR{});
    const VkResult result = vkGetQueryPoolResults(getNativeDevice(), handle, 0, 1,
        sizeof(VkPerformanceCounterResultKHR) * counters.size(),
        counters.data(),
        sizeof(VkPerformanceCounterResultKHR) * counters.size(),
        // If the queryType used to create queryPool was
        // VK_QUERY_TYPE_PERFORMANCE_QUERY_KHR, flags must not
        // contain VK_QUERY_RESULT_WITH_AVAILABILITY_BIT,
        // VK_QUERY_RESULT_WITH_STATUS_BIT_KHR,
        // VK_QUERY_RESULT_PARTIAL_BIT, or VK_QUERY_RESULT_64_BIT.
        wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    return counters;
}
#endif // VK_KHR_performance_query

#ifdef VK_EXT_mesh_shader
MeshPrimitivesQuery::MeshPrimitivesQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    IntegerQueryPool(VK_QUERY_TYPE_MESH_PRIMITIVES_GENERATED_EXT, std::move(device), queryCount, 0, std::move(allocator), extendedInfo)
{}
#endif // VK_EXT_mesh_shader

#ifdef VK_EXT_transform_feedback
TransformFeedbackQuery::TransformFeedbackQuery(std::shared_ptr<Device> device, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    QueryPool(VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT, std::move(device), queryCount, 0, 0, std::move(allocator), extendedInfo)
{}

std::vector<TransformFeedbackQuery::Result> TransformFeedbackQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const
{
    return getQueryResults<Result>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

std::vector<QueryPool::Result<TransformFeedbackQuery::Result, uint64_t>> TransformFeedbackQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const
{
    return getQueryResults<QueryPool::Result<Result, uint64_t>>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}
#endif // VK_EXT_transform_feedback

#ifdef VK_KHR_acceleration_structure
AccelerationStructureQuery::AccelerationStructureQuery(std::shared_ptr<Device> device, Type queryType, uint32_t queryCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    IntegerQueryPool(toVkType(queryType), std::move(device), queryCount, 0, std::move(allocator), extendedInfo)
{}

VkQueryType AccelerationStructureQuery::toVkType(Type queryType) noexcept
{
    switch (queryType)
    {
    case CompactedSize:
        return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
    case SerializationSize:
        return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR;
#ifdef VK_KHR_ray_tracing_maintenance1
    case Size:
        return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SIZE_KHR;
    case BottomLevelPointers:
        return VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_BOTTOM_LEVEL_POINTERS_KHR;
#endif // VK_KHR_ray_tracing_maintenance1
    }
    MAGMA_ASSERT(false);
    return VK_QUERY_TYPE_MAX_ENUM;
}
#endif // VK_KHR_acceleration_structure
} // namespace magma
