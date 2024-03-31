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
#include "../objects/timelineSemaphore.h"

namespace magma
{
    /* An NT handle returned by ID3D12Device::CreateSharedHandle
       referring to a Direct3D 12 fence, or ID3D11Device5::CreateFence
       referring to a Direct3D 11 fence. It owns a reference
       to the underlying synchronization primitive associated
       with the Direct3D fence. */

    class D3d12ExternalSemaphore : public Semaphore
    {
    public:
        explicit D3d12ExternalSemaphore(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit D3d12ExternalSemaphore(std::shared_ptr<Device> device,
            HANDLE hFenceHandle,
            LPCWSTR name = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            VkSemaphoreImportFlags importFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~D3d12ExternalSemaphore();
        HANDLE getNtHandle() const;

    private:
        mutable HANDLE hSemaphore;
    };

#ifdef VK_KHR_timeline_semaphore
    class D3d12ExternalTimelineSemaphore : public TimelineSemaphore
    {
    public:
        explicit D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device,
            uint64_t initialValue,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit D3d12ExternalTimelineSemaphore(std::shared_ptr<Device> device,
            uint64_t initialValue,
            HANDLE hFenceHandle,
            LPCWSTR name = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSemaphoreCreateFlags flags = 0,
            VkSemaphoreImportFlags importFlags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~D3d12ExternalTimelineSemaphore();
        HANDLE getNtHandle() const;

    private:
        mutable HANDLE hSemaphore;
    };

    typedef D3d12ExternalTimelineSemaphore D3d11ExternalTimelineSemaphore;
#endif // VK_KHR_timeline_semaphore
    typedef D3d12ExternalSemaphore D3d11ExternalSemaphore;
} // namespace magma
#endif // VK_KHR_external_semaphore && VK_KHR_external_semaphore_win32
