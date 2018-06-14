/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../objects/device.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
    namespace helpers
    {
        template<typename Function>
        MAGMA_INLINE Function extensionFuncPointer(VkInstance instance,
            const char *name,
            const char *extension,
            bool throwIfNotPresent)
        {
            MAGMA_ASSERT(instance != VK_NULL_HANDLE);
            MAGMA_ASSERT(name);
            PFN_vkVoidFunction pfn = vkGetInstanceProcAddr(instance, name);
            if (!pfn)
            {
                if (throwIfNotPresent)
                {
                    MAGMA_ASSERT(extension);
                    MAGMA_THROW_NOT_PRESENT(extension);
                }
            }
            return reinterpret_cast<Function>(pfn);
        }

        template<typename Function>
        MAGMA_INLINE Function extensionFuncPointer(VkDevice device,
            const char *name,
            const char *extension,
            bool throwIfNotPresent)
        {
            MAGMA_ASSERT(device != VK_NULL_HANDLE);
            MAGMA_ASSERT(name);
            PFN_vkVoidFunction pfn = vkGetDeviceProcAddr(device, name);
            if (!pfn)
            {
                if (throwIfNotPresent)
                {
                    MAGMA_ASSERT(extension);
                    MAGMA_THROW_NOT_PRESENT(extension);
                }
            }
            return reinterpret_cast<Function>(pfn);
        }
    } // namespace helpers
} // namespace magma

#define MAGMA_INSTANCE_EXTENSION_FUNC(func, extension)\
    magma::helpers::extensionFuncPointer<PFN_##func>(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func), extension##_EXTENSION_NAME, true);
#define MAGMA_DEVICE_EXTENSION_FUNC(func, extension)\
    magma::helpers::extensionFuncPointer<PFN_##func>(MAGMA_HANDLE(device), MAGMA_STRINGIZE(func), extension##_EXTENSION_NAME, true);
#define MAGMA_OPTIONAL_INSTANCE_EXTENSION_FUNC(func)\
    magma::helpers::extensionFuncPointer<PFN_##func>(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(func), nullptr, false);
#define MAGMA_OPTIONAL_DEVICE_EXTENSION_FUNC(func)\
    magma::helpers::extensionFuncPointer<PFN_##func>(MAGMA_HANDLE(device), MAGMA_STRINGIZE(func), nullptr, false);