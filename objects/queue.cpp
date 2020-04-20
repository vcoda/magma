/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "fence.h"
#include "swapchain.h"
#include "../helpers/stackArray.h"
#include "../misc/exception.h"

namespace magma
{
Queue::Queue(VkQueue handle, std::shared_ptr<Device> device,
    VkQueueFlagBits flags, uint32_t familyIndex, uint32_t index):
    Dispatchable(VK_OBJECT_TYPE_QUEUE, std::move(device), nullptr),
    flags(flags),
    familyIndex(familyIndex),
    index(index)
{
    this->handle = handle;
}

bool Queue::submit(const std::vector<std::shared_ptr<const CommandBuffer>>& commandBuffers,
    const std::vector<VkPipelineStageFlags>& waitStageMasks /* {} */,
    const std::vector<std::shared_ptr<const Semaphore>>& waitSemaphores /* {} */,
    const std::vector<std::shared_ptr<const Semaphore>>& signalSemaphores /* {} */,
    std::shared_ptr<const Fence> fence /* nullptr */,
    const void *extension /* nullptr */) noexcept
{
    if (commandBuffers.empty())
        return false;
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedWaitSemaphores, waitSemaphores.size());
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedSignalSemaphores, signalSemaphores.size());
    // https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkSubmitInfo.html
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = extension;
    if (waitSemaphores.empty())
    {
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = nullptr;
    }
    else
    {   // Dereference wait semaphores
        for (const auto& semaphore : waitSemaphores)
            dereferencedWaitSemaphores.put(*semaphore);
        submitInfo.waitSemaphoreCount = MAGMA_COUNT(dereferencedWaitSemaphores);
        submitInfo.pWaitSemaphores = dereferencedWaitSemaphores;
        submitInfo.pWaitDstStageMask = waitStageMasks.data();
    }
    for (const auto& cmdBuffer : commandBuffers)
        dereferencedCommandBuffers.put(*cmdBuffer);
    submitInfo.commandBufferCount = MAGMA_COUNT(dereferencedCommandBuffers);
    submitInfo.pCommandBuffers = dereferencedCommandBuffers;
    if (signalSemaphores.empty())
    {
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = nullptr;
    }
    else
    {   // Dereference signal semaphores
        for (const auto& semaphore : signalSemaphores)
            dereferencedSignalSemaphores.put(*semaphore);
        submitInfo.signalSemaphoreCount = MAGMA_COUNT(dereferencedSignalSemaphores);
        submitInfo.pSignalSemaphores = dereferencedSignalSemaphores;
    }
    const VkResult submit = vkQueueSubmit(handle, 1, &submitInfo, MAGMA_OPTIONAL_HANDLE(fence));
    return (VK_SUCCESS == submit);
}

bool Queue::submit(std::shared_ptr<const CommandBuffer> commandBuffer,
    VkPipelineStageFlags waitStageMask /* 0 */,
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */,
    std::shared_ptr<const Semaphore> signalSemaphore /* nullptr */,
    std::shared_ptr<const Fence> fence /* nullptr */) noexcept
{
    if (!commandBuffer)
        return false;
    std::vector<std::shared_ptr<const CommandBuffer>> commandBuffers = {commandBuffer};
    std::vector<std::shared_ptr<const Semaphore>> waitSemaphores;
    if (waitSemaphore)
        waitSemaphores.push_back(waitSemaphore);
    std::vector<std::shared_ptr<const Semaphore>> signalSemaphores;
    if (signalSemaphore)
        signalSemaphores.push_back(signalSemaphore);
    return submit(commandBuffers, {waitStageMask}, waitSemaphores, signalSemaphores, std::move(fence));
}

#ifdef VK_KHR_device_group
bool Queue::submitDeviceGroup(const std::vector<std::shared_ptr<const CommandBuffer>>& commandBuffers,
    const std::vector<uint32_t>& commandBufferDeviceMasks /* {} */,
    const std::vector<VkPipelineStageFlags>& waitStageMasks /* {} */,
    const std::vector<std::shared_ptr<const Semaphore>>& waitSemaphores /* {} */,
    const std::vector<uint32_t>& waitSemaphoreDeviceIndices /* {} */,
    const std::vector<std::shared_ptr<const Semaphore>>& signalSemaphores /* {} */,
    const std::vector<uint32_t>& signalSemaphoreDeviceIndices /* {} */,
    std::shared_ptr<const Fence> fence /* nullptr */) noexcept
{
    VkDeviceGroupSubmitInfo deviceGroupSubmitInfo;
    deviceGroupSubmitInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO;
    deviceGroupSubmitInfo.pNext = nullptr;
    deviceGroupSubmitInfo.waitSemaphoreCount = MAGMA_COUNT(waitSemaphoreDeviceIndices);
    deviceGroupSubmitInfo.pWaitSemaphoreDeviceIndices = waitSemaphoreDeviceIndices.data();
    deviceGroupSubmitInfo.commandBufferCount = MAGMA_COUNT(commandBufferDeviceMasks);
    deviceGroupSubmitInfo.pCommandBufferDeviceMasks = commandBufferDeviceMasks.data();
    deviceGroupSubmitInfo.signalSemaphoreCount = MAGMA_COUNT(signalSemaphoreDeviceIndices);
    deviceGroupSubmitInfo.pSignalSemaphoreDeviceIndices = signalSemaphoreDeviceIndices.data();
    return submit(commandBuffers, waitStageMasks, waitSemaphores, signalSemaphores, fence, &deviceGroupSubmitInfo);
}
#endif // VK_KHR_device_group

bool Queue::waitIdle() noexcept
{
    const VkResult wait = vkQueueWaitIdle(handle);
    return (VK_SUCCESS == wait);
}

void Queue::present(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */)
{
    present(std::move(swapchain), imageIndex, nullptr, std::move(waitSemaphore));
}

#ifdef VK_KHR_display_swapchain
void Queue::presentDisplay(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
    const VkRect2D& srcRect, const VkRect2D& dstRect, bool persistent,
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */)
{
    VkDisplayPresentInfoKHR displayPresentInfo;
    displayPresentInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR;
    displayPresentInfo.pNext = nullptr;
    displayPresentInfo.srcRect = srcRect;
    displayPresentInfo.dstRect = dstRect;
    displayPresentInfo.persistent = MAGMA_BOOLEAN(persistent);
    present(std::move(swapchain), imageIndex, &displayPresentInfo, std::move(waitSemaphore));
}
#endif // VK_KHR_display_swapchain

void Queue::present(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
    const VkDisplayPresentInfoKHR *displayPresentInfo,
    std::shared_ptr<const Semaphore> waitSemaphore)
{
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = displayPresentInfo;
    VkSemaphore dereferencedWaitSemaphore;
    if (waitSemaphore)
    {
        dereferencedWaitSemaphore = *waitSemaphore;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &dereferencedWaitSemaphore;
    }
    else
    {
        presentInfo.waitSemaphoreCount = 0;
        presentInfo.pWaitSemaphores = nullptr;
    }
    const VkSwapchainKHR dereferencedSwapchain = *swapchain;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &dereferencedSwapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    const VkResult present = vkQueuePresentKHR(handle, &presentInfo);
    MAGMA_THROW_PRESENT_FAILURE(present, "queue present failed");
}
} // namespace magma
