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
#include "fence.h"

namespace magma
{
    /* An application using external memory may wish to
       synchronize access to that memory using fences.
       External fences allows to export non-Vulkan handle
       that reference the underlying synchronization primitive. */

#ifdef VK_KHR_external_fence
    class ExternalFence : public Fence
    {
    public:
        explicit ExternalFence(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkFenceCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit ExternalFence(std::shared_ptr<Device> device,
        #if defined(VK_KHR_external_fence_win32)
            HANDLE hFence,
            LPCWSTR name = nullptr,
        #elif defined(VK_KHR_external_fence_fd)
            int fd,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkFenceCreateFlags flags = 0,
            VkFenceImportFlags importFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~ExternalFence();
    #if defined(VK_KHR_external_fence_win32)
        HANDLE getNtHandle() const;
    #elif defined(VK_KHR_external_fence_fd)
        int getFd() const;
    #endif

    private:
    #if defined(VK_KHR_external_fence_win32)
        mutable HANDLE hFence;
    #elif defined(VK_KHR_external_fence_fd)
        mutable int fd;
    #endif
    };
#endif // VK_KHR_external_fence
} // namespace magma
