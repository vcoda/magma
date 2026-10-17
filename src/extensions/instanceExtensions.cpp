/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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

namespace magma
{
InstanceExtensions::InstanceExtensions(const char *layerName /* nullptr */):
    ExtensionRegistry<VkExtensionProperties>(Instance::enumerateExtensions(layerName)),

    MAGMA_EXTENSION(EXT_acquire_drm_display),
    MAGMA_EXTENSION(EXT_acquire_xlib_display),
    MAGMA_EXTENSION(EXT_debug_report),
    MAGMA_EXTENSION(EXT_debug_utils),
    MAGMA_EXTENSION(EXT_direct_mode_display),
    MAGMA_EXTENSION(EXT_directfb_surface),
    MAGMA_EXTENSION(EXT_display_surface_counter),
    MAGMA_EXTENSION(EXT_headless_surface),
    MAGMA_EXTENSION(EXT_layer_settings),
    MAGMA_EXTENSION(EXT_metal_surface),
    MAGMA_EXTENSION(EXT_surface_maintenance1),
    MAGMA_EXTENSION(EXT_swapchain_colorspace),
    MAGMA_EXTENSION(EXT_validation_features),
    MAGMA_EXTENSION(EXT_validation_flags),

    MAGMA_EXTENSION(FUCHSIA_imagepipe_surface),

    MAGMA_EXTENSION(GGP_stream_descriptor_surface),

    MAGMA_EXTENSION(GOOGLE_surfaceless_query),

    MAGMA_EXTENSION(KHR_android_surface),
    MAGMA_EXTENSION(KHR_device_group_creation),
    MAGMA_EXTENSION(KHR_display),
    MAGMA_EXTENSION(KHR_external_fence_capabilities),
    MAGMA_EXTENSION(KHR_external_memory_capabilities),
    MAGMA_EXTENSION(KHR_external_semaphore_capabilities),
    MAGMA_EXTENSION(KHR_get_display_properties2),
    MAGMA_EXTENSION(KHR_get_physical_device_properties2),
    MAGMA_EXTENSION(KHR_get_surface_capabilities2),
    MAGMA_EXTENSION(KHR_mir_surface),
    MAGMA_EXTENSION(KHR_portability_enumeration),
    MAGMA_EXTENSION(KHR_surface),
    MAGMA_EXTENSION(KHR_surface_maintenance1),
    MAGMA_EXTENSION(KHR_surface_protected_capabilities),
    MAGMA_EXTENSION(KHR_wayland_surface),
    MAGMA_EXTENSION(KHR_win32_surface),
    MAGMA_EXTENSION(KHR_xcb_surface),
    MAGMA_EXTENSION(KHR_xlib_surface),

    MAGMA_EXTENSION(KHX_device_group_creation),
    MAGMA_EXTENSION(KHX_external_memory_capabilities),
    MAGMA_EXTENSION(KHX_external_semaphore_capabilities),

    MAGMA_EXTENSION(LUNARG_direct_driver_loading),

    MAGMA_EXTENSION(MVK_ios_surface),
    MAGMA_EXTENSION(MVK_macos_surface),

    MAGMA_EXTENSION(NN_vi_surface),

    MAGMA_EXTENSION(NV_display_stereo),
    MAGMA_EXTENSION(NV_external_memory_capabilities),

    MAGMA_EXTENSION(OHOS_surface),

    MAGMA_EXTENSION(QNX_screen_surface),

    MAGMA_EXTENSION(SEC_ubm_surface)
{}
} // namespace magma
