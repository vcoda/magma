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
#include "profiler.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/commandBuffer.h"
#include "../objects/queryPool.h"
#include "../objects/buffer.h"
#include "../allocator/allocator.h"

namespace magma
{
namespace aux
{
Profiler::Profiler(VkQueueFlags queueType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator):
    queueType(queueType),
    timestampPeriod(0.f),
    timestampMask(0ull),
    queryCount(0),
    frameIndex(0),
    hostQueryReset(false),
    debugUtils(false),
    debugMarker(false),
    useLabels(false),
    resetQueries(false),
    insideFrame(false)
{
    std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    const VkPhysicalDeviceProperties properties = physicalDevice->getProperties();
    const VkPhysicalDeviceLimits& limits = properties.limits;
    timestampPeriod = limits.timestampPeriod; // The number of nanoseconds it takes for a timestamp value to be incremented by 1
    // Try to find dedicated queue
    const std::vector<VkQueueFamilyProperties> queueFamilyProperties = physicalDevice->getQueueFamilyProperties();
    auto it = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
        [queueType](const VkQueueFamilyProperties& properties)
        {
            return (properties.queueFlags & queueType) == queueType;
        });
    if (queueFamilyProperties.end() == it)
    {   // Find any queue that has corresponding flag
        it = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
            [queueType](const VkQueueFamilyProperties& properties)
            {
                return (properties.queueFlags & queueType);
            });
    }
    if (queueFamilyProperties.end() == it)
        MAGMA_THROW("queue not found");
    // timestampValidBits is the unsigned integer count of meaningful bits in the timestamps.
    // The valid range for the count is 36 to 64 bits, or a value of 0, indicating no support for timestamps.
    if (it->timestampValidBits)
        timestampMask = (it->timestampValidBits < 64) ? (1ull << it->timestampValidBits) - 1 : std::numeric_limits<uint64_t>::max();
    else
        MAGMA_THROW("queue has no support for timestamps");
    constexpr uint32_t poolSize = 256;
    queryPool = std::make_shared<magma::TimestampQuery>(device, poolSize, std::move(allocator));
#ifdef VK_EXT_host_query_reset
    hostQueryReset = device->extensionEnabled(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME);
#endif
#ifdef VK_EXT_debug_utils
    std::shared_ptr<const Instance> instance = physicalDevice->getInstance();
    debugUtils = instance->extensionEnabled(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
#ifdef VK_EXT_debug_marker
    if (!debugUtils)
        debugMarker = device->extensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
#endif
}

bool Profiler::beginFrame(uint32_t frameIndex_)
{
    MAGMA_ASSERT(!insideFrame);
    MAGMA_ASSERT(stack.empty());
    if (insideFrame || !stack.empty())
        return false;
    if (queryCount > 0)
    {
        if (queryCount > queryPool->getQueryCount())
        {   // Grow
            queryPool = std::make_shared<magma::TimestampQuery>(queryPool->getDevice(),
                queryCount, queryPool->getHostAllocator());
            queryCount = 0;
        }
        else
        {
            if (!hostQueryReset)
                resetQueries = true;
            else
            {   // Reset from host
            #ifdef VK_EXT_host_query_reset
                queryPool->reset(0, queryCount);
            #endif
                queryCount = 0;
            }
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

void Profiler::beginSection(const char *name, uint32_t color, std::shared_ptr<CommandBuffer> cmdBuffer)
{
    MAGMA_ASSERT(insideFrame);
    MAGMA_ASSERT(strlen(name) > 0);
    if (resetQueries)
    {   // VK_EXT_host_query_reset not supported, use vkCmdResetQueryPool()
        cmdBuffer->resetQueryPool(queryPool, 0, queryCount);
        queryCount = 0;
        resetQueries = false;
    }
    if (useLabels)
    {
    #ifdef VK_EXT_debug_utils
        if (debugUtils)
            cmdBuffer->beginDebugLabel(name, color);
        else
    #endif // VK_EXT_debug_utils
        {
        #ifdef VK_EXT_debug_marker
            if (debugMarker)
                cmdBuffer->beginDebugMarker(name, color);
        #endif // VK_EXT_debug_marker
        }
    }
    const uint32_t beginQuery = queryCount % queryPool->getQueryCount();
    sections.emplace_back(name, frameIndex, beginQuery);
    stack.push(sections.back());
    // When vkCmdWriteTimestamp is submitted to a queue, it defines an execution dependency on commands
    // that were submitted before it. vkCmdWriteTimestamp latches the value of the timer when all
    // previous commands have completed executing as far as the specified pipeline stage.
    cmdBuffer->writeTimestamp(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, queryPool, stack.top().beginQuery);
    queryCount += 2;
}

void Profiler::endSection(std::shared_ptr<CommandBuffer> cmdBuffer)
{
    MAGMA_ASSERT(insideFrame);
    MAGMA_ASSERT(!stack.empty());
    if (!stack.empty())
    {
        cmdBuffer->writeTimestamp(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, queryPool, stack.top().beginQuery + 1);
        stack.pop();
    }
    if (useLabels)
    {
    #ifdef VK_EXT_debug_utils
        if (debugUtils)
            cmdBuffer->endDebugLabel();
        else
    #endif // VK_EXT_debug_utils
        {
        #ifdef VK_EXT_debug_marker
            if (debugMarker)
                cmdBuffer->endDebugMarker();
        #endif // VK_EXT_debug_marker
        }
    }
}

std::vector<Profiler::Timing> Profiler::getExecutionTimings(bool wait) const
{
    MAGMA_ASSERT(!insideFrame);
    std::vector<Timing> executionTimings;
    executionTimings.reserve(sections.size());
    const uint32_t count = std::min(queryCount, queryPool->getQueryCount());
    if (wait)
    {   // Block CPU until all query results will be ready
        const auto timestamps = queryPool->getResults<uint64_t>(0, count, wait);
        for (const auto& section: sections)
        {   // Bits outside the valid range are guaranteed to be zeros, but we use timestamp mask anyway
            const uint64_t beginTs = timestamps[section.beginQuery];
            const uint64_t endTs = timestamps[section.beginQuery + 1];
            const uint64_t start = beginTs & timestampMask;
            const uint64_t end = endTs & timestampMask;
            double time = double(end - start) * timestampPeriod; // nanoseconds
            executionTimings.emplace_back(section.name, section.frameIndex, time);
        }
    }  else
    {   // Do not stall, return only available results
        const auto timestamps = queryPool->getResultsWithAvailability<uint64_t>(0, count);
        for (const auto& section: sections)
        {
            const auto& beginTs = timestamps[section.beginQuery];
            const auto& endTs = timestamps[section.beginQuery + 1];
            if (!beginTs.availability || !endTs.availability)
                continue;
            const uint64_t start = beginTs.result & timestampMask;
            const uint64_t end = endTs.result & timestampMask;
            double time = double(end - start) * timestampPeriod; // nanoseconds
            executionTimings.emplace_back(section.name, section.frameIndex, time);
        }
    }
    return executionTimings;
}

void Profiler::copyExecutionTimings(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<Buffer> buffer,
    VkDeviceSize bufferOffset /* 0 */) const noexcept
{   // TODO: does vkCmdCopyQueryPoolResults() requires synchronizaition with earlier submitted timestamps commands?
    constexpr bool wait = true;
    const uint32_t count = std::min(queryCount, queryPool->getQueryCount());
    // vkCmdCopyQueryPoolResults must only be called outside of a render pass instance!
    cmdBuffer->copyQueryResults<uint64_t>(queryPool, buffer, wait, 0, count, bufferOffset);
}
} // namespace aux
} // namespace magma
