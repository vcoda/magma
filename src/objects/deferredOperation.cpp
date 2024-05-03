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
#include "deferredOperation.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_deferred_host_operations
DeferredOperation::DeferredOperation(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR, std::move(device), std::move(allocator))
{
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateDeferredOperationKHR, VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
    const VkResult result = vkCreateDeferredOperationKHR(getNativeDevice(), MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create deferred operation");
}

DeferredOperation::~DeferredOperation()
{
    MAGMA_DEVICE_EXTENSION(vkDestroyDeferredOperationKHR);
    vkDestroyDeferredOperationKHR(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

uint32_t DeferredOperation::getMaxConcurrency() const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkGetDeferredOperationMaxConcurrencyKHR);
    return vkGetDeferredOperationMaxConcurrencyKHR(getNativeDevice(), handle);
}

VkResult DeferredOperation::getResult() const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkGetDeferredOperationResultKHR);
    return vkGetDeferredOperationResultKHR(getNativeDevice(), handle);
}

VkResult DeferredOperation::join() const noexcept
{
    MAGMA_DEVICE_EXTENSION(vkDeferredOperationJoinKHR);
    return vkDeferredOperationJoinKHR(getNativeDevice(), handle);
}
#endif // VK_KHR_deferred_host_operations
} // namespace magma
