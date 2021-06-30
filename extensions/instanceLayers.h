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
#include "layers.h"

namespace magma
{
    /* https://vulkan.gpuinfo.org/listinstancelayers.php
       Keep layers list in alphabetical order. */

    class InstanceLayers : public Layers
    {
    public:
        const bool GalaxyOverlayVkLayer;
        const bool GalaxyOverlayVkLayer_DEBUG;
        const bool GalaxyOverlayVkLayer_VERBOSE;

        const bool GraphicsSpy;
        const bool libVK_LAYER_RGA_pipeline_extraction;
        const bool MoltenVK;
        const bool VirtualSwapchain;

        const bool AMD_switchable_graphics;
        const bool AMD_switchable_graphics_32;
        const bool AMD_switchable_graphics_64;

        const bool bandicam_helper;
        const bool DxtoryHookHelper;
        const bool fossilize;
        const bool fpsmon;

        const bool GOOGLE_threading;
        const bool GOOGLE_unique_objects;

        // https://github.com/KhronosGroup/Vulkan-ExtensionLayer
        const bool KHRONOS_synchronization2;
        const bool KHRONOS_timeline_semaphore;
        const bool KHRONOS_validation;

        const bool LUNARG_api_dump;
        const bool LUNARG_assistant_layer;
        const bool LUNARG_core_validation;
        const bool LUNARG_demo_layer;
        const bool LUNARG_device_profile_api;
        const bool LUNARG_device_simulation;
        const bool LUNARG_gfxreconstruct;
        const bool LUNARG_image;
        const bool LUNARG_monitor;
        const bool LUNARG_object_tracker;
        const bool LUNARG_overlay;
        const bool LUNARG_override;
        const bool LUNARG_parameter_validation;
        const bool LUNARG_screenshot;
        const bool LUNARG_standard_validation;
        const bool LUNARG_starter_layer;
        const bool LUNARG_swapchain;
        const bool LUNARG_TestingIntercept;
        const bool LUNARG_VkGHL32;
        const bool LUNARG_VkGHL64;
        const bool LUNARG_vktrace;

        const bool MESA_device_select;
        const bool MESA_overlay;

        const bool NV_nomad;
        const bool NV_nsight;
        const bool NV_nsight_sys;
        const bool NV_optimus;
        const bool NV_vgd;

        const bool PRIMUS_PrimusVK;
        const bool RENDERDOC_Capture;
        const bool reshade;
        const bool RGA_pipeline_extraction;
        const bool RTSS;

        const bool VALVE_steam_fossilize;
        const bool VALVE_steam_fossilize_32;
        const bool VALVE_steam_fossilize_64;
        const bool VALVE_steam_overlay;
        const bool VALVE_steam_overlay_32;
        const bool VALVE_steam_overlay_64;

        const bool VKBASALT_post_processing;
        const bool VKBASALT_PostProcess32;
        const bool VKBASALT_PostProcess64;

        const bool vkpipeline_db;

        const bool XGCV4_64;
        const bool XSPLIT_64;

        InstanceLayers();
    };

    MAGMA_TYPEDEF_UNIQUE_PTR(InstanceLayers)
} // namespace magma
