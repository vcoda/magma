/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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

#define MAGMA_CHECK_LAYER(ext) ext(hasLayer(#ext))

namespace magma
{
InstanceLayers::InstanceLayers():
    Layers(Instance::enumerateLayers()),
    MAGMA_CHECK_LAYER(AMD_switchable_graphics),
    MAGMA_CHECK_LAYER(bandicam_helper),
    MAGMA_CHECK_LAYER(DxtoryHookHelper),
    MAGMA_CHECK_LAYER(fpsmon),
    MAGMA_CHECK_LAYER(GOOGLE_threading),
    MAGMA_CHECK_LAYER(GOOGLE_unique_objects),
    MAGMA_CHECK_LAYER(KHRONOS_validation),
    MAGMA_CHECK_LAYER(LUNARG_api_dump),
    MAGMA_CHECK_LAYER(LUNARG_assistant_layer),
    MAGMA_CHECK_LAYER(LUNARG_core_validation),
    MAGMA_CHECK_LAYER(LUNARG_demo_layer),
    MAGMA_CHECK_LAYER(LUNARG_device_simulation),
    MAGMA_CHECK_LAYER(LUNARG_image),
    MAGMA_CHECK_LAYER(LUNARG_monitor),
    MAGMA_CHECK_LAYER(LUNARG_object_tracker),
    MAGMA_CHECK_LAYER(LUNARG_override),
    MAGMA_CHECK_LAYER(LUNARG_parameter_validation),
    MAGMA_CHECK_LAYER(LUNARG_screenshot),
    MAGMA_CHECK_LAYER(LUNARG_standard_validation),
    MAGMA_CHECK_LAYER(LUNARG_starter_layer),
    MAGMA_CHECK_LAYER(LUNARG_swapchain),
    MAGMA_CHECK_LAYER(LUNARG_vktrace),
    MAGMA_CHECK_LAYER(MESA_overlay),
    MAGMA_CHECK_LAYER(NV_nomad),
    MAGMA_CHECK_LAYER(NV_nomad_release_public_2019_3_0),
    MAGMA_CHECK_LAYER(NV_nomad_release_public_2019_3_1),
    MAGMA_CHECK_LAYER(NV_nomad_release_public_2019_4_0),
    MAGMA_CHECK_LAYER(NV_nsight),
    MAGMA_CHECK_LAYER(NV_nsight_sys),
    MAGMA_CHECK_LAYER(NV_optimus),
    MAGMA_CHECK_LAYER(PRIMUS_PrimusVK),
    MAGMA_CHECK_LAYER(RENDERDOC_Capture),
    MAGMA_CHECK_LAYER(RGA_pipeline_extraction),
    MAGMA_CHECK_LAYER(RTSS),
    MAGMA_CHECK_LAYER(VALVE_steam_fossilize),
    MAGMA_CHECK_LAYER(VALVE_steam_fossilize_32),
    MAGMA_CHECK_LAYER(VALVE_steam_fossilize_64),
    MAGMA_CHECK_LAYER(VALVE_steam_overlay),
    MAGMA_CHECK_LAYER(VALVE_steam_overlay_32),
    MAGMA_CHECK_LAYER(VALVE_steam_overlay_64)
{}
} // namespace magma
