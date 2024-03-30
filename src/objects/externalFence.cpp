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
#include "externalFence.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_external_fence
ExternalFence::ExternalFence(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool signaled /* false */,
    const StructureChain& extendedInfo /* default */):
    Fence(std::move(device), std::move(allocator)),
#if defined(VK_KHR_external_fence_win32)
    hFence(NULL)
#elif defined(VK_KHR_external_fence_fd)
    fd(0)
#endif
{
    VkFenceCreateInfo fenceInfo;
    VkExportFenceCreateInfoKHR exportFenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = &exportFenceInfo;
    fenceInfo.flags = 0;
    if (signaled)
        fenceInfo.flags |= VK_FENCE_CREATE_SIGNALED_BIT;
    exportFenceInfo.sType = VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO_KHR;
    exportFenceInfo.pNext = extendedInfo.chainNodes();
#if defined(VK_KHR_external_fence_win32)
    exportFenceInfo.handleTypes = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR;
#elif defined(VK_KHR_external_fence_fd)
    exportFenceInfo.handleTypes = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
#else
    exportFenceInfo.handleTypes = 0;
#endif
    const VkResult result = vkCreateFence(MAGMA_HANDLE(device), &fenceInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create external fence");
}

ExternalFence::~ExternalFence()
{
#if defined(VK_KHR_external_fence_win32)
    CloseHandle(hFence);
#elif defined(VK_KHR_external_fence_fd)
    close(fd);
#endif
}

#if defined(VK_KHR_external_fence_win32)
HANDLE ExternalFence::getNtHandle() const
{
    VkFenceGetWin32HandleInfoKHR win32HandleInfo;
    win32HandleInfo.sType = VK_STRUCTURE_TYPE_FENCE_GET_WIN32_HANDLE_INFO_KHR;
    win32HandleInfo.pNext = nullptr;
    win32HandleInfo.fence = handle;
    win32HandleInfo.handleType = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetFenceWin32HandleKHR, VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME);
    const VkResult result = vkGetFenceWin32HandleKHR(MAGMA_HANDLE(device), &win32HandleInfo, &hFence);
    MAGMA_HANDLE_RESULT(result, "failed to get Win32 handle");
    return hFence;
}

#elif defined(VK_KHR_external_fence_fd)
int ExternalFence::getFd() const
{
    VkFenceGetFdInfoKHR fdInfo;
    fdInfo.sType = VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR;
    fdInfo.pNext = nullptr;
    fdInfo.fence = handle;
    fdInfo.handleType = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetFenceFdKHR, VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME);
    const VkResult result = vkGetFenceFdKHR(MAGMA_HANDLE(device), &fdInfo, &fd);
    MAGMA_HANDLE_RESULT(result, "failed to get POSIX file descriptor");
    return fd;
}
#endif // VK_KHR_external_fence_fd
#endif // VK_KHR_external_fence
} // namespace magma
