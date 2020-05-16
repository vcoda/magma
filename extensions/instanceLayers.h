/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
        MAGMA_LAYER(AMD_switchable_graphics)

        MAGMA_LAYER(bandicam_helper)
        MAGMA_LAYER(DxtoryHookHelper)
        MAGMA_LAYER(fpsmon)

        MAGMA_LAYER(GOOGLE_threading)
        MAGMA_LAYER(GOOGLE_unique_objects)

        MAGMA_LAYER(KHRONOS_timeline_semaphore)
        MAGMA_LAYER(KHRONOS_validation)

        MAGMA_LAYER(LUNARG_api_dump)
        MAGMA_LAYER(LUNARG_assistant_layer)
        MAGMA_LAYER(LUNARG_core_validation)
        MAGMA_LAYER(LUNARG_demo_layer)
        MAGMA_LAYER(LUNARG_device_simulation)
        MAGMA_LAYER(LUNARG_image)
        MAGMA_LAYER(LUNARG_monitor)
        MAGMA_LAYER(LUNARG_object_tracker)
        MAGMA_LAYER(LUNARG_override)
        MAGMA_LAYER(LUNARG_parameter_validation)
        MAGMA_LAYER(LUNARG_screenshot)
        MAGMA_LAYER(LUNARG_standard_validation)
        MAGMA_LAYER(LUNARG_starter_layer)
        MAGMA_LAYER(LUNARG_swapchain)
        MAGMA_LAYER(LUNARG_vktrace)

        MAGMA_LAYER(MESA_overlay)

        MAGMA_LAYER(NV_nomad)
        MAGMA_LAYER(NV_nsight)
        MAGMA_LAYER(NV_nsight_sys)
        MAGMA_LAYER(NV_optimus)
        MAGMA_LAYER(NV_vgd)

        MAGMA_LAYER(PRIMUS_PrimusVK)
        MAGMA_LAYER(RENDERDOC_Capture)
        MAGMA_LAYER(reshade)
        MAGMA_LAYER(RGA_pipeline_extraction)
        MAGMA_LAYER(RTSS)

        MAGMA_LAYER(VALVE_steam_fossilize)
        MAGMA_LAYER(VALVE_steam_fossilize_32)
        MAGMA_LAYER(VALVE_steam_fossilize_64)
        MAGMA_LAYER(VALVE_steam_overlay)
        MAGMA_LAYER(VALVE_steam_overlay_32)
        MAGMA_LAYER(VALVE_steam_overlay_64)

        MAGMA_LAYER(VKBASALT_PostProcess32)
        MAGMA_LAYER(VKBASALT_PostProcess64)

        MAGMA_LAYER(vkpipeline_db)

        InstanceLayers();
    };
} // namespace magma
