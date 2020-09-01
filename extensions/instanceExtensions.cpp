/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY), without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "pch.h"
#pragma hdrstop
#include "instanceExtensions.h"
#include "../objects/instance.h"
#include "../core/constexprHash.h"

namespace magma
{
#define MAGMA_CHECK_EXTENSION(extension)\
    extension(this->hasExtension(core::hashString(MAGMA_EXTENSION_PREFIX #extension)))

InstanceExtensions::InstanceExtensions(const char *layerName /* nullptr */):
    Extensions(Instance::enumerateExtensions(layerName)),

    MAGMA_CHECK_EXTENSION(ANDROID_native_buffer),

    MAGMA_CHECK_EXTENSION(EXT_acquire_xlib_display),
    MAGMA_CHECK_EXTENSION(EXT_debug_report),
    MAGMA_CHECK_EXTENSION(EXT_debug_utils),
    MAGMA_CHECK_EXTENSION(EXT_direct_mode_display),
    MAGMA_CHECK_EXTENSION(EXT_display_surface_counter),
    MAGMA_CHECK_EXTENSION(EXT_metal_surface),
    MAGMA_CHECK_EXTENSION(EXT_swapchain_colorspace),

    MAGMA_CHECK_EXTENSION(KHR_android_surface),
    MAGMA_CHECK_EXTENSION(KHR_device_group_creation),
    MAGMA_CHECK_EXTENSION(KHR_display),
    MAGMA_CHECK_EXTENSION(KHR_external_fence_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_external_memory_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_external_semaphore_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_get_display_properties2),
    MAGMA_CHECK_EXTENSION(KHR_get_physical_device_properties2),
    MAGMA_CHECK_EXTENSION(KHR_get_surface_capabilities2),
    MAGMA_CHECK_EXTENSION(KHR_surface),
    MAGMA_CHECK_EXTENSION(KHR_surface_protected_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_wayland_surface),
    MAGMA_CHECK_EXTENSION(KHR_win32_surface),
    MAGMA_CHECK_EXTENSION(KHR_xcb_surface),
    MAGMA_CHECK_EXTENSION(KHR_xlib_surface),

    MAGMA_CHECK_EXTENSION(MVK_ios_surface),
    MAGMA_CHECK_EXTENSION(MVK_macos_surface),

    MAGMA_CHECK_EXTENSION(NV_external_memory_capabilities)
{}
} // namespace magma
