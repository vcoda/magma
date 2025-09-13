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
#include "pch.h"
#pragma hdrstop
#include "profiler.h"
#include "../objects/instance.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/commandBuffer.h"
#include "../objects/queryPool.h"
#include "../objects/buffer.h"
#include "../allocator/allocator.h"

namespace magma::aux
{
Profiler *Profiler::profilers[2];

Profiler::Profiler(VkQueueFlags queueType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator, uint32_t maxTimestampQueries):
    queueType(queueType)
{
    const std::shared_ptr<PhysicalDevice>& physicalDevice = device->getPhysicalDevice();
    const VkPhysicalDeviceProperties properties = physicalDevice->getProperties();
    const VkPhysicalDeviceLimits& limits = properties.limits;
    timestampPeriod = limits.timestampPeriod; // The number of nanoseconds it takes for a timestamp value to be incremented by 1
    // Try to find dedicated queue
    const std::vector<VkQueueFamilyProperties> queueFamilyProperties = physicalDevice->getQueueFamilyProperties();
    auto it = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
        [queueType](const VkQueueFamilyProperties& properties) {
            return (properties.queueFlags & queueType) == queueType;
        });
    if (queueFamilyProperties.end() == it)
    {   // Find any queue that has corresponding flag
        it = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
            [queueType](const VkQueueFamilyProperties& properties) {
                return (properties.queueFlags & queueType);
            });
    }
    if (queueFamilyProperties.end() == it)
        MAGMA_ERROR("queue not found");
    // timestampValidBits is the unsigned integer count of meaningful bits in the timestamps.
    // The valid range for the count is 36 to 64 bits, or a value of 0, indicating no support for timestamps.
    if (it->timestampValidBits)
        timestampMask = (it->timestampValidBits < 64) ? (1ull << it->timestampValidBits) - 1 : std::numeric_limits<uint64_t>::max();
    else
        MAGMA_ERROR("queue has no support for timestamps");
    queryPool = std::make_unique<TimestampQuery>(device, maxTimestampQueries, std::move(allocator));
#ifdef VK_EXT_host_query_reset
    hostQueryReset = device->extensionEnabled(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME);
#endif // VK_EXT_host_query_reset
}

void Profiler::set(Profiler *profiler) noexcept
{
    if (VK_QUEUE_GRAPHICS_BIT == profiler->getQueueType())
        profilers[Graphics] = profiler;
    else // VK_QUEUE_COMPUTE_BIT
        profilers[Compute] = profiler;
}

bool Profiler::beginFrame(lent_ptr<CommandBuffer> cmdBuffer, uint32_t frameIndex_)
{
    MAGMA_ASSERT(!insideFrame);
    MAGMA_ASSERT(stack.empty());
    if (insideFrame || !stack.empty())
        return false;
    if ((queryCount > 0) || (0 == frameIndex_))
    {
        if (queryCount > queryPool->getQueryCount())
        {   // Growable pool
            queryPool = std::make_unique<TimestampQuery>(queryPool->getDevice(),
                queryCount, queryPool->getHostAllocator());
            queryCount = 0;
        }
        if (!hostQueryReset)
        {   // VK_EXT_host_query_reset not supported, use vkCmdResetQueryPool()
            // vkCmdResetQueryPool command must only be called outside of a render pass instance!
            MAGMA_ASSERT(!cmdBuffer->insideRenderPass());
            cmdBuffer->resetQueryPool(queryPool, 0, getResetQueryCount());
            queryCount = 0;
        }
        else
        {   // Reset from host
        #ifdef VK_EXT_host_query_reset
            queryPool->reset(0, getResetQueryCount());
            queryCount = 0;
        #endif
        }
        sections.clear();
    }
    frameIndex = frameIndex_;
    insideFrame = true;
    return true;
}

bool Profiler::endFrame()
{
    MAGMA_ASSERT(insideFrame);
    MAGMA_ASSERT(stack.empty());
    if (!insideFrame || !stack.empty())
        return false;
    insideFrame = false;
    return true;
}

