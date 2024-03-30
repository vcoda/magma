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
#include "externalSemaphore.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_external_semaphore
ExternalSemaphore::ExternalSemaphore(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkSemaphoreCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Semaphore(std::move(allocator), std::move(device)),
#if defined(VK_KHR_external_semaphore_win32)
    hSemaphore(NULL)
#elif defined(VK_FUCHSIA_external_semaphore)
    zxEvent(0)
#elif defined(VK_KHR_external_semaphore_fd)
    fd(0)
#endif
{
    VkSemaphoreCreateInfo semaphoreInfo;
    VkExportSemaphoreCreateInfoKHR exportSemaphoreInfo;
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = &exportSemaphoreInfo;
    semaphoreInfo.flags = flags;
    exportSemaphoreInfo.sType = VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO_KHR;
    exportSemaphoreInfo.pNext = extendedInfo.chainNodes();
#if defined(VK_KHR_external_semaphore_win32)
    exportSemaphoreInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR;
#elif defined(VK_FUCHSIA_external_semaphore)
    exportSemaphoreInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_ZIRCON_EVENT_BIT_FUCHSIA;
#elif defined(VK_KHR_external_semaphore_fd)
    exportSemaphoreInfo.handleTypes = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
#else
    exportSemaphoreInfo.handleTypes = 0;
#endif
    const VkResult result = vkCreateSemaphore(MAGMA_HANDLE(device), &semaphoreInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create external semaphore");
}

ExternalSemaphore::~ExternalSemaphore()
{
#if defined(VK_KHR_external_semaphore_win32)
    CloseHandle(hSemaphore);
#elif defined(VK_FUCHSIA_external_semaphore)
    zx_handle_close(zxEvent);
#elif defined(VK_KHR_external_semaphore_fd)
    close(fd);
#endif
}

#if defined(VK_KHR_external_semaphore_win32)
HANDLE ExternalSemaphore::getNtHandle() const
{
    VkSemaphoreGetWin32HandleInfoKHR win32HandleInfo;
    win32HandleInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR;
    win32HandleInfo.pNext = nullptr;
    win32HandleInfo.semaphore = handle;
    win32HandleInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetSemaphoreWin32HandleKHR, VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
    const VkResult result = vkGetSemaphoreWin32HandleKHR(MAGMA_HANDLE(device), &win32HandleInfo, &hSemaphore);
    MAGMA_HANDLE_RESULT(result, "failed to get Win32 handle");
    return hSemaphore;
}

#elif defined(VK_FUCHSIA_external_semaphore)
zx_handle_t ExternalSemaphore::getEvent() const
{
    VkSemaphoreGetZirconHandleInfoFUCHSIA zirconHandleInfo;
    zirconHandleInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_ZIRCON_HANDLE_INFO_FUCHSIA;
    zirconHandleInfo.pNext = nullptr;
    zirconHandleInfo.semaphore = handle;
    zirconHandleInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_ZIRCON_EVENT_BIT_FUCHSIA;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetSemaphoreWin32HandleKHR, VK_FUCHSIA_EXTERNAL_SEMAPHORE_EXTENSION_NAME);
    const VkResult result = vkGetSemaphoreZirconHandleFUCHSIA(MAGMA_HANDLE(device), &zirconHandleInfo, &zxEvent);
    MAGMA_HANDLE_RESULT(result, "failed to get Zircon event object");
    return zxEvent;
}

#elif defined(VK_KHR_external_semaphore_fd)
int ExternalSemaphore::getFd() const
{
    VkSemaphoreGetFdInfoKHR fdInfo;
    fdInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR;
    fdInfo.pNext = nullptr;
    fdInfo.semaphore = handle;
    fdInfo.handleType = VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetSemaphoreFdKHR, VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME);
    const VkResult result = vkGetSemaphoreFdKHR(MAGMA_HANDLE(device), &fdInfo, &fd);
    MAGMA_HANDLE_RESULT(result, "failed to get POSIX file descriptor");
    return fd;
}
#endif // VK_KHR_external_semaphore_fd
#endif // VK_KHR_external_semaphore
} // namespace magma
