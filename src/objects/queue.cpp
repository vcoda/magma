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
#include "queue.h"
#include "device.h"
#include "buffer.h"
#include "image.h"
#include "commandBuffer.h"
#include "semaphore.h"
#include "timelineSemaphore.h"
#include "fence.h"
#include "swapchain.h"
#include "../platform/d3dExternalSemaphore.h"
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

void Queue::bindSparse(lent_ptr<const Buffer> buffer, const std::vector<VkSparseMemoryBind>& bufferBinds,
    lent_ptr<const Image> imageOpaque, const std::vector<VkSparseMemoryBind>& imageOpaqueBinds,
    lent_ptr<const Image> image, const std::vector<VkSparseImageMemoryBind>& imageBinds,
    lent_ptr<const Semaphore> waitSemaphore /* nullptr */,
    lent_ptr<Semaphore> signalSemaphore /* nullptr */,
    lent_ptr<Fence> fence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    VkSparseBufferMemoryBindInfo bufferMemoryBindInfo;
    bufferMemoryBindInfo.buffer = MAGMA_OPTIONAL_HANDLE(buffer);
    bufferMemoryBindInfo.bindCount = core::countof(bufferBinds);
    bufferMemoryBindInfo.pBinds = bufferBinds.data();
    VkSparseImageOpaqueMemoryBindInfo imageOpaqueMemoryBindInfo;
    imageOpaqueMemoryBindInfo.image = MAGMA_OPTIONAL_HANDLE(imageOpaque);
    imageOpaqueMemoryBindInfo.bindCount = core::countof(imageOpaqueBinds);
    imageOpaqueMemoryBindInfo.pBinds = imageOpaqueBinds.data();
    VkSparseImageMemoryBindInfo imageMemoryBindInfo;
    imageMemoryBindInfo.image = MAGMA_OPTIONAL_HANDLE(image);
    imageMemoryBindInfo.bindCount = core::countof(imageBinds);
    imageMemoryBindInfo.pBinds = imageBinds.data();
    VkBindSparseInfo bindSparseInfo = {};
    bindSparseInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
    bindSparseInfo.pNext = extendedInfo.headNode();
    if (waitSemaphore)
    {
        bindSparseInfo.waitSemaphoreCount = 1;
        bindSparseInfo.pWaitSemaphores = waitSemaphore->getHandleAddress();
    }
    if (buffer)
    {
        bindSparseInfo.bufferBindCount = 1;
        bindSparseInfo.pBufferBinds = &bufferMemoryBindInfo;
    }
    if (imageOpaque)
    {
        bindSparseInfo.imageOpaqueBindCount = 1;
        bindSparseInfo.pImageOpaqueBinds = &imageOpaqueMemoryBindInfo;
    }
    if (image)
    {
        bindSparseInfo.imageBindCount = 1;
        bindSparseInfo.pImageBinds = &imageMemoryBindInfo;
    }
    if (signalSemaphore)
    {
        bindSparseInfo.signalSemaphoreCount = 1;
        bindSparseInfo.pSignalSemaphores = signalSemaphore->getHandleAddress();
    }
    const VkResult result = vkQueueBindSparse(handle, 1, &bindSparseInfo, MAGMA_OPTIONAL_HANDLE(fence));
    MAGMA_HANDLE_RESULT(result, "failed to submit sparse binding operation");
}

void Queue::bindSparse(const std::vector<VkSparseBufferMemoryBindInfo>& bufferBinds,
    const std::vector<VkSparseImageOpaqueMemoryBindInfo>& imageOpaqueBinds,
    const std::vector<VkSparseImageMemoryBindInfo>& imageBinds,
    const std::initializer_list<lent_ptr<const Semaphore>>& waitSemaphores /* void */,
    const std::initializer_list<lent_ptr<Semaphore>>& signalSemaphores /* void */,
    lent_ptr<Fence> fence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    MAGMA_VLA(VkSemaphore, dereferencedWaitSemaphores, waitSemaphores.size());
    MAGMA_VLA(VkSemaphore, dereferencedSignalSemaphores, signalSemaphores.size());
    VkBindSparseInfo bindSparseInfo = {};
    bindSparseInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
    bindSparseInfo.pNext = extendedInfo.headNode();
    if (waitSemaphores.size() > 0)
    {
        for (auto const& semaphore: waitSemaphores)
        {
            if (semaphore)
                dereferencedWaitSemaphores.put(*semaphore);
        }
        bindSparseInfo.waitSemaphoreCount = dereferencedWaitSemaphores.count();
        bindSparseInfo.pWaitSemaphores = dereferencedWaitSemaphores;
    }
    bindSparseInfo.bufferBindCount = core::countof(bufferBinds);
    bindSparseInfo.pBufferBinds = bufferBinds.data();
    bindSparseInfo.imageOpaqueBindCount = core::countof(imageOpaqueBinds);
    bindSparseInfo.pImageOpaqueBinds = imageOpaqueBinds.data();
    bindSparseInfo.imageBindCount = core::countof(imageBinds);
    bindSparseInfo.pImageBinds = imageBinds.data();
    if (signalSemaphores.size() > 0)
    {
        for (auto const& semaphore: signalSemaphores)
        {
            if (semaphore)
                dereferencedSignalSemaphores.put(*semaphore);
        }
        bindSparseInfo.signalSemaphoreCount = dereferencedSignalSemaphores.count();
        bindSparseInfo.pSignalSemaphores = dereferencedSignalSemaphores;
    }
    const VkResult result = vkQueueBindSparse(handle, 1, &bindSparseInfo, MAGMA_OPTIONAL_HANDLE(fence));
    MAGMA_HANDLE_RESULT(result, "failed to submit sparse binding operations");
}

