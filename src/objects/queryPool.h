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
#include "model/nondispatchable.h"

namespace magma
{
    /* Invalid query result value. */

    template<class Int>
    struct BadQueryResult
    {
        static constexpr Int value = 0;
    };

#ifdef MAGMA_DEBUG
    template<> struct BadQueryResult<uint32_t> { static constexpr uint32_t value = MAGMA_BAD_CODE; };
    template<> struct BadQueryResult<uint64_t> { static constexpr uint64_t value = MAGMA_BAD_CODE | uint64_t(MAGMA_BAD_CODE) << 32; };
#else
    template<> struct BadQueryResult<uint32_t> { static constexpr uint32_t value = 0u; };
    template<> struct BadQueryResult<uint64_t> { static constexpr uint64_t value = 0ull; };
#endif // MAGMA_DEBUG

    /* An object that contains a number of query entries
       and their associated state and results.
       Queries are managed using query pool objects. */

    class QueryPool : public NonDispatchable<VkQueryPool>
    {
    public:
        template<class Type, class Int>
        struct Result
        {
            static_assert(std::is_same<Int, uint32_t>::value || std::is_same<Int, uint64_t>::value,
                "query result should be of unsigned int or long long type");
            Type result = {BadQueryResult<Int>::value};
            // If VK_QUERY_RESULT_WITH_AVAILABILITY_BIT is used,
            // the final element of each query's result is an integer
            // indicating whether the query's result is available,
            // with any non-zero value indicating that it is available.
            Int availability = BadQueryResult<Int>::value;
        };

        ~QueryPool();
        VkQueryType getType() const noexcept { return queryType; }
        VkQueryControlFlags getControlFlags() const noexcept { return controlFlags; }
        uint32_t getQueryCount() const noexcept { return queryCount; }
    #ifdef VK_EXT_host_query_reset
        void reset(uint32_t firstQuery, uint32_t queryCount) noexcept;
    #endif

    protected:
        QueryPool(VkQueryType queryType,
            std::shared_ptr<Device> device,
            uint32_t queryCount,
            VkQueryControlFlags controlFlags,
            VkQueryPipelineStatisticFlags pipelineStatistics,
            std::shared_ptr<IAllocator> allocator,
            const StructureChain& extendedInfo);
        template<class Type>
        std::vector<Type> getQueryResults(uint32_t firstQuery,
            uint32_t queryCount,
            VkQueryResultFlags flags) const;

    protected:
        const VkQueryType queryType;
        const VkQueryControlFlags controlFlags;
        const uint32_t queryCount;
    };

    /* Query which result is of uint32/64 types. */

    class IntegerQueryPool : public QueryPool
    {
    public:
        template<class Type>
        std::vector<Type> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const;
        template<class Type>
        std::vector<Result<Type, Type>> getResultsWithAvailability(uint32_t firstQuery,
            uint32_t queryCount) const;

    protected:
        IntegerQueryPool(VkQueryType queryType,
            std::shared_ptr<Device> device,
            uint32_t queryCount,
            VkQueryControlFlags controlFlags,
            std::shared_ptr<IAllocator> allocator,
            const StructureChain& extendedInfo);
    };

    /* Occlusion queries track the number of samples that pass
       the per-fragment tests for a set of drawing commands.
       The application can then use these results to inform
       future rendering decisions. */

    class OcclusionQuery : public IntegerQueryPool
    {
    public:
        explicit OcclusionQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            bool precise,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        bool precise() const noexcept { return controlFlags & VK_QUERY_CONTROL_PRECISE_BIT; }
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
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        VkQueryPipelineStatisticFlags getStatisticFlags() const noexcept { return flags; }
        Result getResults(bool wait) const;
        QueryPool::Result<Result, uint64_t> getResultsWithAvailability() const;

    private:
        uint32_t spreadResults(const std::vector<uint64_t>& data,
            Result& result) const noexcept;

        const VkQueryPipelineStatisticFlags flags;
        mutable std::vector<uint64_t> data;
    };

