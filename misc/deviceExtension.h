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
#include <map>
#include "../objects/device.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
    template<typename Function>
    class DeviceExtension
    {
    public:
        DeviceExtension(VkDevice device, 
            const char *name, const char *extension,
            bool throwNotPresent)
        {
            if (!addr)
            {
                MAGMA_ASSERT(device != VK_NULL_HANDLE);
                MAGMA_ASSERT(name);
                addr = vkGetDeviceProcAddr(device, name);
                if (!addr && throwNotPresent)
                    MAGMA_THROW_NOT_PRESENT(extension);
            }
        }
        operator Function() const 
            { return reinterpret_cast<Function>(addr); }

    private:
        static PFN_vkVoidFunction addr;
    };

    template<typename Function>
    PFN_vkVoidFunction DeviceExtension<Function>::addr;
} // namespace magma

#define MAGMA_DEVICE_EXTENSION(func, extension)\
    DeviceExtension<PFN_##func> func(MAGMA_HANDLE(device), MAGMA_STRINGIZE(func), extension, true)
#define MAGMA_OPTIONAL_DEVICE_EXTENSION(func)\
    DeviceExtension<PFN_##func> func(MAGMA_HANDLE(device), MAGMA_STRINGIZE(func), nullptr, false)
