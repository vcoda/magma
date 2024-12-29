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
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_external_fence
ExternalFence::ExternalFence(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFenceCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Fence(std::move(allocator), std::move(device)),
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
    fenceInfo.flags = flags;
    exportFenceInfo.sType = VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO_KHR;
    exportFenceInfo.pNext = extendedInfo.headNode();
    exportFenceInfo.handleTypes =
    #if defined(VK_KHR_external_fence_win32)
        VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR;
    #elif defined(VK_KHR_external_fence_fd)
        #ifdef VK_USE_PLATFORM_ANDROID_KHR
        // https://registry.khronos.org/EGL/extensions/ANDROID/EGL_ANDROID_native_fence_sync.txt
        VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT_KHR;
        #else
        VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
        #endif
    #else
        0;
    #endif // VK_KHR_external_fence_fd
    const VkResult result = vkCreateFence(getNativeDevice(), &fenceInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create external fence");
}

ExternalFence::ExternalFence(std::shared_ptr<Device> device,
#if defined(VK_KHR_external_fence_win32)
    HANDLE hFence,
    LPCWSTR name /* nullptr */,
#elif defined(VK_KHR_external_fence_fd)
    int fd,
#endif
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFenceCreateFlags flags /* 0 */,
    VkFenceImportFlagsKHR importFlags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Fence(std::move(allocator), std::move(device)),
#if defined(VK_KHR_external_fence_win32)
    hFence(NULL)
#elif defined(VK_KHR_external_fence_fd)
    fd(0)
#endif
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = extendedInfo.headNode();
    fenceInfo.flags = flags;
    VkResult result = vkCreateFence(getNativeDevice(), &fenceInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create fence");
#if defined(VK_KHR_external_fence_win32)
    VkImportFenceWin32HandleInfoKHR importWin32HandleInfo;
    importWin32HandleInfo.sType = VK_STRUCTURE_TYPE_IMPORT_FENCE_WIN32_HANDLE_INFO_KHR;
    importWin32HandleInfo.pNext = nullptr;
    importWin32HandleInfo.fence = handle;
    importWin32HandleInfo.flags = importFlags;
    importWin32HandleInfo.handleType = VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR;
    importWin32HandleInfo.handle = hFence;
    importWin32HandleInfo.name = name;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkImportFenceWin32HandleKHR, VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME);
    result = vkImportFenceWin32HandleKHR(getNativeDevice(), &importWin32HandleInfo);
    MAGMA_HANDLE_RESULT(result, "failed to import Win32 handle");
#elif defined(VK_KHR_external_fence_fd)
    VkImportFenceFdInfoKHR importFdInfo;
    importFdInfo.sType = VK_STRUCTURE_TYPE_IMPORT_FENCE_FD_INFO_KHR;
    importFdInfo.pNext = nullptr;
    importFdInfo.fence = handle;
    importFdInfo.flags = importFlags;
    importFdInfo.handleType =
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
        VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT_KHR;
    #else
        VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
    #endif
    importFdInfo.fd = fd;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkImportFenceFdKHR, VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME);
    result = vkImportFenceFdKHR(getNativeDevice(), &importFdInfo);
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    MAGMA_HANDLE_RESULT(result, "failed to import Android fence descriptor");
    #else
    MAGMA_HANDLE_RESULT(result, "failed to import POSIX file descriptor");
    #endif
#endif // VK_KHR_external_fence_fd
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
    const VkResult result = vkGetFenceWin32HandleKHR(getNativeDevice(), &win32HandleInfo, &hFence);
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
    fdInfo.handleType =
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
        VK_EXTERNAL_FENCE_HANDLE_TYPE_SYNC_FD_BIT_KHR;
    #else
        VK_EXTERNAL_FENCE_HANDLE_TYPE_OPAQUE_FD_BIT_KHR;
    #endif
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetFenceFdKHR, VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME);
    const VkResult result = vkGetFenceFdKHR(getNativeDevice(), &fdInfo, &fd);
    #ifdef VK_USE_PLATFORM_ANDROID_KHR
    MAGMA_HANDLE_RESULT(result, "failed to get Android fence descriptor");
    #else
    MAGMA_HANDLE_RESULT(result, "failed to get POSIX file descriptor");
    #endif
    return fd;
}
#endif // VK_KHR_external_fence_fd
#endif // VK_KHR_external_fence
} // namespace magma