void Profiler::beginSection(lent_ptr<CommandBuffer> cmdBuffer, const char *name, uint32_t color)
{
    MAGMA_ASSERT(insideFrame);
    MAGMA_ASSERT(strlen(name));
    if (useLabels)
    {
        float r, g, b, a;
        MAGMA_DWORD_TO_FLOAT_RGBA(color, r, g, b, a);
    #if defined(VK_EXT_debug_utils)
        cmdBuffer->beginDebugLabel(name, r, g, b, a);
    #elif defined(VK_EXT_debug_marker)
        cmdBuffer->beginDebugMarker(name, r, g, b, a);
    #else
        MAGMA_UNUSED(name);
        MAGMA_UNUSED(r);
        MAGMA_UNUSED(g);
        MAGMA_UNUSED(b);
        MAGMA_UNUSED(a);
    #endif // !defined(VK_EXT_debug_marker)
    }
    const uint32_t beginQuery = queryCount % queryPool->getQueryCount();
    sections.emplace_front(name, frameIndex, beginQuery);
    stack.push(&sections.front());
    // When vkCmdWriteTimestamp is submitted to a queue, it defines an execution dependency on commands
    // that were submitted before it. vkCmdWriteTimestamp latches the value of the timer when all
    // previous commands have completed executing as far as the specified pipeline stage.
    cmdBuffer->writeTimestamp(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, stack.top()->beginQuery);
    queryCount += 2;
}

void Profiler::endSection(lent_ptr<CommandBuffer> cmdBuffer)
{
    MAGMA_ASSERT(insideFrame);
    MAGMA_ASSERT(!stack.empty());
    if (!stack.empty())
    {
        cmdBuffer->writeTimestamp(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, queryPool, stack.top()->beginQuery + 1);
        stack.pop();
    }
    if (useLabels)
    {
    #if defined(VK_EXT_debug_utils)
        cmdBuffer->endDebugLabel();
    #elif defined(VK_EXT_debug_marker)
        cmdBuffer->endDebugMarker();
    #endif
    }
}

std::vector<Profiler::Sample> Profiler::collectSamples(bool wait) const
{
    MAGMA_ASSERT(!insideFrame);
    std::vector<Sample> samples;
    const uint32_t count = std::min(queryCount, queryPool->getQueryCount());
    if (wait)
    {   // Block CPU until all query results will be ready
        const auto timestamps = queryPool->getResults<uint64_t>(0, count, wait);
        for (auto const& section: sections)
        {   // Bits outside the valid range are guaranteed to be zeros, but we use timestamp mask anyway
            const uint64_t beginTs = timestamps[section.beginQuery];
            const uint64_t endTs = timestamps[section.beginQuery + 1];
            const uint64_t start = beginTs & timestampMask;
            const uint64_t end = endTs & timestampMask;
            double time = double(end - start) * timestampPeriod; // nanoseconds
            samples.emplace_back(section.name, section.frameIndex, time);
        }
    }
    else
    {   // Do not stall, return only available results
        const auto timestamps = queryPool->getResultsWithAvailability<uint64_t>(0, count);
        for (auto const& section: sections)
        {
            auto const& beginTs = timestamps[section.beginQuery];
            auto const& endTs = timestamps[section.beginQuery + 1];
            if (!beginTs.availability || !endTs.availability)
                continue;
            const uint64_t start = beginTs.result & timestampMask;
            const uint64_t end = endTs.result & timestampMask;
            double time = double(end - start) * timestampPeriod; // nanoseconds
            samples.emplace_back(section.name, section.frameIndex, time);
        }
    }
    return samples;
}

void Profiler::copyTimestamps(lent_ptr<CommandBuffer> cmdBuffer, lent_ptr<Buffer> buffer,
    VkDeviceSize bufferOffset /* 0 */, bool hostRead /* true */) const noexcept
{
    constexpr bool wait = true;
    const uint32_t count = std::min(queryCount, queryPool->getQueryCount());
    // vkCmdCopyQueryPoolResults command must only be called outside of a render pass instance!
    MAGMA_ASSERT(!cmdBuffer->insideRenderPass());
    const BufferMemoryBarrier transferWriteHostReadBarrier(buffer.get(), barrier::buffer::transferWriteHostRead);
    cmdBuffer->copyQueryResults<uint64_t>(queryPool, std::move(buffer), wait, 0, count, bufferOffset);
    if (hostRead)
    {   // vkCmdCopyQueryPoolResults is considered to be a transfer operation, and its writes to buffer memory
        // must be synchronized using VK_PIPELINE_STAGE_TRANSFER_BIT and VK_ACCESS_TRANSFER_WRITE_BIT before using the results.
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT, transferWriteHostReadBarrier);
    }
}

uint32_t Profiler::getResetQueryCount() const noexcept
{
    if (queryCount > 0)
        return queryCount; // Reset only those queries that have been used in the previos frame
    // Reset all queries before first use
    return queryPool->getQueryCount();
}
} // namespace magma::aux
