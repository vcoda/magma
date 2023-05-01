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
        /* Auxiliary GPU performance profiler.

           As GPU may perform out-of-order execution of commands, writing timestamps
           may introduce execution dependency on commands that were submitted inside
           profiling scope. A preferred usage is to profile large render blocks
           to avoid performance impact due to synchronizations inside command list. */

        class Profiler : public Base
        {
        public:
            enum Queue
            {
                Graphics, Compute
            };

            struct Timing
            {
                const char *name;
                const uint32_t frameIndex;
                const double time;
                Timing(const char *name, uint32_t frameIndex, double time) noexcept:
                    name(name), frameIndex(frameIndex), time(time) {}
            };

            static void set(Profiler *profiler) noexcept;
            static Profiler *get(Queue queue) noexcept { return profilers[queue]; }
            VkQueueFlags getQueueType() const noexcept { return queueType; }
            bool setLabelUsage(bool enable) noexcept { useLabels = enable; }
            bool getLabelUsage() const noexcept { return useLabels; }
            uint32_t getQueryCount() const noexcept { return queryCount; }
            bool beginFrame(uint32_t frameIndex);
            bool endFrame();
            void beginSection(const char *name,
                uint32_t color,
                std::shared_ptr<CommandBuffer> cmdBuffer);
            void endSection(std::shared_ptr<CommandBuffer> cmdBuffer);
            std::vector<Timing> getExecutionTimings(bool wait) const;
            void copyExecutionTimings(std::shared_ptr<CommandBuffer> cmdBuffer,
                std::shared_ptr<Buffer> buffer,
                VkDeviceSize bufferOffset = 0,
                bool hostRead = true) const noexcept;

        protected:
            explicit Profiler(VkQueueFlags queueType,
                std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator);

        private:
            struct Section
            {
                const char *name;
                const uint32_t frameIndex;
                const uint32_t beginQuery;
                Section(const char *name, uint32_t frameIndex, uint32_t beginQuery) noexcept:
                    name(name), frameIndex(frameIndex), beginQuery(beginQuery) {}
            };

            static Profiler *profilers[2];
            const VkQueueFlags queueType;
            float timestampPeriod;
            uint64_t timestampMask;
            std::shared_ptr<TimestampQuery> queryPool;
            uint32_t queryCount;
            uint32_t frameIndex;
            std::vector<Section> sections;
            std::stack<Section> stack;
            bool hostQueryReset;
            bool debugUtils;
            bool debugMarker;
            bool useLabels;
            bool resetQueries;
            bool insideFrame;
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

#include "scopedProfile.h"
