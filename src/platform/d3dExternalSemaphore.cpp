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
#include "d3dExternalSemaphore.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_external_semaphore_win32
D3d12ExternalSemaphore::D3d12ExternalSemaphore(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Semaphore(std::move(allocator), std::move(device)),
    Win32ExternalSemaphore(this, VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR)
{
    VkSemaphoreCreateInfo semaphoreInfo;
    VkExportSemaphoreCreateInfoKHR exportSemaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = &exportSemaphoreInfo;
    semaphoreInfo.flags = flags;
    exportSemaphoreInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO_KHR;
    exportSemaphoreInfo.pNext = extendedInfo.headNode();
    exportSemaphoreInfo.handleTypes = handleType;
    const VkResult result = vkCreateSemaphore(getNativeDevice(), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create D3D external semaphore");
}

D3d12ExternalSemaphore::D3d12ExternalSemaphore(std::shared_ptr<Device> device,
    HANDLE hFence,
    LPCWSTR name /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    VkSemaphoreImportFlags importFlags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Semaphore(std::move(device), std::move(allocator), flags, extendedInfo),
    Win32ExternalSemaphore(this, VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR)
{
    importNtHandle(hFence, name, importFlags);
}

#ifdef VK_KHR_timeline_semaphore
D3d12ExternalTimelineSemaphore::D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device,
    uint64_t initialValue,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TimelineSemaphore(std::move(device), std::move(allocator)),
    Win32ExternalSemaphore(this, VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR)
{
    VkSemaphoreCreateInfo semaphoreInfo;
    VkSemaphoreTypeCreateInfoKHR semaphoreTypeInfo;
    VkExportSemaphoreCreateInfoKHR exportSemaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = &semaphoreTypeInfo;
    semaphoreInfo.flags = flags;
    semaphoreTypeInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR;
    semaphoreTypeInfo.pNext = &exportSemaphoreInfo;
    semaphoreTypeInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE_KHR;
    semaphoreTypeInfo.initialValue = initialValue;
    exportSemaphoreInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO_KHR;
    exportSemaphoreInfo.pNext = extendedInfo.headNode();
    exportSemaphoreInfo.handleTypes = handleType;
    const VkResult result = vkCreateSemaphore(getNativeDevice(), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create D3D external timeline semaphore");
}

D3d12ExternalTimelineSemaphore::D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device,
    uint64_t initialValue, HANDLE hFence,
    LPCWSTR name /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    VkSemaphoreImportFlags importFlags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TimelineSemaphore(std::move(device), initialValue, std::move(allocator), flags, extendedInfo),
    Win32ExternalSemaphore(this, VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR)
{
    importNtHandle(hFence, name, importFlags);
}
#endif // VK_KHR_timeline_semaphore
#endif // VK_KHR_external_semaphore_win32
} // namespace magma
