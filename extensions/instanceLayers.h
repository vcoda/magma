/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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

namespace magma
{
    class Layers
    {
    public:
        bool hasLayer(const char *name) const noexcept;

    protected:
        Layers(const std::vector<VkLayerProperties>&);

    private:
        std::map<std::string, uint32_t> layers;
    };

    /* https://vulkan.gpuinfo.org/listinstancelayers.php */

    class InstanceLayers : public Layers
    {
    public:
        const bool AMD_switchable_graphics;
        const bool bandicam_helper;
        const bool DxtoryHookHelper;
        const bool fpsmon;
        const bool GOOGLE_threading;
        const bool GOOGLE_unique_objects;
        const bool KHRONOS_validation;
        const bool LUNARG_api_dump;
        const bool LUNARG_assistant_layer;
        const bool LUNARG_core_validation;
        const bool LUNARG_demo_layer;
        const bool LUNARG_device_simulation;
        const bool LUNARG_image;
        const bool LUNARG_monitor;
        const bool LUNARG_object_tracker;
        const bool LUNARG_override;
        const bool LUNARG_parameter_validation;
        const bool LUNARG_screenshot;
        const bool LUNARG_standard_validation;
        const bool LUNARG_starter_layer;
        const bool LUNARG_swapchain;
        const bool LUNARG_vktrace;
        const bool MESA_overlay;
        const bool NV_nomad;
        const bool NV_nomad_release_public_2019_3_0;
        const bool NV_nomad_release_public_2019_3_1;
        const bool NV_nomad_release_public_2019_4_0;
        const bool NV_nsight;
        const bool NV_nsight_sys;
        const bool NV_optimus;
        const bool PRIMUS_PrimusVK;
        const bool RENDERDOC_Capture;
        const bool RGA_pipeline_extraction;
        const bool RTSS;
        const bool VALVE_steam_fossilize;
        const bool VALVE_steam_fossilize_32;
        const bool VALVE_steam_fossilize_64;
        const bool VALVE_steam_overlay;
        const bool VALVE_steam_overlay_32;
        const bool VALVE_steam_overlay_64;

        InstanceLayers();
    };
} // namespace magma
