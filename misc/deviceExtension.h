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
#include "extension.h"
#include "../objects/device.h"

namespace magma
{
    /* In order to support systems with multiple Vulkan implementations,
       the function pointers returned by vkGetInstanceProcAddr may point
       to dispatch code that calls a different real implementation for
       different VkDevice objects or their child objects. The overhead
       of the internal dispatch for VkDevice objects can be avoided by
       obtaining device-specific function pointers for any commands that
       use a device or device-child object as their dispatchable object. */

    template<class ExtProc>
    class DeviceExtension final : public Extension<ExtProc, true>
    {
    public:
        explicit DeviceExtension(VkDevice device,
            const char *name) noexcept:
            Extension<ExtProc, true>(vkGetDeviceProcAddr(device, name)) {}
        explicit DeviceExtension(VkDevice device,
            const char *name,
            const char *extensionName):
            DeviceExtension(device, name)
        {
            Extension<ExtProc, true>::verify(extensionName);
        }
    };
} // namespace magma

#define MAGMA_DEVICE_EXTENSION(proc) static DeviceExtension<PFN_##proc> proc(MAGMA_HANDLE(device), MAGMA_STRINGIZE(proc))
#define MAGMA_REQUIRED_DEVICE_EXTENSION(proc, extensionName) static DeviceExtension<PFN_##proc> proc(MAGMA_HANDLE(device), MAGMA_STRINGIZE(proc), extensionName)
