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
#include "../objects/instance.h"
#include "../objects/device.h"
#include "../exceptions/unsupportedExtension.h"

#define MAGMA_INSTANCE_EXTENSION(function)\
    static const PFN_##function function = reinterpret_cast<PFN_##function>(\
        vkGetInstanceProcAddr(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(function)))

#define MAGMA_DEVICE_EXTENSION(function)\
    static const PFN_##function function = reinterpret_cast<PFN_##function>(\
        vkGetDeviceProcAddr(MAGMA_HANDLE(device), MAGMA_STRINGIZE(function)))

#define MAGMA_REQUIRED_INSTANCE_EXTENSION(function, extensionName)\
    MAGMA_INSTANCE_EXTENSION(function);\
    if (!function) MAGMA_THROW_UNSUPPORTED(extensionName);

#define MAGMA_REQUIRED_DEVICE_EXTENSION(function, extensionName)\
    MAGMA_DEVICE_EXTENSION(function);\
    if (!function) MAGMA_THROW_UNSUPPORTED(extensionName);
