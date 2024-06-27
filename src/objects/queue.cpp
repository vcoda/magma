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
#include "queue.h"
#include "device.h"
#include "commandBuffer.h"
#include "semaphore.h"
#include "timelineSemaphore.h"
#include "fence.h"
#include "swapchain.h"
#include "../platform/d3dExternalSemaphore.h"
#include "../helpers/stackArray.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Queue::Queue(VkQueue handle, VkQueueFlagBits flags, uint32_t familyIndex, uint32_t index) noexcept:
    Dispatchable(VK_OBJECT_TYPE_QUEUE, handle),
    flags(flags),
    familyIndex(familyIndex),
    index(index)
{
#ifdef VK_EXT_swapchain_maintenance1
    presentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
#endif
}

void Queue::submit(std::shared_ptr<CommandBuffer> cmdBuffer,
    VkPipelineStageFlags waitDstStageMask /* 0 */,
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */,
    std::shared_ptr<const Semaphore> signalSemaphore /* nullptr */,
    std::shared_ptr<const Fence> fence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    MAGMA_ASSERT(cmdBuffer->primary());
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = extendedInfo.headNode();
    submitInfo.waitSemaphoreCount = waitSemaphore ? 1 : 0;
    submitInfo.pWaitSemaphores = waitSemaphore ? waitSemaphore->getHandleAddress() : nullptr;
    submitInfo.pWaitDstStageMask = &waitDstStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = cmdBuffer->getHandleAddress();
    submitInfo.signalSemaphoreCount = signalSemaphore ? 1 : 0;
    submitInfo.pSignalSemaphores = signalSemaphore ? signalSemaphore->getHandleAddress() : nullptr;
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, MAGMA_OPTIONAL_HANDLE(fence));
    MAGMA_HANDLE_RESULT(result, "queue submission failed");
    cmdBuffer->finishedQueueSubmission();
}

void Queue::submit(const std::initializer_list<std::shared_ptr<CommandBuffer>> cmdBuffers,
    const std::initializer_list<VkPipelineStageFlags> waitDstStageMask /* void */,
    const std::initializer_list<std::shared_ptr<const Semaphore>> waitSemaphores /* void */,
    const std::initializer_list<std::shared_ptr<const Semaphore>> signalSemaphores /* void */,
    std::shared_ptr<const Fence> fence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    MAGMA_ASSERT(cmdBuffers.size());
    if (0 == cmdBuffers.size())
        return;
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCmdBuffers, cmdBuffers.size());
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedWaitSemaphores, waitSemaphores.size());
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedSignalSemaphores, signalSemaphores.size());
    // https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkSubmitInfo.html
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = extendedInfo.headNode();
    if (0 == waitSemaphores.size())
    {
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = nullptr;
    }
    else
    {   // Dereference valid wait semaphores
        for (auto const& semaphore: waitSemaphores)
        {
            if (semaphore)
                dereferencedWaitSemaphores.put(*semaphore);
        }
        submitInfo.waitSemaphoreCount = dereferencedWaitSemaphores.count();
        submitInfo.pWaitSemaphores = dereferencedWaitSemaphores;
        submitInfo.pWaitDstStageMask = waitDstStageMask.begin();
    }
    // Dereference command buffers
    for (auto const& cmdBuffer: cmdBuffers)
    {
        if (cmdBuffer)
        {
            MAGMA_ASSERT(cmdBuffer->primary());
            dereferencedCmdBuffers.put(*cmdBuffer);
        }
    }
    submitInfo.commandBufferCount = dereferencedCmdBuffers.count();
    submitInfo.pCommandBuffers = dereferencedCmdBuffers;
    if (0 == signalSemaphores.size())
    {
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = nullptr;
    }
    else
    {   // Dereference valid semaphores to be signaled
        for (auto const& semaphore: signalSemaphores)
        {
            if (semaphore)
                dereferencedSignalSemaphores.put(*semaphore);
        }
        submitInfo.signalSemaphoreCount = dereferencedSignalSemaphores.count();
        submitInfo.pSignalSemaphores = dereferencedSignalSemaphores;
    }
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, MAGMA_OPTIONAL_HANDLE(fence));
    MAGMA_HANDLE_RESULT(result, "queue submission failed");
    for (auto& cmdBuffer: cmdBuffers)
    {   // Change state of the command buffer
        cmdBuffer->finishedQueueSubmission();
    }
}

