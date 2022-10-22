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
    class IAllocator;

    namespace aux
    {
        /* Auxiliary performance profiler. */

        class Profiler : public Base
        {
        protected:
            explicit Profiler(VkQueueFlags queueType,
                std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator);

        public:
            struct Timing
            {
                const char *name;
                double time;
                Timing(const char *name, double time) noexcept:
                    name(name), time(time) {}
            };

            VkQueueFlags getQueueType() const noexcept { return queueType; }
            uint32_t getQueryCount() const noexcept { return queryCount; }
            bool beginFrame();
            bool endFrame();
            void beginSection(const char *name,
                uint32_t color,
                std::shared_ptr<CommandBuffer> cmdBuffer);
            void endSection(std::shared_ptr<CommandBuffer> cmdBuffer);
            std::vector<Timing> getExecutionTimings(bool dontBlockCpu);

        private:
            struct Section
            {
                const char *name;
                uint32_t beginQuery;
                Section(const char *name, uint32_t beginQuery) noexcept:
                    name(name), beginQuery(beginQuery) {}
            };

            const VkQueueFlags queueType;
            float timestampPeriod;
            uint64_t timestampMask;
            std::shared_ptr<TimestampQuery> queryPool;
            uint32_t queryCount;
            std::vector<Section> sections;
            std::stack<Section> stack;
            bool hostQueryReset;
            bool debugUtils;
            bool debugMarker;
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
