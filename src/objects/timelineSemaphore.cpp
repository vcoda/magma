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
#include "timelineSemaphore.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_timeline_semaphore
BinarySemaphore::BinarySemaphore(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_SEMAPHORE, std::move(device), std::move(allocator))
{
    VkSemaphoreCreateInfo semaphoreInfo;
    VkSemaphoreTypeCreateInfoKHR semaphoreTypeInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = &semaphoreTypeInfo;
    semaphoreInfo.flags = flags;
    semaphoreTypeInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR;
    semaphoreTypeInfo.pNext = extendedInfo.chainNodes();
    semaphoreTypeInfo.semaphoreType = VK_SEMAPHORE_TYPE_BINARY_KHR;
    semaphoreTypeInfo.initialValue = VK_FALSE; // When created, the semaphore is in the unsignaled state
    const VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create binary semaphore");
}

BinarySemaphore::~BinarySemaphore()
{
    vkDestroySemaphore(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void BinarySemaphore::signal(bool value)
{
    VkSemaphoreSignalInfoKHR signalInfo;
    signalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO_KHR;
    signalInfo.pNext = nullptr;
    signalInfo.semaphore = handle;
    signalInfo.value = MAGMA_BOOLEAN(value);
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkSignalSemaphoreKHR, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkSignalSemaphoreKHR(MAGMA_HANDLE(device), &signalInfo);
    MAGMA_HANDLE_RESULT(result, "failed to signal binary semaphore from a host");
}

TimelineSemaphore::TimelineSemaphore(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator) noexcept:
    Semaphore(std::move(allocator), std::move(device))
{}

TimelineSemaphore::TimelineSemaphore(std::shared_ptr<Device> device, uint64_t initialValue,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Semaphore(std::move(allocator), std::move(device))
{
    VkSemaphoreCreateInfo semaphoreInfo;
    VkSemaphoreTypeCreateInfoKHR semaphoreTypeInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = &semaphoreTypeInfo;
    semaphoreInfo.flags = flags;
    semaphoreTypeInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR;
    semaphoreTypeInfo.pNext = extendedInfo.chainNodes();
    semaphoreTypeInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE_KHR;
    semaphoreTypeInfo.initialValue = initialValue;
    const VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create timeline semaphore");
}

uint64_t TimelineSemaphore::getCounterValue() const
{
    uint64_t counter = 0ull;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetSemaphoreCounterValueKHR, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkGetSemaphoreCounterValueKHR(MAGMA_HANDLE(device), handle, &counter);
    MAGMA_HANDLE_RESULT(result, "failed to get counter value of the timeline semaphore");
    return counter;
}

void TimelineSemaphore::signal(uint64_t counter)
{
    VkSemaphoreSignalInfoKHR signalInfo;
    signalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO_KHR;
    signalInfo.pNext = nullptr;
    signalInfo.semaphore = handle;
    signalInfo.value = counter;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkSignalSemaphoreKHR, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkSignalSemaphoreKHR(MAGMA_HANDLE(device), &signalInfo);
    MAGMA_HANDLE_RESULT(result, "failed to signal timeline semaphore from a host");
}

bool TimelineSemaphore::wait(uint64_t counter,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */) const
{
    VkSemaphoreWaitInfoKHR waitInfo;
    waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO_KHR;
    waitInfo.pNext = nullptr;
    waitInfo.flags = 0;
    waitInfo.semaphoreCount = 1;
    waitInfo.pSemaphores = &handle;
    waitInfo.pValues = &counter;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkWaitSemaphoresKHR, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkWaitSemaphoresKHR(MAGMA_HANDLE(device), &waitInfo, timeout);
    MAGMA_HANDLE_RESULT(result, "failed to wait timeline semaphore from a host");
    // VK_SUCCESS or VK_TIMEOUT
    return (result != VK_TIMEOUT);
}
#endif // VK_KHR_timeline_semaphore
} // namespace magma