#ifdef VK_KHR_timeline_semaphore
void Queue::submit(std::shared_ptr<const TimelineSemaphore> semaphore, uint64_t waitValue, uint64_t signalValue)
{   // https://www.khronos.org/blog/vulkan-timeline-semaphores
    VkSubmitInfo submitInfo;
    VkTimelineSemaphoreSubmitInfoKHR submitTimelineInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = &submitTimelineInfo;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = semaphore->getHandleAddress();
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 0;
    submitInfo.pCommandBuffers = nullptr;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = semaphore->getHandleAddress();
    submitTimelineInfo.sType = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO_KHR;
    submitTimelineInfo.pNext = nullptr;
    submitTimelineInfo.waitSemaphoreValueCount = 1;
    submitTimelineInfo.pWaitSemaphoreValues = &waitValue;
    submitTimelineInfo.signalSemaphoreValueCount = 1;
    submitTimelineInfo.pSignalSemaphoreValues = &signalValue;
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, VK_NULL_HANDLE);
    MAGMA_HANDLE_RESULT(result, "queue submission failed");
}
#endif // VK_KHR_timeline_semaphore

#ifdef VK_KHR_external_semaphore_win32
void Queue::submit(std::shared_ptr<const D3d12ExternalSemaphore> semaphore, uint64_t waitValue, uint64_t signalValue)
{
    VkSubmitInfo submitInfo;
    VkD3D12FenceSubmitInfoKHR submitFenceInfoD3d12;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = &submitFenceInfoD3d12;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = semaphore->getHandleAddress();
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 0;
    submitInfo.pCommandBuffers = nullptr;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = semaphore->getHandleAddress();
    submitFenceInfoD3d12.sType = VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR;
    submitFenceInfoD3d12.pNext = nullptr;
    submitFenceInfoD3d12.waitSemaphoreValuesCount = 1;
    submitFenceInfoD3d12.pWaitSemaphoreValues = &waitValue;
    submitFenceInfoD3d12.signalSemaphoreValuesCount = 1;
    submitFenceInfoD3d12.pSignalSemaphoreValues = &signalValue;
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, VK_NULL_HANDLE);
    MAGMA_HANDLE_RESULT(result, "queue submission with Direct3D fence failed");
}

#ifdef VK_KHR_timeline_semaphore
void Queue::submit(std::shared_ptr<const D3d12ExternalTimelineSemaphore> semaphore, uint64_t waitValue, uint64_t signalValue)
{
    VkSubmitInfo submitInfo;
    VkD3D12FenceSubmitInfoKHR submitFenceInfoD3d12;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = &submitFenceInfoD3d12;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = semaphore->getHandleAddress();
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 0;
    submitInfo.pCommandBuffers = nullptr;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = semaphore->getHandleAddress();
    submitFenceInfoD3d12.sType = VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR;
    submitFenceInfoD3d12.pNext = nullptr;;
    submitFenceInfoD3d12.waitSemaphoreValuesCount = 1;
    submitFenceInfoD3d12.pWaitSemaphoreValues = &waitValue;
    submitFenceInfoD3d12.signalSemaphoreValuesCount = 1;
    submitFenceInfoD3d12.pSignalSemaphoreValues = &signalValue;
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, VK_NULL_HANDLE);
    MAGMA_HANDLE_RESULT(result, "queue submission with Direct3D fence failed");
}
#endif // VK_KHR_timeline_semaphore
#endif // VK_KHR_external_semaphore_win32

#ifdef VK_KHR_device_group
void Queue::submitDeviceGroup(const std::initializer_list<std::shared_ptr<CommandBuffer>> cmdBuffers,
    const std::initializer_list<uint32_t> cmdBufferDeviceMasks /* void */,
    const std::initializer_list<VkPipelineStageFlags> waitDstStageMask /* void */,
    const std::initializer_list<std::shared_ptr<const Semaphore>> waitSemaphores /* void */,
    const std::initializer_list<uint32_t> waitSemaphoreDeviceIndices /* void */,
    const std::initializer_list<std::shared_ptr<const Semaphore>> signalSemaphores /* void */,
    const std::initializer_list<uint32_t> signalSemaphoreDeviceIndices /* void */,
    std::shared_ptr<const Fence> fence /* nullptr */)
{
    MAGMA_ASSERT_FOR_EACH(cmdBuffers, cmdBuffer, cmdBuffer->primary());
    VkDeviceGroupSubmitInfo deviceGroupSubmitInfo;
    deviceGroupSubmitInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO;
    deviceGroupSubmitInfo.pNext = nullptr;
    deviceGroupSubmitInfo.waitSemaphoreCount = MAGMA_COUNT(waitSemaphoreDeviceIndices);
    deviceGroupSubmitInfo.pWaitSemaphoreDeviceIndices = waitSemaphoreDeviceIndices.begin();
    deviceGroupSubmitInfo.commandBufferCount = MAGMA_COUNT(cmdBufferDeviceMasks);
    deviceGroupSubmitInfo.pCommandBufferDeviceMasks = cmdBufferDeviceMasks.begin();
    deviceGroupSubmitInfo.signalSemaphoreCount = MAGMA_COUNT(signalSemaphoreDeviceIndices);
    deviceGroupSubmitInfo.pSignalSemaphoreDeviceIndices = signalSemaphoreDeviceIndices.begin();
    submit(cmdBuffers, waitDstStageMask, waitSemaphores, signalSemaphores, std::move(fence), StructureChain(deviceGroupSubmitInfo));
}
#endif // VK_KHR_device_group

