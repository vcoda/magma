/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../objects/deferredOperation.h"

namespace magma
{
    class DeferredOperation;

    namespace helpers
    {
        /* Helper function to execute deferred operation concurrently.
           Parameter <maxConcurrency> should be used only if application
           wants to reduce the parallelism for this task. Otherwise,
           deferred operation will be scheduled onto all available
           threads in the system. */

    #ifdef VK_KHR_deferred_host_operations
        VkResult executeDeferredOperation(std::shared_ptr<DeferredOperation> deferredOperation,
            uint32_t maxConcurrency = 0);
    #endif
    } // namespace helpers
} // namespace magma