void Queue::submit(lent_ptr<CommandBuffer> cmdBuffer,
    VkPipelineStageFlags waitDstStageMask /* 0 */,
    lent_ptr<const Semaphore> waitSemaphore /* nullptr */,
    lent_ptr<Semaphore> signalSemaphore /* nullptr */,
    lent_ptr<Fence> fence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    MAGMA_ASSERT(cmdBuffer->primary());
    MAGMA_ASSERT(cmdBuffer->getState() == CommandBuffer::State::Executable);
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = extendedInfo.headNode();
    if (waitSemaphore)
    {
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphore->getHandleAddress();
    }
    submitInfo.pWaitDstStageMask = &waitDstStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = cmdBuffer->getLean().getHandleAddress();
    if (signalSemaphore)
    {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphore->getHandleAddress();
    }
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, MAGMA_OPTIONAL_HANDLE(fence));
    MAGMA_HANDLE_RESULT(result, "queue submission failed");
    MAGMA_INUSE(waitSemaphore);
    MAGMA_INUSE(signalSemaphore);
    cmdBuffer->queueSubmissionFinished();
    submittedCmdBuffers.insert(cmdBuffer.get());
    if (fence)
        fence->completeExecutionOnSignaled(std::move(cmdBuffer));
}

void Queue::submit(std::vector<lent_ptr<CommandBuffer>> cmdBuffers,
    const std::initializer_list<VkPipelineStageFlags>& waitDstStageMask /* void */,
    const std::initializer_list<lent_ptr<const Semaphore>>& waitSemaphores /* void */,
    const std::initializer_list<lent_ptr<Semaphore>>& signalSemaphores /* void */,
    lent_ptr<Fence> fence  /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    MAGMA_ASSERT(cmdBuffers.size());
    if (0 == cmdBuffers.size())
        return;
    MAGMA_VLA(VkCommandBuffer, dereferencedCmdBuffers, cmdBuffers.size());
    MAGMA_VLA(VkSemaphore, dereferencedWaitSemaphores, waitSemaphores.size());
    MAGMA_VLA(VkSemaphore, dereferencedSignalSemaphores, signalSemaphores.size());
    // https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkSubmitInfo.html
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = extendedInfo.headNode();
    if (waitSemaphores.size() > 0)
    {
        for (auto const& semaphore: waitSemaphores)
        {
            if (semaphore)
            {
                dereferencedWaitSemaphores.put(*semaphore);
                MAGMA_INUSE(semaphore);
            }
        }
        submitInfo.waitSemaphoreCount = dereferencedWaitSemaphores.count();
        submitInfo.pWaitSemaphores = dereferencedWaitSemaphores;
        submitInfo.pWaitDstStageMask = waitDstStageMask.begin();
    }
    for (auto const& cmdBuffer: cmdBuffers)
    {
        if (cmdBuffer)
        {
            MAGMA_ASSERT(cmdBuffer->primary());
            MAGMA_ASSERT(cmdBuffer->getState() == CommandBuffer::State::Executable);
            dereferencedCmdBuffers.put(cmdBuffer->getLean());
            submittedCmdBuffers.insert(cmdBuffer.get());
        }
    }
    submitInfo.commandBufferCount = dereferencedCmdBuffers.count();
    submitInfo.pCommandBuffers = dereferencedCmdBuffers;
    if (signalSemaphores.size() > 0)
    {
        for (auto const& semaphore: signalSemaphores)
        {
            if (semaphore)
            {
                dereferencedSignalSemaphores.put(*semaphore);
                MAGMA_INUSE(semaphore);
            }
        }
        submitInfo.signalSemaphoreCount = dereferencedSignalSemaphores.count();
        submitInfo.pSignalSemaphores = dereferencedSignalSemaphores;
    }
    const VkResult result = vkQueueSubmit(handle, 1, &submitInfo, MAGMA_OPTIONAL_HANDLE(fence));
    MAGMA_HANDLE_RESULT(result, "queue submission failed");
    for (auto& cmdBuffer: cmdBuffers)
    {
        cmdBuffer->queueSubmissionFinished();
        if (fence)
            fence->completeExecutionOnSignaled(std::move(cmdBuffer));
    }
}

