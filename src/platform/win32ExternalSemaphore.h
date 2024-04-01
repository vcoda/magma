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

namespace magma
{
    class Semaphore;

    /* An application using external memory may wish to
       synchronize access to that memory using semaphores.
       This extension enables an application to export
       semaphore payload to and import semaphore payload
       from Windows handles. */

#ifdef VK_KHR_external_semaphore_win32
    class Win32ExternalSemaphore
    {
    public:
        ~Win32ExternalSemaphore();
        HANDLE getNtHandle() const;

    protected:
        Win32ExternalSemaphore(VkExternalSemaphoreHandleTypeFlagBits handleType,
            const Semaphore *self) noexcept;
        void importNtHandle(HANDLE hSemaphore,
            LPCWSTR name,
            VkSemaphoreImportFlags flags);

    protected:
        const VkExternalSemaphoreHandleTypeFlagBits handleType;

    private:
        const Semaphore *self;
        mutable HANDLE hSemaphore;
    };
#endif // VK_KHR_external_semaphore_win32
} // namespace magma
