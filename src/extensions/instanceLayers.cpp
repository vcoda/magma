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
#include "instanceLayers.h"
#include "../objects/instance.h"

namespace magma
{
InstanceLayers::InstanceLayers():
    ExtensionRegistry<VkLayerProperties>(Instance::enumerateLayers()),

    MAGMA_LAYER_UNPREFIXED(AEJS_DeviceChooserLayer),

    MAGMA_LAYER(AMD_switchable_graphics),
    MAGMA_LAYER(AMD_switchable_graphics_32),
    MAGMA_LAYER(AMD_switchable_graphics_64),

    MAGMA_LAYER(Galaxy_Overlay),
    MAGMA_LAYER(Galaxy_Overlay_DEBUG),
    MAGMA_LAYER(Galaxy_Overlay_VERBOSE),
    MAGMA_LAYER_UNPREFIXED(GalaxyOverlayVkLayer),
    MAGMA_LAYER_UNPREFIXED(GalaxyOverlayVkLayer_DEBUG),
    MAGMA_LAYER_UNPREFIXED(GalaxyOverlayVkLayer_VERBOSE),

    MAGMA_LAYER_UNPREFIXED(GraphicsSpy),

    MAGMA_LAYER(fpsmon),
    MAGMA_LAYER(FPSMonitor),

    MAGMA_LAYER(GOOGLE_threading),
    MAGMA_LAYER(GOOGLE_unique_objects),

    MAGMA_LAYER(gshade),

    MAGMA_LAYER(INTEL_nullhw),
    MAGMA_LAYER(INTEL_state_tracker),

    MAGMA_LAYER(KHRONOS_memory_decompression),
    MAGMA_LAYER(KHRONOS_profiles),
    MAGMA_LAYER(KHRONOS_shader_object),
    MAGMA_LAYER(KHRONOS_synchronization2),
    MAGMA_LAYER(KHRONOS_timeline_semaphore),
    MAGMA_LAYER(KHRONOS_validation),

    MAGMA_LAYER(LUNARG_api_dump),
    MAGMA_LAYER(LUNARG_assistant_layer),
    MAGMA_LAYER(LUNARG_core_validation),
    MAGMA_LAYER(LUNARG_crash_diagnostic),
    MAGMA_LAYER(LUNARG_demo_layer),
    MAGMA_LAYER(LUNARG_device_profile_api),
    MAGMA_LAYER(LUNARG_device_simulation),
    MAGMA_LAYER(LUNARG_gfxreconstruct),
    MAGMA_LAYER(LUNARG_image),
    MAGMA_LAYER(LUNARG_monitor),
    MAGMA_LAYER(LUNARG_object_tracker),
    MAGMA_LAYER(LUNARG_overlay),
    MAGMA_LAYER(LUNARG_override),
    MAGMA_LAYER(LUNARG_parameter_validation),
    MAGMA_LAYER(LUNARG_screenshot),
    MAGMA_LAYER(LUNARG_standard_validation),
    MAGMA_LAYER(LUNARG_starter_layer),
    MAGMA_LAYER(LUNARG_swapchain),
    MAGMA_LAYER(LUNARG_TestingIntercept),
    MAGMA_LAYER(LUNARG_VkGHL32),
    MAGMA_LAYER(LUNARG_VkGHL64),
    MAGMA_LAYER(LUNARG_vktrace),
    MAGMA_LAYER(LUNARG_xenviro_layer),

    MAGMA_LAYER_UNPREFIXED(MangoHud),
    MAGMA_LAYER(MANGOAPP_overlay),
    MAGMA_LAYER(MANGOHUD32_overlay),
    MAGMA_LAYER(MANGOHUD_overlay),
    MAGMA_LAYER(MANGOHUD_overlay_32),
    MAGMA_LAYER(MANGOHUD_overlay_32_x86),
    MAGMA_LAYER(MANGOHUD_overlay_64_x86_64),
    MAGMA_LAYER(MANGOHUD_overlay_aarch64),
    MAGMA_LAYER(MANGOHUD_overlay_x32),
    MAGMA_LAYER(MANGOHUD_overlay_x32_x86_64),
    MAGMA_LAYER(MANGOHUD_overlay_x86),
    MAGMA_LAYER(MANGOHUD_overlay_x86_64),

    MAGMA_LAYER_UNPREFIXED(MoltenVK),

    MAGMA_LAYER(MESA_device_select),
    MAGMA_LAYER(MESA_overlay),

    MAGMA_LAYER(NV_nomad),
    MAGMA_LAYER(NV_nsight),
    NV_nsight_sys(MAGMA_QUERY_AVAILABILITY("VK_LAYER_NV_nsight-sys")),
    MAGMA_LAYER(NV_optimus),
    MAGMA_LAYER(NV_vgd),
    MAGMA_LAYER(Nvidia_Shader_Dump),

    MAGMA_LAYER(OBS_HOOK),
    MAGMA_LAYER(OBS_vkcapture_32),
    MAGMA_LAYER(OBS_vkcapture_64),
    MAGMA_LAYER(OW_OBS_HOOK),
    MAGMA_LAYER(OW_OVERLAY),

    MAGMA_LAYER(playclaw),

    MAGMA_LAYER(PRIMUS_PrimusVK),

    MAGMA_LAYER(RENDERDOC_ARM_Capture),
    MAGMA_LAYER(RENDERDOC_Capture),
    MAGMA_LAYER(RENDERDOC_Capture_32),

    MAGMA_LAYER(reshade),
    MAGMA_LAYER(reshade32),

    MAGMA_LAYER(RGA_pipeline_extraction),
    MAGMA_LAYER_UNPREFIXED(libVK_LAYER_RGA_pipeline_extraction),

    MAGMA_LAYER(ROCKSTAR_GAMES_social_club),

    MAGMA_LAYER(VALVE_steam_fossilize),
    MAGMA_LAYER(VALVE_steam_fossilize_32),
    MAGMA_LAYER(VALVE_steam_fossilize_64),
    MAGMA_LAYER(VALVE_steam_overlay),
    MAGMA_LAYER(VALVE_steam_overlay_32),
    MAGMA_LAYER(VALVE_steam_overlay_64),

    MAGMA_LAYER(VKBASALT_post_processing),
    MAGMA_LAYER(VKBASALT_post_processing_32),
    MAGMA_LAYER(VKBASALT_post_processing_64),
    MAGMA_LAYER(VKBASALT_PostProcess32),
    MAGMA_LAYER(VKBASALT_PostProcess64),

    MAGMA_LAYER(bandicam_helper),
    MAGMA_LAYER(com_xsplit_www_xbc_64_1),
    MAGMA_LAYER(DxtoryHookHelper),
    MAGMA_LAYER(EOS_Overlay),
    MAGMA_LAYER(FCAT_DT_overlay64),
    MAGMA_LAYER(FLIMES),
    MAGMA_LAYER(force_priority),
    MAGMA_LAYER(fossilize),
    MAGMA_LAYER_UNPREFIXED(MIRILLIS_LAYER),
    MAGMA_LAYER(RTSS),
    MAGMA_LAYER(XGCV4_64),
    MAGMA_LAYER(XSPLIT_64),
    MAGMA_LAYER_UNPREFIXED(VirtualSwapchain),
    MAGMA_LAYER(vkpipeline_db),
    vkdto_x86_64(MAGMA_QUERY_AVAILABILITY("vkdto x86_64")),
    MAGMA_LAYER(window_system_integration)
{}
} // namespace magma
