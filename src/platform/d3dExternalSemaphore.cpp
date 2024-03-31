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
#if defined(VK_KHR_external_semaphore) && defined(VK_KHR_external_semaphore_win32)
#include "d3dExternalSemaphore.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
D3d12ExternalSemaphore::D3d12ExternalSemaphore(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Semaphore(std::move(allocator), std::move(device)),
    hSemaphore(NULL)
{
    VkSemaphoreCreateInfo semaphoreInfo;
    VkExportSemaphoreCreateInfoKHR exportSemaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = &exportSemaphoreInfo;
    semaphoreInfo.flags = flags;
    exportSemaphoreInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO_KHR;
    exportSemaphoreInfo.pNext = extendedInfo.chainNodes();
#ifdef VK_KHR_external_semaphore_capabilities
    exportSemaphoreInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR;
#endif
    const VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create D3D external semaphore");
}

D3d12ExternalSemaphore::D3d12ExternalSemaphore(std::shared_ptr<Device> device, HANDLE hFenceHandle,
    LPCWSTR name /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    VkSemaphoreImportFlags importFlags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Semaphore(std::move(allocator), std::move(device)),
    hSemaphore(NULL)
{
    VkSemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = extendedInfo.chainNodes();
    semaphoreInfo.flags = flags;
    VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create semaphore");
    VkImportSemaphoreWin32HandleInfoKHR importWin32HandleInfo;
    importWin32HandleInfo.sType = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR;
    importWin32HandleInfo.pNext = nullptr;
    importWin32HandleInfo.semaphore = handle;
    importWin32HandleInfo.flags = importFlags;
#ifdef VK_KHR_external_semaphore_capabilities
    importWin32HandleInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR;
#endif
    importWin32HandleInfo.handle = hFenceHandle;
    importWin32HandleInfo.name = name;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkImportSemaphoreWin32HandleKHR, VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
    result = vkImportSemaphoreWin32HandleKHR(MAGMA_HANDLE(device), &importWin32HandleInfo);
    MAGMA_HANDLE_RESULT(result, "failed to import D3D fence handle");
}

D3d12ExternalSemaphore::~D3d12ExternalSemaphore()
{
    CloseHandle(hSemaphore);
}

HANDLE D3d12ExternalSemaphore::getNtHandle() const
{
    VkSemaphoreGetWin32HandleInfoKHR win32HandleInfo;
    win32HandleInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR;
    win32HandleInfo.pNext = nullptr;
    win32HandleInfo.semaphore = handle;
#ifdef VK_KHR_external_semaphore_capabilities
    win32HandleInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT;
#endif
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetSemaphoreWin32HandleKHR, VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
    const VkResult result = vkGetSemaphoreWin32HandleKHR(MAGMA_HANDLE(device), &win32HandleInfo, &hSemaphore);
    MAGMA_HANDLE_RESULT(result, "failed to get Win32 handle");
    return hSemaphore;
}

#ifdef VK_KHR_external_semaphore
D3d12ExternalTimelineSemaphore::D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device, uint64_t initialValue,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TimelineSemaphore(std::move(device), std::move(allocator)),
    hSemaphore(NULL)
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
    exportSemaphoreInfo.pNext = extendedInfo.chainNodes();
#ifdef VK_KHR_external_semaphore_capabilities
    exportSemaphoreInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR;
#endif
    const VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create D3D external timeline semaphore");
}

D3d12ExternalTimelineSemaphore::D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device, uint64_t initialValue, HANDLE hFenceHandle,
    LPCWSTR name /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    VkSemaphoreImportFlags importFlags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TimelineSemaphore(std::move(device), std::move(allocator)),
    hSemaphore(NULL)
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
    VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create timeline semaphore");
    VkImportSemaphoreWin32HandleInfoKHR importWin32HandleInfo;
    importWin32HandleInfo.sType = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR;
    importWin32HandleInfo.pNext = nullptr;
    importWin32HandleInfo.semaphore = handle;
    importWin32HandleInfo.flags = importFlags;
#ifdef VK_KHR_external_semaphore_capabilities
    importWin32HandleInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT_KHR;
#endif
    importWin32HandleInfo.handle = hFenceHandle;
    importWin32HandleInfo.name = name;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkImportSemaphoreWin32HandleKHR, VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
    result = vkImportSemaphoreWin32HandleKHR(MAGMA_HANDLE(device), &importWin32HandleInfo);
    MAGMA_HANDLE_RESULT(result, "failed to import D3D fence handle");
}

D3d12ExternalTimelineSemaphore::~D3d12ExternalTimelineSemaphore()
{
    CloseHandle(hSemaphore);
}

HANDLE D3d12ExternalTimelineSemaphore::getNtHandle() const
{
    VkSemaphoreGetWin32HandleInfoKHR win32HandleInfo;
    win32HandleInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR;
    win32HandleInfo.pNext = nullptr;
    win32HandleInfo.semaphore = handle;
    win32HandleInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetSemaphoreWin32HandleKHR, VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
    const VkResult result = vkGetSemaphoreWin32HandleKHR(MAGMA_HANDLE(device), &win32HandleInfo, &hSemaphore);
    MAGMA_HANDLE_RESULT(result, "failed to get Win32 handle");
    return hSemaphore;
}
#endif // VK_KHR_timeline_semaphore
} // namespace magma
#endif // VK_KHR_external_semaphore && VK_KHR_external_semaphore_win32
