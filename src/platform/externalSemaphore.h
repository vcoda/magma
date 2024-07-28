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
#pragma once
#include "../objects/semaphore.h"
#ifdef VK_KHR_external_semaphore_win32
#include "win32ExternalSemaphore.h"
#endif

namespace magma
{
    /* An application using external memory may wish to
       synchronize access to that memory using semaphores.
       External semaphore allows to import and export
       non-Vulkan handle that reference the underlying
       synchronization primitive. */

#ifdef VK_KHR_external_semaphore
    class ExternalSemaphore : public Semaphore
    #ifdef VK_KHR_external_semaphore_win32
        ,public Win32ExternalSemaphore
    #endif
    {
    public:
        explicit ExternalSemaphore(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit ExternalSemaphore(std::shared_ptr<Device> device,
        #ifdef VK_KHR_external_semaphore_win32
            HANDLE hSemaphore,
            LPCWSTR name = nullptr,
        #elif defined(VK_FUCHSIA_external_semaphore)
            zx_handle_t zirconHandle,
        #elif defined(VK_KHR_external_semaphore_fd)
            int fd,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            VkSemaphoreImportFlagsKHR importFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~ExternalSemaphore();
    #ifdef VK_KHR_external_semaphore_win32
        // HANDLE Win32ExternalSemaphore::getNtHandle() const;
    #elif defined(VK_FUCHSIA_external_semaphore)
        zx_handle_t getEvent() const;
    #elif defined(VK_KHR_external_semaphore_fd)
        int getFd() const;
    #endif

    private:
    #ifdef VK_KHR_external_semaphore_win32
        // Win32ExternalSemaphore::hSemaphore;
    #elif defined(VK_FUCHSIA_external_semaphore)
        mutable zx_handle_t zirconHandle;
    #elif defined(VK_KHR_external_semaphore_fd)
        mutable int fd;
    #endif
    };
#endif // VK_KHR_external_semaphore
} // namespace magma
