/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../exceptions/unsupportedExtension.h"

#define MAGMA_INSTANCE_EXTENSION(vkFunc, extensionName) static const PFN_##vkFunc vkFunc(reinterpret_cast<PFN_##vkFunc>(vkGetInstanceProcAddr(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(vkFunc))));\
    if (!vkFunc) throw magma::exception::UnsupportedInstanceExtension(extensionName, MAGMA_SOURCE_LOCATION);
#define MAGMA_DEVICE_EXTENSION(vkFunc, extensionName) static const PFN_##vkFunc vkFunc(reinterpret_cast<PFN_##vkFunc>(vkGetDeviceProcAddr(MAGMA_HANDLE(device), MAGMA_STRINGIZE(vkFunc))));\
    if (!vkFunc) throw magma::exception::UnsupportedDeviceExtension(extensionName, MAGMA_SOURCE_LOCATION);
#define MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkFunc) static const PFN_##vkFunc vkFunc(reinterpret_cast<PFN_##vkFunc>(vkGetInstanceProcAddr(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(vkFunc))))
#define MAGMA_OPTIONAL_DEVICE_EXTENSION(vkFunc) static const PFN_##vkFunc vkFunc(reinterpret_cast<PFN_##vkFunc>(vkGetDeviceProcAddr(MAGMA_HANDLE(device), MAGMA_STRINGIZE(vkFunc))))
