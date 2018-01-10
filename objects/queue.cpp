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

namespace magma
{
Queue::Queue(std::shared_ptr<const Device> device, uint32_t queueFamilyIndex /* 0 */, uint32_t queueIndex /* 0 */):
    Handle(VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT, device)
{
    vkGetDeviceQueue(*device, queueFamilyIndex, queueIndex, &handle);
    if (VK_NULL_HANDLE == handle)
        throw Exception("failed to get device queue");
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
    VkSubmitInfo info;
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    if (waitSemaphores.empty())
    {
        info.waitSemaphoreCount = 0;
        info.pWaitSemaphores = nullptr;
    }
    else
    {
        for (const auto& semaphore : waitSemaphores)
            dereferencedWaitSemaphores.put(*semaphore);
        info.waitSemaphoreCount = MAGMA_COUNT(dereferencedWaitSemaphores);
        info.pWaitSemaphores = dereferencedWaitSemaphores.data();
    }
    info.pWaitDstStageMask = &waitStageMask;  
    for (const auto& cmdBuffer : commandBuffers)
        dereferencedCommandBuffers.put(*cmdBuffer);
    info.commandBufferCount = MAGMA_COUNT(dereferencedCommandBuffers);
    info.pCommandBuffers = dereferencedCommandBuffers.data();  
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
        info.pSignalSemaphores = dereferencedSignalSemaphores.data();
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
        fence);
}

bool Queue::waitIdle() noexcept
{
    const VkResult wait = vkQueueWaitIdle(handle);
    return (VK_SUCCESS == wait);
}

bool Queue::present(std::shared_ptr<const Swapchain> swapchain, uint32_t imageIndex, 
    std::shared_ptr<const Semaphore> waitSemaphore /* nullptr */) noexcept
{
    VkPresentInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    if (waitSemaphore)
    {
        const VkSemaphore dereferencedWaitSemaphores[1] = {*waitSemaphore};
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = dereferencedWaitSemaphores;
    }
    else
    {
        info.waitSemaphoreCount = 0;
        info.pWaitSemaphores = nullptr;
    }
    const VkSwapchainKHR dereferencedSwapchains[1] = {*swapchain};
    info.swapchainCount = 1;
    info.pSwapchains = dereferencedSwapchains;
    info.pImageIndices = &imageIndex;
    info.pResults = nullptr;
    const VkResult present = vkQueuePresentKHR(handle, &info);
    return (VK_SUCCESS == present);
}
} // namespace magma
