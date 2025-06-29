/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "extensionRegistry.h"

namespace magma
{
    /* https://vulkan.gpuinfo.org/listinstancelayers.php
       Keep list of layers in alphabetical order! */

    class InstanceLayers : public ExtensionRegistry<VkLayerProperties>
    {
    public:
        // https://github.com/aejsmith/vkdevicechooser/
        const VkBool32 AEJS_DeviceChooserLayer: 1;
        // Advanced Micro Devices, Inc.
        const VkBool32 AMD_switchable_graphics: 1;
        const VkBool32 AMD_switchable_graphics_32: 1;
        const VkBool32 AMD_switchable_graphics_64: 1;
        // GOG.com
        const VkBool32 Galaxy_Overlay: 1;
        const VkBool32 Galaxy_Overlay_DEBUG: 1;
        const VkBool32 Galaxy_Overlay_VERBOSE: 1;
        const VkBool32 GalaxyOverlayVkLayer: 1;
        const VkBool32 GalaxyOverlayVkLayer_DEBUG: 1;
        const VkBool32 GalaxyOverlayVkLayer_VERBOSE: 1;
        // GAPID: Graphics API Debugger https://github.com/google/gapid
        const VkBool32 GraphicsSpy: 1;
        // FPS Monitor - hardware in-game & desktop overlays
        const VkBool32 fpsmon: 1;
        const VkBool32 FPSMonitor: 1;
        // Google LLC
        const VkBool32 GOOGLE_threading: 1;
        const VkBool32 GOOGLE_unique_objects: 1;
        // GShade https://www.gshade.org/
        const VkBool32 gshade: 1;
        // Intel Corporation
        const VkBool32 INTEL_nullhw	: 1;
        const VkBool32 INTEL_state_tracker: 1;
        // https://github.com/KhronosGroup/Vulkan-ExtensionLayer
        const VkBool32 KHRONOS_memory_decompression: 1;
        const VkBool32 KHRONOS_profiles: 1;
        const VkBool32 KHRONOS_shader_object: 1;
        const VkBool32 KHRONOS_synchronization2: 1;
        const VkBool32 KHRONOS_timeline_semaphore: 1;
        const VkBool32 KHRONOS_validation: 1;
        // LunarG, Inc.
        const VkBool32 LUNARG_api_dump: 1;
        const VkBool32 LUNARG_assistant_layer: 1;
        const VkBool32 LUNARG_core_validation: 1;
        const VkBool32 LUNARG_crash_diagnostic: 1;
        const VkBool32 LUNARG_demo_layer: 1;
        const VkBool32 LUNARG_device_profile_api: 1;
        const VkBool32 LUNARG_device_simulation: 1;
        const VkBool32 LUNARG_gfxreconstruct: 1;
        const VkBool32 LUNARG_image: 1;
        const VkBool32 LUNARG_monitor: 1;
        const VkBool32 LUNARG_object_tracker: 1;
        const VkBool32 LUNARG_overlay: 1;
        const VkBool32 LUNARG_override: 1;
        const VkBool32 LUNARG_parameter_validation: 1;
        const VkBool32 LUNARG_screenshot: 1;
        const VkBool32 LUNARG_standard_validation: 1;
        const VkBool32 LUNARG_starter_layer: 1;
        const VkBool32 LUNARG_swapchain: 1;
        const VkBool32 LUNARG_TestingIntercept: 1;
        const VkBool32 LUNARG_VkGHL32: 1;
        const VkBool32 LUNARG_VkGHL64: 1;
        const VkBool32 LUNARG_vktrace: 1;
        const VkBool32 LUNARG_xenviro_layer: 1;
        // MangoHud Vulkan overlay
        const VkBool32 MangoHud: 1;
        const VkBool32 MANGOAPP_overlay: 1;
        const VkBool32 MANGOHUD32_overlay: 1;
        const VkBool32 MANGOHUD_overlay: 1;
        const VkBool32 MANGOHUD_overlay_32: 1;
        const VkBool32 MANGOHUD_overlay_32_x86: 1;
        const VkBool32 MANGOHUD_overlay_64_x86_64: 1;
        const VkBool32 MANGOHUD_overlay_aarch64: 1;
        const VkBool32 MANGOHUD_overlay_x32: 1;
        const VkBool32 MANGOHUD_overlay_x32_x86_64: 1;
        const VkBool32 MANGOHUD_overlay_x86: 1;
        const VkBool32 MANGOHUD_overlay_x86_64: 1;
        // MoltenVK
        const VkBool32 MoltenVK: 1;
        // The Mesa 3D Graphics Library
        const VkBool32 MESA_device_select: 1;
        const VkBool32 MESA_overlay: 1;
        // NVIDIA Corporation
        const VkBool32 NV_nomad: 1;
        const VkBool32 NV_nsight: 1;
        const VkBool32 NV_nsight_sys: 1;
        const VkBool32 NV_optimus: 1;
        const VkBool32 NV_vgd: 1;
        const VkBool32 Nvidia_Shader_Dump: 1;
        // OBS Studio https://obsproject.com/
        const VkBool32 OBS_HOOK: 1;
        const VkBool32 OBS_vkcapture_32: 1;
        const VkBool32 OBS_vkcapture_64: 1;
        const VkBool32 OW_OBS_HOOK: 1;
        const VkBool32 OW_OVERLAY: 1;
        // PlayClaw https://playclaw.ru/
        const VkBool32 playclaw: 1;
        // Primus for Vulkan https://github.com/felixdoerre/primus_vk
        const VkBool32 PRIMUS_PrimusVK: 1;
        // RenderDoc https://renderdoc.org/
        const VkBool32 RENDERDOC_ARM_Capture: 1;
        const VkBool32 RENDERDOC_Capture: 1;
        const VkBool32 RENDERDOC_Capture_32: 1;
        // ReShade https://reshade.me/
        const VkBool32 reshade: 1;
        const VkBool32 reshade32: 1;
        // Radeon GPU Analyzer
        const VkBool32 RGA_pipeline_extraction: 1;
        const VkBool32 libVK_LAYER_RGA_pipeline_extraction: 1;
        // Rockstar Games, Inc
        const VkBool32 ROCKSTAR_GAMES_social_club: 1;
        // Valve Corporation
        const VkBool32 VALVE_steam_fossilize: 1;
        const VkBool32 VALVE_steam_fossilize_32: 1;
        const VkBool32 VALVE_steam_fossilize_64: 1;
        const VkBool32 VALVE_steam_overlay: 1;
        const VkBool32 VALVE_steam_overlay_32: 1;
        const VkBool32 VALVE_steam_overlay_64: 1;
        // https://github.com/DadSchoorse/vkBasalt
        const VkBool32 VKBASALT_post_processing: 1;
        const VkBool32 VKBASALT_post_processing_32: 1;
        const VkBool32 VKBASALT_post_processing_64: 1;
        const VkBool32 VKBASALT_PostProcess32: 1;
        const VkBool32 VKBASALT_PostProcess64: 1;
        // Misc
        const VkBool32 bandicam_helper: 1;
        const VkBool32 com_xsplit_www_xbc_64_1: 1;
        const VkBool32 DxtoryHookHelper: 1;
        const VkBool32 EOS_Overlay: 1;
        const VkBool32 FCAT_DT_overlay64: 1;
        const VkBool32 FLIMES: 1;
        const VkBool32 force_priority: 1;
        const VkBool32 fossilize: 1;
        const VkBool32 MIRILLIS_LAYER: 1;
        const VkBool32 RTSS: 1;
        const VkBool32 XGCV4_64: 1;
        const VkBool32 XSPLIT_64: 1;
        const VkBool32 VirtualSwapchain: 1;
        const VkBool32 vkpipeline_db: 1;
        const VkBool32 vkdto_x86_64: 1;
        const VkBool32 window_system_integration: 1;

        InstanceLayers();
    };

    MAGMA_TYPEDEF_MANAGED_PTR(InstanceLayers)
} // namespace magma
