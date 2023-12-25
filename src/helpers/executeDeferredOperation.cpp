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
#include "pch.h"
#pragma hdrstop
#include "../objects/deferredOperation.h"

namespace magma
{
namespace helpers
{
#ifdef VK_KHR_deferred_host_operations
VkResult executeDeferredOperation(std::shared_ptr<DeferredOperation> deferredOperation,
    uint32_t maxConcurrency /* 0 */)
{
    // The maximum number of threads that can usefully execute a deferred operation concurrently.
    // This value is intended to be used to better schedule work onto available threads.
    const uint32_t maxDeferredOpConcurrency = deferredOperation->getMaxConcurrency();
    // Implementations may return 2^32-1 to indicate that the maximum concurrency is unknown
    // and cannot be easily derived. Implementations may return values larger than the maximum
    // concurrency available on the host CPU. In these situations, an application should clamp
    // the return value rather than oversubscribing the machine.
    const uint32_t hardwareConcurrency = std::thread::hardware_concurrency();
    const uint32_t deferredOpConcurrency = std::min(hardwareConcurrency, maxDeferredOpConcurrency);
    maxConcurrency = maxConcurrency ? std::min(maxConcurrency, deferredOpConcurrency)
                                    : deferredOpConcurrency;
    std::vector<std::future<void>> tasks;
    tasks.reserve(maxConcurrency);
    for (uint32_t i = 0; i < maxConcurrency; ++i)
    {
        tasks.emplace_back(
            std::async(std::launch::async,
                [&deferredOperation]()
                {   // Applications can join multiple threads to the same deferred operation,
                    // enabling concurrent execution of subtasks within that operation.
                    const VkResult result = deferredOperation->join();
                    // Future calls to vkDeferredOperationJoinKHR are not necessary and will simply harm performance.
                    MAGMA_ASSERT(result != VK_THREAD_DONE_KHR);
                }));
    }
    for (auto& future : tasks)
        future.get();
    // If the deferred operation is complete, it returns the appropriate return value from
    // the original command. This value must be one of the VkResult values which could have been
    // returned by the original command if the operation had not been deferred.
    return deferredOperation->getResult();
}
#endif // VK_KHR_deferred_host_operations
} // namespace helpers
} // namespace magma
