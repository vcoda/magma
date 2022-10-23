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
#pragma once
#include "nondispatchable.h"

namespace magma
{
    /* If VK_QUERY_RESULT_WITH_AVAILABILITY_BIT is used, the final element
       of each query's result is an integer indicating whether the query's result
       is available, with any non-zero value indicating that it is available. */

    template<typename Type>
    struct QueryResult
    {
        Type result; // Depends on query type
        int64_t availability = 0;
    };

    /* Invalid query result value. */

    template<class Int>
    struct BadQueryResult
    {
        static const Int value;
    };

#ifdef MAGMA_DEBUG
    template<> struct BadQueryResult<uint32_t> { static const uint32_t value = 0xBAADC0DE; };
    template<> struct BadQueryResult<uint64_t> { static const uint64_t value = 0xBAADC0DEBAADC0DE; };
#else
    template<> struct BadQueryResult<uint32_t> { static const uint32_t value = 0u; };
    template<> struct BadQueryResult<uint64_t> { static const uint64_t value = 0ull; };
#endif

    /* An object that contains a number of query entries
       and their associated state and results.
       Queries are managed using query pool objects. */

    class QueryPool : public NonDispatchable<VkQueryPool>
    {
    public:
        ~QueryPool();
        VkQueryType getType() const noexcept { return queryType; }
        VkQueryControlFlags getControlFlags() const noexcept { return controlFlags; }
        uint32_t getQueryCount() const noexcept { return queryCount; }
    #ifdef VK_EXT_host_query_reset
        void reset(uint32_t firstQuery, uint32_t queryCount) noexcept;
    #endif

    protected:
        explicit QueryPool(VkQueryType queryType,
            std::shared_ptr<Device> device,
            uint32_t queryCount,
            VkQueryControlFlags controlFlags,
            VkQueryPipelineStatisticFlags pipelineStatistics,
            std::shared_ptr<IAllocator> allocator);
        template<typename Type>
        std::vector<Type> getQueryResults(uint32_t firstQuery,
            uint32_t queryCount,
            VkQueryResultFlags flags) const noexcept;

    protected:
        const VkQueryType queryType;
        const VkQueryControlFlags controlFlags;
        const uint32_t queryCount;
    };

    /* Occlusion queries track the number of samples that pass
       the per-fragment tests for a set of drawing commands.
       The application can then use these results to inform
       future rendering decisions. */

    class OcclusionQuery : public QueryPool
    {
    public:
        explicit OcclusionQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            bool precise,
            std::shared_ptr<IAllocator> allocator = nullptr);
        bool precise() const noexcept { return controlFlags & VK_QUERY_CONTROL_PRECISE_BIT; }
        std::vector<uint64_t> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const noexcept;
        std::vector<QueryResult<uint64_t>> getResultsWithAvailability(uint32_t firstQuery,
            uint32_t queryCount) const noexcept;
    };

    /* Pipeline statistics queries allow the application to sample
       a specified set of pipeline counters. These counters are
       accumulated by Vulkan for a set of either draw or dispatch commands
       while a pipeline statistics query is active. */

    class PipelineStatisticsQuery : public QueryPool
    {
    public:
        struct Result
        {
            uint64_t inputAssemblyVertices = BadQueryResult<uint64_t>::value;
            uint64_t inputAssemblyPrimitives = BadQueryResult<uint64_t>::value;
            uint64_t vertexShaderInvocations = BadQueryResult<uint64_t>::value;
            uint64_t geometryShaderInvocations = BadQueryResult<uint64_t>::value;
            uint64_t geometryShaderPrimitives = BadQueryResult<uint64_t>::value;
            uint64_t clippingInvocations = BadQueryResult<uint64_t>::value;
            uint64_t clippingPrimitives = BadQueryResult<uint64_t>::value;
            uint64_t fragmentShaderInvocations = BadQueryResult<uint64_t>::value;
            uint64_t tesselationControlShaderPatches = BadQueryResult<uint64_t>::value;
            uint64_t tesselationEvaluationShaderInvocations = BadQueryResult<uint64_t>::value;
            uint64_t computeShaderInvocations = BadQueryResult<uint64_t>::value;
        };

        explicit PipelineStatisticsQuery(std::shared_ptr<Device> device,
            VkQueryPipelineStatisticFlags pipelineStatistics,
            std::shared_ptr<IAllocator> allocator = nullptr);
        VkQueryPipelineStatisticFlags getStatisticFlags() const noexcept { return flags; }
        Result getResults(bool wait) const noexcept;
        QueryResult<Result> getResultsWithAvailability() const noexcept;

    private:
        uint32_t spreadResults(const std::vector<uint64_t>& data,
            Result& result) const noexcept;

        const VkQueryPipelineStatisticFlags flags;
        mutable std::vector<uint64_t> data;
    };

    /* Timestamps provide applications with a mechanism
       for timing the execution of commands. A timestamp
       is an integer value generated by the physical device. */

    class TimestampQuery : public QueryPool
    {
    public:
        explicit TimestampQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr);
        std::vector<uint64_t> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const noexcept;
        std::vector<QueryResult<uint64_t>> getResultsWithAvailability(uint32_t firstQuery,
            uint32_t queryCount) const noexcept;
    };

    /* Transform feedback queries track the number of primitives attempted to be written and
       actually written, by the vertex stream being captured, to a transform feedback buffer.
       This query is updated during drawing commands while transform feedback is active.
       The number of primitives actually written will be less than the number attempted to be written
       if the bound transform feedback buffer size was too small for the number of primitives actually drawn. */

#ifdef VK_EXT_transform_feedback
    class TransformFeedbackStreamQuery : public QueryPool
    {
    public:
        struct Result
        {
            uint64_t numPrimitivesWritten = BadQueryResult<uint64_t>::value;
            uint64_t numPrimitivesNeeded = BadQueryResult<uint64_t>::value;
        };

        explicit TransformFeedbackStreamQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr);
        std::vector<Result> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const noexcept;
        std::vector<QueryResult<Result>> getResultsWithAvailability(uint32_t firstQuery,
            uint32_t queryCount) const noexcept;
    };
#endif // VK_EXT_transform_feedback

    /* The acceleration structure object may be compacted
       in order to improve performance. Before copying, an application
       must query the size of the resulting acceleration structure. */

#ifdef VK_NV_ray_tracing
    class AccelerationStructureCompactedSizeQuery : public QueryPool
    {
    public:
        explicit AccelerationStructureCompactedSizeQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr);
        std::vector<uint64_t> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const noexcept;
        std::vector<QueryResult<uint64_t>> getResultsWithAvailability(uint32_t firstQuery,
            uint32_t queryCount) const noexcept;
    };
#endif // VK_NV_ray_tracing
} // namespace magma
