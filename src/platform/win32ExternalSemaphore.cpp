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
#include "win32ExternalSemaphore.h"
#include "../objects/semaphore.h"
#include "../objects/device.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

#define MAGMA_EXTERNAL_SEMAPHORE_WIN32_EXTENSION(proc)\
    static const magma::DeviceExtension<PFN_##proc> proc(*self->getDevice(), MAGMA_STRINGIZE(proc), VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME)

namespace magma
{
#ifdef VK_KHR_external_semaphore_win32
Win32ExternalSemaphore::Win32ExternalSemaphore(const Semaphore *self,
    VkExternalSemaphoreHandleTypeFlagBits handleType) noexcept:
    handleType(handleType),
    self(self),
    hSemaphore(NULL)
{}

Win32ExternalSemaphore::~Win32ExternalSemaphore()
{
    /* Do applications need to call CloseHandle() on the values
       returned from vkGetSemaphoreWin32HandleKHR when handleType
       is VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR? */
    if (VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_OPAQUE_WIN32_BIT_KHR == handleType)
    {
        /* Yes. A successful get call transfers ownership of the handle
           to the application. Destroying the semaphore object will not
           destroy the handle or the handle's reference to the underlying
           semaphore resource. Unlike file descriptor opaque handles,
           win32 opaque handle ownership can not be transferred back
           to a driver by an import operation. */
        CloseHandle(hSemaphore);
    }
}

HANDLE Win32ExternalSemaphore::getNtHandle() const
{
    VkSemaphoreGetWin32HandleInfoKHR win32HandleInfo;
    win32HandleInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR;
    win32HandleInfo.pNext = nullptr;
    win32HandleInfo.semaphore = *self;
    win32HandleInfo.handleType = handleType;
    MAGMA_EXTERNAL_SEMAPHORE_WIN32_EXTENSION(vkGetSemaphoreWin32HandleKHR);
    const VkResult result = vkGetSemaphoreWin32HandleKHR(*self->getDevice(), &win32HandleInfo, &hSemaphore);
    MAGMA_HANDLE_RESULT(result, "failed to get Win32 handle");
    return hSemaphore;
}

void Win32ExternalSemaphore::importNtHandle(HANDLE hSemaphore, LPCWSTR name, VkSemaphoreImportFlags flags)
{
    VkImportSemaphoreWin32HandleInfoKHR importWin32HandleInfo;
    importWin32HandleInfo.sType = VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR;
    importWin32HandleInfo.pNext = nullptr;
    importWin32HandleInfo.semaphore = *self;
    importWin32HandleInfo.flags = flags;
    importWin32HandleInfo.handleType = handleType;
    importWin32HandleInfo.handle = hSemaphore;
    importWin32HandleInfo.name = name;
    MAGMA_EXTERNAL_SEMAPHORE_WIN32_EXTENSION(vkImportSemaphoreWin32HandleKHR);
    const VkResult result = vkImportSemaphoreWin32HandleKHR(*self->getDevice(), &importWin32HandleInfo);
    MAGMA_HANDLE_RESULT(result, "failed to import Win32 handle");
}
#endif // VK_KHR_external_semaphore_win32
} // namespace magma
