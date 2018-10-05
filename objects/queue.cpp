/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
    Dispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT, std::move(device), nullptr),
    flags(flags),
    familyIndex(familyIndex),
    index(index)
{
    this->handle = handle;
}

bool Queue::submit(const std::vector<std::shared_ptr<const CommandBuffer>>& commandBuffers,
    VkPipelineStageFlags waitStageMask,
    const std::vector<std::shared_ptr<const Semaphore>>& waitSemaphores,
    const std::vector<std::shared_ptr<const Semaphore>>& signalSemaphores,
    std::shared_ptr<const Fence> fence /* nullptr */) noexcept
{
    if (commandBuffers.empty())
        return false;
    MAGMA_STACK_ARRAY(VkCommandBuffer, dereferencedCommandBuffers, commandBuffers.size());
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedWaitSemaphores, waitSemaphores.size());
    MAGMA_STACK_ARRAY(VkSemaphore, dereferencedSignalSemaphores, signalSemaphores.size());
    // https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkSubmitInfo.html
    VkSubmitInfo info;
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    if (waitSemaphores.empty())
    {
        info.waitSemaphoreCount = 0;
        info.pWaitSemaphores = nullptr;
        info.pWaitDstStageMask = nullptr;
    }
    else
    {
        for (const auto& semaphore : waitSemaphores)
            dereferencedWaitSemaphores.put(*semaphore);
        info.waitSemaphoreCount = MAGMA_COUNT(dereferencedWaitSemaphores);
        info.pWaitSemaphores = dereferencedWaitSemaphores;
        info.pWaitDstStageMask = &waitStageMask;
    }
    for (const auto& cmdBuffer : commandBuffers)
        dereferencedCommandBuffers.put(*cmdBuffer);
    info.commandBufferCount = MAGMA_COUNT(dereferencedCommandBuffers);
    info.pCommandBuffers = dereferencedCommandBuffers;
    if (signalSemaphores.empty())
    {
        info.signalSemaphoreCount = 0;
        info.pSignalSemaphores = nullptr;
    }
    else
    {
        for (const auto& semaphore : signalSemaphores)
            dereferencedSignalSemaphores.put(*semaphore);
        info.signalSemaphoreCount = MAGMA_COUNT(dereferencedSignalSemaphores);
        info.pSignalSemaphores = dereferencedSignalSemaphores;
    }
    const VkResult submit = vkQueueSubmit(handle, 1, &info, MAGMA_OPTIONAL_HANDLE(fence));
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
    std::vector<std::shared_ptr<const Semaphore>> waitSemaphores, signalSemaphores;
    if (waitSemaphore)
        waitSemaphores.push_back(waitSemaphore);
    if (signalSemaphore)
        signalSemaphores.push_back(signalSemaphore);
    return submit(std::vector<std::shared_ptr<const CommandBuffer>>{commandBuffer},
        waitStageMask,
        waitSemaphores,
        signalSemaphores,
        std::move(fence));
}

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

void Queue::presentToDisplay(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex,
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

void Queue::present(std::shared_ptr<const Swapchain> swapchain,
    uint32_t imageIndex,
    const VkDisplayPresentInfoKHR *displayPresentInfo,
    std::shared_ptr<const Semaphore> waitSemaphore)
{
    VkPresentInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = displayPresentInfo;
    VkSemaphore dereferencedWaitSemaphore;
    if (waitSemaphore)
    {
        dereferencedWaitSemaphore = *waitSemaphore;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &dereferencedWaitSemaphore;
    }
    else
    {
        info.waitSemaphoreCount = 0;
        info.pWaitSemaphores = nullptr;
    }
    const VkSwapchainKHR dereferencedSwapchain = *swapchain;
    info.swapchainCount = 1;
    info.pSwapchains = &dereferencedSwapchain;
    info.pImageIndices = &imageIndex;
    info.pResults = nullptr;
    const VkResult present = vkQueuePresentKHR(handle, &info);
    MAGMA_THROW_FAILURE(present, "queue present failed");
}
} // namespace magma
