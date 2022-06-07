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
#include "layers.h"

namespace magma
{
    /* https://vulkan.gpuinfo.org/listinstancelayers.php
       Keep layers list in alphabetical order. */

    class InstanceLayers : public Layers
    {
    public:
        const VkBool32 GalaxyOverlayVkLayer : 1;
        const VkBool32 GalaxyOverlayVkLayer_DEBUG : 1;
        const VkBool32 GalaxyOverlayVkLayer_VERBOSE : 1;

        const VkBool32 GraphicsSpy : 1;
        const VkBool32 libVK_LAYER_RGA_pipeline_extraction : 1;
        const VkBool32 MoltenVK : 1;
        const VkBool32 VirtualSwapchain : 1;

        const VkBool32 AMD_switchable_graphics : 1;
        const VkBool32 AMD_switchable_graphics_32 : 1;
        const VkBool32 AMD_switchable_graphics_64 : 1;

        const VkBool32 bandicam_helper : 1;
        const VkBool32 DxtoryHookHelper : 1;
        const VkBool32 fossilize : 1;
        const VkBool32 fpsmon : 1;

        const VkBool32 GOOGLE_threading : 1;
        const VkBool32 GOOGLE_unique_objects : 1;

        // https://github.com/KhronosGroup/Vulkan-ExtensionLayer
        const VkBool32 KHRONOS_synchronization2 : 1;
        const VkBool32 KHRONOS_timeline_semaphore : 1;
        const VkBool32 KHRONOS_validation : 1;

        const VkBool32 LUNARG_api_dump : 1;
        const VkBool32 LUNARG_assistant_layer : 1;
        const VkBool32 LUNARG_core_validation : 1;
        const VkBool32 LUNARG_demo_layer : 1;
        const VkBool32 LUNARG_device_profile_api : 1;
        const VkBool32 LUNARG_device_simulation : 1;
        const VkBool32 LUNARG_gfxreconstruct : 1;
        const VkBool32 LUNARG_image : 1;
        const VkBool32 LUNARG_monitor : 1;
        const VkBool32 LUNARG_object_tracker : 1;
        const VkBool32 LUNARG_overlay : 1;
        const VkBool32 LUNARG_override : 1;
        const VkBool32 LUNARG_parameter_validation : 1;
        const VkBool32 LUNARG_screenshot : 1;
        const VkBool32 LUNARG_standard_validation : 1;
        const VkBool32 LUNARG_starter_layer : 1;
        const VkBool32 LUNARG_swapchain : 1;
        const VkBool32 LUNARG_TestingIntercept : 1;
        const VkBool32 LUNARG_VkGHL32 : 1;
        const VkBool32 LUNARG_VkGHL64 : 1;
        const VkBool32 LUNARG_vktrace : 1;

        const VkBool32 MESA_device_select : 1;
        const VkBool32 MESA_overlay : 1;

        const VkBool32 NV_nomad : 1;
        const VkBool32 NV_nsight : 1;
        const VkBool32 NV_nsight_sys : 1;
        const VkBool32 NV_optimus : 1;
        const VkBool32 NV_vgd : 1;

        const VkBool32 PRIMUS_PrimusVK : 1;
        const VkBool32 RENDERDOC_Capture : 1;
        const VkBool32 reshade : 1;
        const VkBool32 RGA_pipeline_extraction : 1;
        const VkBool32 RTSS : 1;

        const VkBool32 VALVE_steam_fossilize : 1;
        const VkBool32 VALVE_steam_fossilize_32 : 1;
        const VkBool32 VALVE_steam_fossilize_64 : 1;
        const VkBool32 VALVE_steam_overlay : 1;
        const VkBool32 VALVE_steam_overlay_32 : 1;
        const VkBool32 VALVE_steam_overlay_64 : 1;

        const VkBool32 VKBASALT_post_processing : 1;
        const VkBool32 VKBASALT_PostProcess32 : 1;
        const VkBool32 VKBASALT_PostProcess64 : 1;

        const VkBool32 vkpipeline_db : 1;

        const VkBool32 XGCV4_64 : 1;
        const VkBool32 XSPLIT_64 : 1;

        InstanceLayers();
    };

    MAGMA_TYPEDEF_UNIQUE_PTR(InstanceLayers)
} // namespace magma
