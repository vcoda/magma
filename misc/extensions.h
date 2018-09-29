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
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../vulkan.h"

namespace magma
{
    class Instance;
    class PhysicalDevice;

    /* https://www.khronos.org/registry/vulkan/ */

    class Extensions
    {
    public:
        const std::map<std::string, uint32_t>& getExtensions() const noexcept
            { return extensions; }
        bool hasExtension(const char *name) const noexcept;

    protected:
        Extensions(const std::vector<VkExtensionProperties>& extensions);

    protected:
        std::map<std::string, uint32_t> extensions;
    };

    struct InstanceExtensions : Extensions
    {
        const bool EXT_debug_report;

        const bool KHR_display;
        const bool KHR_external_fence_capabilities;
        const bool KHR_external_memory_capabilities;
        const bool KHR_external_semaphore_capabilities;
        const bool KHR_get_physical_device_properties2;
        const bool KHR_get_surface_capabilities2;

        // Window system integration
        const bool KHR_surface;
        const bool KHR_android_surface;
        const bool KHR_mir_surface;
        const bool KHR_xcb_surface;
        const bool KHR_xlib_surface;
        const bool KHR_wayland_surface;
        const bool KHR_win32_surface;

        const bool MVK_ios_surface;
        const bool MVK_macos_surface;

        InstanceExtensions(std::shared_ptr<const Instance> instance);
    };

    struct PhysicalDeviceExtensions : Extensions
    {   // Keep extensions list in alphabetical order
        const bool AMD_buffer_marker;
        const bool AMD_calibrated_timestamps;
        const bool AMD_draw_indirect_count;
        const bool AMD_gcn_shader;
        const bool AMD_gpa_interface;
        const bool AMD_gpu_shader_half_float;
        const bool AMD_gpu_shader_int16;
        const bool AMD_mixed_attachment_samples;
        const bool AMD_negative_viewport_height;
        const bool AMD_rasterization_order;
        const bool AMD_shader_ballot;
        const bool AMD_shader_core_properties;
        const bool AMD_shader_explicit_vertex_parameter;
        const bool AMD_shader_fragment_mask;
        const bool AMD_shader_image_load_store_lod;
        const bool AMD_shader_info;
        const bool AMD_shader_trinary_minmax;
        const bool AMD_texture_gather_bias_lod;
        const bool AMD_wave_limits;

        const bool EXT_acquire_xlib_display;
        const bool EXT_blend_operation_advanced;
        const bool EXT_conservative_rasterization;
        const bool EXT_debug_marker;
        const bool EXT_depth_range_unrestricted;
        const bool EXT_direct_mode_display;
        const bool EXT_discard_rectangles;
        const bool EXT_display_control;
        const bool EXT_display_surface_counter;
        const bool EXT_external_memory_dma_buf;
        const bool EXT_external_memory_host;
        const bool EXT_global_priority;
        const bool EXT_hdr_metadata;
        const bool EXT_post_depth_coverage;
        const bool EXT_queue_family_foreign;
        const bool EXT_sample_locations;
        const bool EXT_sampler_filter_minmax;
        const bool EXT_shader_stencil_export;
        const bool EXT_shader_subgroup_ballot;
        const bool EXT_shader_subgroup_vote;
        const bool EXT_shader_viewport_index_layer;
        const bool EXT_swapchain_colorspace;
        const bool EXT_validation_cache;
        const bool EXT_validation_flags;

        const bool IMG_filter_cubic;
        const bool IMG_format_pvrtc;

        const bool KHR_dedicated_allocation;
        const bool KHR_maintenance1;
        const bool KHR_maintenance2;
        const bool KHR_maintenance3;
        const bool KHR_shader_draw_parameters;

        const bool NV_clip_space_w_scaling;
        const bool NV_dedicated_allocation;
        const bool NV_external_memory;
        const bool NV_external_memory_capabilities;
        const bool NV_external_memory_win32;
        const bool NV_fill_rectangle;
        const bool NV_fragment_coverage_to_color;
        const bool NV_framebuffer_mixed_samples;
        const bool NV_geometry_shader_passthrough;
        const bool NV_glsl_shader;
        const bool NV_sample_mask_override_coverage;
        const bool NV_viewport_array2;
        const bool NV_viewport_swizzle;
        const bool NV_win32_keyed_mutex;

        PhysicalDeviceExtensions(std::shared_ptr<const PhysicalDevice> device);
    };
} // namespace magma
