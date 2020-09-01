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
#include "extensions.h"

namespace magma
{
    /* Global functionality that affects the entire Vulkan API,
       including instance and device-level objects and commands,
       should be an instance extension.
       Keep extensions list in alphabetical order. */

    class InstanceExtensions : public Extensions
    {
    public:
        const bool ANDROID_native_buffer;

        const bool EXT_acquire_xlib_display;
        const bool EXT_debug_report;
        const bool EXT_debug_utils;
        const bool EXT_direct_mode_display;
        const bool EXT_display_surface_counter;
        const bool EXT_metal_surface;
        const bool EXT_swapchain_colorspace;

        const bool KHR_android_surface;
        const bool KHR_device_group_creation;
        const bool KHR_display;
        const bool KHR_external_fence_capabilities;
        const bool KHR_external_memory_capabilities;
        const bool KHR_external_semaphore_capabilities;
        const bool KHR_get_display_properties2;
        const bool KHR_get_physical_device_properties2;
        const bool KHR_get_surface_capabilities2;
        const bool KHR_surface;
        const bool KHR_surface_protected_capabilities;
        const bool KHR_wayland_surface;
        const bool KHR_win32_surface;
        const bool KHR_xcb_surface;
        const bool KHR_xlib_surface;

        const bool MVK_ios_surface;
        const bool MVK_macos_surface;

        const bool NV_external_memory_capabilities;

        InstanceExtensions(const char *layerName = nullptr);
    };
} // namespace magma
