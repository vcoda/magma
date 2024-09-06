/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "model/nondispatchable.h"

namespace magma
{
    /* Deferred host operations enables driver operations,
       including ray tracing pipeline compilation or CPU-based
       acceleration structure construction, to be offloaded
       to application-managed CPU thread pools. */

#ifdef VK_KHR_deferred_host_operations
    class DeferredOperation : public NonDispatchable<VkDeferredOperationKHR>
    {
    public:
        explicit DeferredOperation(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DeferredOperation();
        uint32_t getMaxConcurrency() const noexcept;
        VkResult getResult() const noexcept;
        VkResult join() const noexcept;
    };
#endif // VK_KHR_deferred_host_operations
} // namespace magma