void Queue::waitIdle()
{
    const VkResult result = vkQueueWaitIdle(handle);
    MAGMA_HANDLE_RESULT(result, "failed to wait for a queue to become idle");
}

void Queue::present(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */,
    std::shared_ptr<Fence> presentFence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = extendedInfo.headNode();
    if (waitSemaphore)
    {
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = waitSemaphore->getHandleAddress();
    }
    else
    {
        presentInfo.waitSemaphoreCount = 0;
        presentInfo.pWaitSemaphores = nullptr;
    }
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchain->getHandleAddress();
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
#ifdef VK_EXT_swapchain_maintenance1
    VkSwapchainPresentFenceInfoEXT presentFenceInfo;
    VkSwapchainPresentModeInfoEXT presentModeInfo;
    if (swapchain->getDevice()->extensionEnabled(VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME))
    {
        if (presentFence)
        {
            presentFenceInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_FENCE_INFO_EXT;
            presentFenceInfo.pNext = nullptr;
            presentFenceInfo.swapchainCount = presentInfo.swapchainCount;
            presentFenceInfo.pFences = presentFence->getHandleAddress();
            linkNode(presentInfo, presentFenceInfo);
        }
        if (presentMode != VK_PRESENT_MODE_MAX_ENUM_KHR)
        {
            presentModeInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_MODE_INFO_EXT;
            presentModeInfo.pNext = nullptr;
            presentModeInfo.swapchainCount = presentInfo.swapchainCount;
            presentModeInfo.pPresentModes = &presentMode;
            linkNode(presentInfo, presentModeInfo);
        }
    }
#else
    MAGMA_UNUSED(presentFence);
#endif // VK_EXT_swapchain_maintenance1
    const VkResult result = vkQueuePresentKHR(handle, &presentInfo);
#ifndef MAGMA_NO_EXCEPTIONS
    switch (result)
    {
#ifdef VK_KHR_swapchain
    case VK_ERROR_OUT_OF_DATE_KHR:
        throw exception::OutOfDate("queue present failed");
#endif
#ifdef VK_KHR_surface
    case VK_ERROR_SURFACE_LOST_KHR:
        throw exception::SurfaceLost("queue present failed");
#endif
#ifdef VK_EXT_full_screen_exclusive
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        throw exception::FullScreenExclusiveModeLost("queue present failed");
#endif
    }
#endif // !MAGMA_NO_EXCEPTIONS
    MAGMA_HANDLE_RESULT(result, "queue present failed");
}

#ifdef VK_KHR_display_swapchain
void Queue::presentDisplay(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
    const VkRect2D& srcRect, const VkRect2D& dstRect, bool persistent,
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */,
    std::shared_ptr<Fence> presentFence /* nullptr */)
{
    VkDisplayPresentInfoKHR displayPresentInfo;
    displayPresentInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR;
    displayPresentInfo.pNext = nullptr;
    displayPresentInfo.srcRect = srcRect;
    displayPresentInfo.dstRect = dstRect;
    displayPresentInfo.persistent = MAGMA_BOOLEAN(persistent);
    present(std::move(swapchain), imageIndex, std::move(waitSemaphore), std::move(presentFence),
        StructureChain(displayPresentInfo));
}
#endif // VK_KHR_display_swapchain

#ifdef VK_NV_device_diagnostic_checkpoints
std::vector<VkCheckpointDataNV> Queue::getCheckpoints(std::shared_ptr<const Device> device) const
{
    auto getNativeDevice = [&device]() -> VkDevice { return device->getHandle(); };
    std::vector<VkCheckpointDataNV> checkpoints;
    uint32_t checkpointDataCount = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetQueueCheckpointDataNV, VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
    vkGetQueueCheckpointDataNV(handle, &checkpointDataCount, nullptr);
    if (checkpointDataCount)
    {
        checkpoints.resize(checkpointDataCount, {VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV});
        vkGetQueueCheckpointDataNV(handle, &checkpointDataCount, checkpoints.data());
    }
    return checkpoints;
}
#endif // VK_NV_device_diagnostic_checkpoints
} // namespace magma