    /* Timestamps provide applications with a mechanism
       for timing the execution of commands. A timestamp
       is an integer value generated by the physical device. */

    class TimestampQuery : public IntegerQueryPool
    {
    public:
        explicit TimestampQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    };

    /* Adds a mechanism to allow querying of performance counters
       for use in applications and by profiling tools. Each queue
       family may expose counters that can be enabled on a queue
       of that family. */

#ifdef VK_KHR_performance_query
    class PerformanceQuery : public QueryPool
    {
    public:
        explicit PerformanceQuery(std::shared_ptr<Device> device,
            uint32_t queueFamilyIndex,
            const std::vector<uint32_t>& counterIndices,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }
        const std::vector<uint32_t>& getCounterIndices() const noexcept { return counterIndices; }
        uint32_t getNumPasses() const;
        std::vector<VkPerformanceCounterResultKHR> getResults(bool wait) const;

    private:
        const uint32_t queueFamilyIndex;
        const std::vector<uint32_t> counterIndices;
    };
#endif // VK_KHR_performance_query

    /* The mesh-primitives-generated count is incremented every time
       a primitive emitted from the mesh shader stage reaches the
       fragment shader stage. When a generated mesh primitives query
       begins, the mesh-primitives-generated count starts from zero. */

#ifdef VK_EXT_mesh_shader
    class MeshPrimitivesQuery : public IntegerQueryPool
    {
    public:
        explicit MeshPrimitivesQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
    };
#endif // VK_EXT_mesh_shader

    /* Transform feedback queries track the number of primitives
       attempted to be written and actually written, by the vertex
       stream being captured, to a transform feedback buffer.
       This query is updated during drawing commands while transform
       feedback is active. The number of primitives actually written
       will be less than the number attempted to be written
       if the bound transform feedback buffer size was too small
       for the number of primitives actually drawn. */

#ifdef VK_EXT_transform_feedback
    class TransformFeedbackQuery : public QueryPool
    {
    public:
        struct Result
        {
            uint64_t numPrimitivesWritten = BadQueryResult<uint64_t>::value;
            uint64_t numPrimitivesNeeded = BadQueryResult<uint64_t>::value;
        };

        explicit TransformFeedbackQuery(std::shared_ptr<Device> device,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        std::vector<Result> getResults(uint32_t firstQuery,
            uint32_t queryCount,
            bool wait) const;
        std::vector<QueryPool::Result<Result, uint64_t>> getResultsWithAvailability(uint32_t firstQuery,
            uint32_t queryCount) const;
    };
#endif // VK_EXT_transform_feedback

    /* The acceleration structure object may be compacted in order
       to improve performance. Before copying, an application must
       query the size of the resulting acceleration structure.
       The acceleration structure object may be serialized in order
       to store memory footprint on disk for faster loading. */

#ifdef VK_KHR_acceleration_structure
    class AccelerationStructureQuery : public IntegerQueryPool
    {
    public:
        enum class Type : uint8_t;
        explicit AccelerationStructureQuery(std::shared_ptr<Device> device,
            Type queryType,
            uint32_t queryCount,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());

    private:
        static VkQueryType castType(Type queryType) noexcept;
    };

    /* The following values are used when writing acceleration structure
       properties:

        * CompactedSize: Number of bytes required by a compacted
          acceleration structure.
        * SerializationSize: Number of bytes required by a serialized
          acceleration structure.
        * Size: The acceleration structure size on the device timeline.
        * BottomLevelPointers: A 64-bit integer of the count of the
          number of acceleration structure handles. This will be zero
          for a bottom-level acceleration structure. For top-level
          acceleration structures this number is implementation-dependent. */

    enum class AccelerationStructureQuery::Type : uint8_t
    {
        CompactedSize, SerializationSize,
    #ifdef VK_KHR_ray_tracing_maintenance1
        Size, BottomLevelPointers
    #endif
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma

#include "queryPool.inl"
