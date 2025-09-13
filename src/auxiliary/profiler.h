/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

namespace magma
{
    class Device;
    class CommandBuffer;
    class TimestampQuery;
    class Buffer;
    class IAllocator;

    namespace aux
    {
        /* Little GPU performance profiler.
           As GPU may perform out-of-order execution of commands,
           writing timestamps may introduce execution dependency
           on commands that were submitted inside profiling scope.
           A preferred usage is to profile large render blocks
           to avoid performance impact due to synchronizations
           inside command list. */

        class Profiler : public IClass
        {
        public:
            enum Queue : uint8_t;
            struct Sample;
            static void set(Profiler *profiler) noexcept;
            static Profiler *get(Queue queue) noexcept { return profilers[queue]; }
            VkQueueFlags getQueueType() const noexcept { return queueType; }
            void setUseLabels(bool enable) noexcept { useLabels = enable; }
            bool getUseLabels() const noexcept { return useLabels; }
            uint32_t getQueryCount() const noexcept { return queryCount; }
            bool beginFrame(lent_ptr<CommandBuffer> cmdBuffer,
                uint32_t frameIndex);
            bool endFrame();
            void beginSection(lent_ptr<CommandBuffer> cmdBuffer,
                const char *name, uint32_t color);
            void endSection(lent_ptr<CommandBuffer> cmdBuffer);
            std::vector<Sample> collectSamples(bool wait) const;
            void copyTimestamps(lent_ptr<CommandBuffer> cmdBuffer,
                lent_ptr<Buffer> buffer, VkDeviceSize bufferOffset = 0,
                bool hostRead = true) const noexcept;

        protected:
            Profiler(VkQueueFlags queueType,
                std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator,
                uint32_t maxTimestampQueries);
            uint32_t getResetQueryCount() const noexcept;

        private:
            struct Section;
            static Profiler *profilers[2];
            const VkQueueFlags queueType;
            float timestampPeriod = 0.f;
            uint64_t timestampMask = 0ull;
            std::unique_ptr<TimestampQuery> queryPool;
            uint32_t queryCount = 0;
            uint32_t frameIndex = 0;
            std::forward_list<Section> sections;
            std::stack<const Section *> stack;
            bool hostQueryReset = false;
            bool useLabels = false;
            bool insideFrame = false;
        };

        enum Profiler::Queue : uint8_t
        {
            Graphics, Compute
        };

        struct Profiler::Sample
        {
            const char *name;
            uint32_t frameIndex;
            double time; // In nanoseconds
            Sample(const char *, uint32_t, double) noexcept;
        };

        struct Profiler::Section
        {
            const char *name;
            uint32_t frameIndex;
            uint32_t beginQuery;
            Section(const char *, uint32_t, uint32_t) noexcept;
        };

        /* Performance profiler for graphics queue. */

        class GraphicsProfiler : public Profiler
        {
        public:
            explicit GraphicsProfiler(std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator = nullptr,
                uint32_t maxTimestampQueries = 256):
                Profiler(VK_QUEUE_GRAPHICS_BIT, std::move(device), std::move(allocator), maxTimestampQueries)
            {}
        };

        /* Performance profiler for compute queue. */

        class ComputeProfiler : public Profiler
        {
        public:
            explicit ComputeProfiler(std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator = nullptr,
                uint32_t maxTimestampQueries = 256):
                Profiler(VK_QUEUE_COMPUTE_BIT, std::move(device), std::move(allocator), maxTimestampQueries)
            {}
        };

        /* GPU profiling block. */

        class ScopedProfile final : NonCopyable
        {
        public:
            explicit ScopedProfile(const char *name,
                lent_ptr<CommandBuffer> cmdBuffer,
                uint32_t color = 0xFFFFFFFF);
            ~ScopedProfile();

        private:
            CommandBuffer *cmdBuffer;
            Profiler *profiler;
        };
    } // namespace aux
} // namespace magma

#include "profiler.inl"

#define MAGMA_SCOPED_PROFILE(name, cmdBuffer) magma::aux::ScopedProfile MAGMA_CONCAT(_magmaProfile, MAGMA_COUNTER)(name, cmdBuffer);

#ifdef MAGMA_DEBUG
    #define MAGMA_PROFILE(cmdBuffer) MAGMA_SCOPED_PROFILE(__FUNCTION__, cmdBuffer)
    #define MAGMA_PROFILE_NAMED(name, cmdBuffer) MAGMA_SCOPED_PROFILE(name, cmdBuffer)
#else
    #define MAGMA_PROFILE(cmdBuffer)
    #define MAGMA_PROFILE_NAMED(name, cmdBuffer)
#endif // MAGMA_DEBUG

#define MAGMA_PROFILE_BEGIN(cmdBuffer) {\
    MAGMA_PROFILE(cmdBuffer)
#define MAGMA_PROFILE_BEGIN_NAMED(name, cmdBuffer) {\
    MAGMA_PROFILE_NAMED(name, cmdBuffer)
#define MAGMA_PROFILE_END }
