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
#if defined(VK_KHR_external_semaphore) && defined(VK_KHR_external_semaphore_win32)
#include "../objects/semaphore.h"
#ifdef VK_KHR_timeline_semaphore
#include "../objects/timelineSemaphore.h"
#endif
#include "win32ExternalSemaphore.h"

namespace magma
{
    /* An NT handle returned by ID3D12Device::CreateSharedHandle
       referring to a Direct3D 12 fence, or ID3D11Device5::CreateFence
       referring to a Direct3D 11 fence. It owns a reference
       to the underlying synchronization primitive associated
       with the Direct3D fence. */

    class D3d12ExternalSemaphore : public Semaphore,
        public Win32ExternalSemaphore
    {
    public:
        explicit D3d12ExternalSemaphore(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit D3d12ExternalSemaphore(std::shared_ptr<Device> device,
            HANDLE hFence,
            LPCWSTR name = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            VkSemaphoreImportFlags importFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
    };

    /* As the introduction of the external semaphore handle type
       VK_EXTERNAL_SEMAPHORE_HANDLE_TYPE_D3D12_FENCE_BIT predates
       that of timeline semaphores, support for importing semaphore
       payloads from external handles of that type into semaphores
       created (implicitly or explicitly) with a VkSemaphoreType of
       VK_SEMAPHORE_TYPE_BINARY is preserved for backwards compatibility.
       However, applications should prefer importing such handle types
       into semaphores created with a VkSemaphoreType of
       VK_SEMAPHORE_TYPE_TIMELINE. */

#ifdef VK_KHR_timeline_semaphore
    class D3d12ExternalTimelineSemaphore : public TimelineSemaphore,
        public Win32ExternalSemaphore
    {
    public:
        explicit D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device,
            uint64_t initialValue,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device,
            uint64_t initialValue,
            HANDLE hFence,
            LPCWSTR name = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            VkSemaphoreImportFlags importFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
    };

    typedef D3d12ExternalTimelineSemaphore D3d11ExternalTimelineSemaphore;
#endif // VK_KHR_timeline_semaphore
    typedef D3d12ExternalSemaphore D3d11ExternalSemaphore;
} // namespace magma
#endif // VK_KHR_external_semaphore && VK_KHR_external_semaphore_win32