#ifdef VK_KHR_timeline_semaphore
void Queue::submit(lent_ptr<const TimelineSemaphore> semaphore, uint64_t waitValue, uint64_t signalValue)
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
void Queue::submit(lent_ptr<const D3d12ExternalSemaphore> semaphore, uint64_t waitValue, uint64_t signalValue)
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
void Queue::submit(lent_ptr<const D3d12ExternalTimelineSemaphore> semaphore, uint64_t waitValue, uint64_t signalValue)
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
void Queue::submitDeviceGroup(std::vector<lent_ptr<CommandBuffer>> cmdBuffers,
    const std::initializer_list<uint32_t>& cmdBufferDeviceMasks /* void */,
    const std::initializer_list<VkPipelineStageFlags>& waitDstStageMask /* void */,
    const std::initializer_list<lent_ptr<const Semaphore>>& waitSemaphores /* void */,
    const std::initializer_list<uint32_t>& waitSemaphoreDeviceIndices /* void */,
    const std::initializer_list<lent_ptr<Semaphore>>& signalSemaphores /* void */,
    const std::initializer_list<uint32_t>& signalSemaphoreDeviceIndices /* void */,
    lent_ptr<Fence> fence  /* nullptr */)
{
    VkDeviceGroupSubmitInfo deviceGroupSubmitInfo;
    deviceGroupSubmitInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO;
    deviceGroupSubmitInfo.pNext = nullptr;
    deviceGroupSubmitInfo.waitSemaphoreCount = core::countof(waitSemaphoreDeviceIndices);
    deviceGroupSubmitInfo.pWaitSemaphoreDeviceIndices = waitSemaphoreDeviceIndices.begin();
    deviceGroupSubmitInfo.commandBufferCount = core::countof(cmdBufferDeviceMasks);
    deviceGroupSubmitInfo.pCommandBufferDeviceMasks = cmdBufferDeviceMasks.begin();
    deviceGroupSubmitInfo.signalSemaphoreCount = core::countof(signalSemaphoreDeviceIndices);
    deviceGroupSubmitInfo.pSignalSemaphoreDeviceIndices = signalSemaphoreDeviceIndices.begin();
    submit(std::move(cmdBuffers), waitDstStageMask, waitSemaphores, signalSemaphores, std::move(fence), StructureChain(deviceGroupSubmitInfo));
}
#endif // VK_KHR_device_group

void Queue::waitIdle()
{
    const VkResult result = vkQueueWaitIdle(handle);
    MAGMA_HANDLE_RESULT(result, "failed to wait for a queue to become idle");
    if (VK_SUCCESS == result)
        onIdle();
}

void Queue::present(const std::unique_ptr<Swapchain>& swapchain, uint32_t imageIndex,
    lent_ptr<const Semaphore> waitSemaphore /* nullptr */,
    lent_ptr<Fence> presentFence /* nullptr */,
    const StructureChain& extendedInfo /* default */)
{
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = extendedInfo.headNode();
    if (waitSemaphore)
    {
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = waitSemaphore->getHandleAddress();
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
    MAGMA_ASSERT(MAGMA_PRESENT_SUCCEEDED(result));
    MAGMA_HANDLE_RESULT(result, "queue present failed");
}

#ifdef VK_KHR_display_swapchain
void Queue::presentDisplay(const std::unique_ptr<Swapchain>& swapchain, uint32_t imageIndex,
    const VkRect2D& srcRect, const VkRect2D& dstRect, bool persistent,
    lent_ptr<const Semaphore> waitSemaphore /* nullptr */,
    lent_ptr<Fence> presentFence /* nullptr */)
{
    VkDisplayPresentInfoKHR displayPresentInfo;
    displayPresentInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR;
    displayPresentInfo.pNext = nullptr;
    displayPresentInfo.srcRect = srcRect;
    displayPresentInfo.dstRect = dstRect;
    displayPresentInfo.persistent = MAGMA_BOOLEAN(persistent);
    present(swapchain, imageIndex, std::move(waitSemaphore), std::move(presentFence),
        StructureChain(displayPresentInfo));
}
#endif // VK_KHR_display_swapchain

#ifdef VK_NV_device_diagnostic_checkpoints
std::vector<VkCheckpointDataNV> Queue::getCheckpoints(lent_ptr<const Device> device) const
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

uint32_t Queue::inUseObjectCount() const noexcept
{
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    return core::countof(inUse);
#else
    return 0;
#endif // MAGMA_RETAIN_OBJECTS_IN_USE
}

/* 3.3.1. Object Lifetime
   The following Vulkan objects must not be destroyed while
   any queue is executing commands that use the object:
    * VkFence
    * VkSemaphore
    * VkCommandBuffer
    * VkCommandPool */
void Queue::onIdle()
{
    for (auto cmdBuffer: submittedCmdBuffers)
    {
        if (CommandBuffer::State::Pending == cmdBuffer->getState())
            cmdBuffer->executionFinished();
    }
    submittedCmdBuffers.clear();
#ifdef MAGMA_RETAIN_OBJECTS_IN_USE
    inUse.clear();
#endif
}
} // namespace magma
