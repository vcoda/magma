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
#include <stack>
#include "base.h"

namespace magma
{
    class Device;
    class CommandBuffer;
    class TimestampQuery;
    class Buffer;
    class IAllocator;

    namespace aux
    {
        /* In-house GPU performance profiler.

           As GPU may perform out-of-order execution of commands,
           writing timestamps may introduce execution dependency
           on commands that were submitted inside profiling scope.
           A preferred usage is to profile large render blocks to
           avoid performance impact due to synchronizations inside
           command list. */

        class Profiler : public Base
        {
            struct Section
            {
                const char *name;
                uint32_t frameIndex;
                uint32_t beginQuery;
                Section(const char *name,
                    uint32_t frameIndex,
                    uint32_t beginQuery);
            };

        public:
            enum Queue
            {
                Graphics, Compute
            };

            struct Sample
            {
                const char *name;
                uint32_t frameIndex;
                double time; // In nanoseconds
                Sample(const char *name,
                    uint32_t frameIndex,
                    double time);
            };

            static void set(Profiler *profiler) noexcept;
            static Profiler *get(Queue queue) noexcept { return profilers[queue]; }
            VkQueueFlags getQueueType() const noexcept { return queueType; }
            bool setLabelUsage(bool enable) noexcept { useLabels = enable; }
            bool getLabelUsage() const noexcept { return useLabels; }
            uint32_t getQueryCount() const noexcept { return queryCount; }
            bool beginFrame(uint32_t frameIndex);
            bool endFrame();
            void beginSection(const char *name, uint32_t color, std::shared_ptr<CommandBuffer> cmdBuffer);
            void endSection(std::shared_ptr<CommandBuffer> cmdBuffer);
            std::vector<Sample> collectSamples(bool wait) const;
            void copyTimestamps(std::shared_ptr<CommandBuffer> cmdBuffer,
                std::shared_ptr<Buffer> buffer, VkDeviceSize bufferOffset = 0,
                bool hostRead = true) const noexcept;

        protected:
            explicit Profiler(VkQueueFlags queueType,
                std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator);
            uint32_t getResetQueryCount() const noexcept;

        private:
            static Profiler *profilers[2];
            const VkQueueFlags queueType;
            float timestampPeriod = 0.f;
            uint64_t timestampMask = 0ull;
            std::shared_ptr<TimestampQuery> queryPool;
            uint32_t queryCount = 0;
            uint32_t frameIndex = 0;
            std::vector<Section> sections;
            std::stack<Section> stack;
            bool hostQueryReset = false;
            bool debugUtils = false;
            bool debugMarker = false;
            bool useLabels = false;
            bool resetQueries = false;
            bool insideFrame = false;
        };

        /* Performance profiler for graphics queue. */

        class GraphicsProfiler : public Profiler
        {
        public:
            explicit GraphicsProfiler(std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator = nullptr):
                Profiler(VK_QUEUE_GRAPHICS_BIT, std::move(device), std::move(allocator)) {}
        };

        /* Performance profiler for compute queue. */

        class ComputeProfiler : public Profiler
        {
        public:
            explicit ComputeProfiler(std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator = nullptr):
                Profiler(VK_QUEUE_COMPUTE_BIT, std::move(device), std::move(allocator)) {}
        };
    } // namespace aux
} // namespace magma

#include "profiler.inl"
#include "scopedProfile.h"
