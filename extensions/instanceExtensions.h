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
#include "extensions.h"

namespace magma
{
    /* Global functionality that affects the entire Vulkan API, including instance
       and device-level objects and commands, should be an instance extension.
       Keep extensions list in alphabetical order. */

    class InstanceExtensions : public Extensions
    {
    public:
        // ANDROID (Google LLC)
        const VkBool32 ANDROID_native_buffer : 1;

        // Multivendor
        const VkBool32 EXT_acquire_drm_display : 1;
        const VkBool32 EXT_acquire_xlib_display : 1;
        const VkBool32 EXT_debug_report : 1;
        const VkBool32 EXT_debug_utils : 1;
        const VkBool32 EXT_direct_mode_display : 1;
        const VkBool32 EXT_directfb_surface : 1;
        const VkBool32 EXT_display_surface_counter : 1;
        const VkBool32 EXT_metal_surface : 1;
        const VkBool32 EXT_swapchain_colorspace : 1;

        // FUCHSIA (Google LLC)
        const VkBool32 FUCHSIA_imagepipe_surface : 1;

        // GGP (Google, LLC)
        const VkBool32 GGP_stream_descriptor_surface : 1;

        // Khronos
        const VkBool32 KHR_android_surface : 1;
        const VkBool32 KHR_device_group_creation : 1;
        const VkBool32 KHR_display : 1;
        const VkBool32 KHR_external_fence_capabilities : 1;
        const VkBool32 KHR_external_memory_capabilities : 1;
        const VkBool32 KHR_external_semaphore_capabilities : 1;
        const VkBool32 KHR_get_display_properties2 : 1;
        const VkBool32 KHR_get_physical_device_properties2 : 1;
        const VkBool32 KHR_get_surface_capabilities2 : 1;
        const VkBool32 KHR_surface : 1;
        const VkBool32 KHR_surface_protected_capabilities : 1;
        const VkBool32 KHR_wayland_surface : 1;
        const VkBool32 KHR_win32_surface : 1;
        const VkBool32 KHR_xcb_surface : 1;
        const VkBool32 KHR_xlib_surface : 1;

        // MoltenVK
        const VkBool32 MVK_ios_surface : 1;
        const VkBool32 MVK_macos_surface : 1;

        // NVIDIA Corporation
        const VkBool32 NV_external_memory_capabilities : 1;

        // BlackBerry Limited
        const VkBool32 QNX_screen_surface : 1;

        InstanceExtensions(const char *layerName = nullptr);
    };

    MAGMA_TYPEDEF_UNIQUE_PTR(InstanceExtensions)
} // namespace magma
