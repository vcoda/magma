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
#include "extensions.h"
#include "../objects/instance.h"
#include "../objects/physicalDevice.h"

#define MAGMA_CHECK_EXTENSION(ext) ext(hasExtension(#ext))

namespace magma
{
bool Extensions::hasExtension(const char *name) const
{
    static const std::string prefix("VK_");
    const auto it = extensions.find(prefix + name);
    return it != extensions.end();
}

InstanceExtensions::InstanceExtensions(std::shared_ptr<const Instance> instance):
    Extensions(instance->enumerateExtensions()),

    MAGMA_CHECK_EXTENSION(EXT_debug_report),

    MAGMA_CHECK_EXTENSION(KHR_android_surface),
    MAGMA_CHECK_EXTENSION(KHR_display),
    MAGMA_CHECK_EXTENSION(KHR_external_fence_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_external_memory_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_external_semaphore_capabilities),
    MAGMA_CHECK_EXTENSION(KHR_get_physical_device_properties2),
    MAGMA_CHECK_EXTENSION(KHR_get_surface_capabilities2),

    MAGMA_CHECK_EXTENSION(KHR_surface),
    MAGMA_CHECK_EXTENSION(KHR_mir_surface),
    MAGMA_CHECK_EXTENSION(KHR_xcb_surface),
    MAGMA_CHECK_EXTENSION(KHR_xlib_surface),
    MAGMA_CHECK_EXTENSION(KHR_wayland_surface),
    MAGMA_CHECK_EXTENSION(KHR_win32_surface)
{}   

DeviceExtensions::DeviceExtensions(std::shared_ptr<const PhysicalDevice> device) :
    Extensions(device->enumerateExtensions()),

    MAGMA_CHECK_EXTENSION(AMD_draw_indirect_count),
    MAGMA_CHECK_EXTENSION(AMD_gcn_shader),
    MAGMA_CHECK_EXTENSION(AMD_gpa_interface),
    MAGMA_CHECK_EXTENSION(AMD_gpu_shader_half_float),
    MAGMA_CHECK_EXTENSION(AMD_gpu_shader_int16),
    MAGMA_CHECK_EXTENSION(AMD_mixed_attachment_samples),
    MAGMA_CHECK_EXTENSION(AMD_negative_viewport_height),
    MAGMA_CHECK_EXTENSION(AMD_rasterization_order),
    MAGMA_CHECK_EXTENSION(AMD_shader_ballot),
    MAGMA_CHECK_EXTENSION(AMD_shader_explicit_vertex_parameter),
    MAGMA_CHECK_EXTENSION(AMD_shader_fragment_mask),
    MAGMA_CHECK_EXTENSION(AMD_shader_image_load_store_lod),
    MAGMA_CHECK_EXTENSION(AMD_shader_info),
    MAGMA_CHECK_EXTENSION(AMD_shader_trinary_minmax),
    MAGMA_CHECK_EXTENSION(AMD_texture_gather_bias_lod),

    MAGMA_CHECK_EXTENSION(EXT_acquire_xlib_display),
    MAGMA_CHECK_EXTENSION(EXT_blend_operation_advanced),
    MAGMA_CHECK_EXTENSION(EXT_conservative_rasterization),
    MAGMA_CHECK_EXTENSION(EXT_debug_marker),
    MAGMA_CHECK_EXTENSION(EXT_depth_range_unrestricted),
    MAGMA_CHECK_EXTENSION(EXT_direct_mode_display),
    MAGMA_CHECK_EXTENSION(EXT_discard_rectangles),
    MAGMA_CHECK_EXTENSION(EXT_display_control),
    MAGMA_CHECK_EXTENSION(EXT_display_surface_counter),
    MAGMA_CHECK_EXTENSION(EXT_external_memory_dma_buf),
    MAGMA_CHECK_EXTENSION(EXT_external_memory_host),
    MAGMA_CHECK_EXTENSION(EXT_global_priority),
    MAGMA_CHECK_EXTENSION(EXT_hdr_metadata),
    MAGMA_CHECK_EXTENSION(EXT_post_depth_coverage),
    MAGMA_CHECK_EXTENSION(EXT_queue_family_foreign),
    MAGMA_CHECK_EXTENSION(EXT_sample_locations),
    MAGMA_CHECK_EXTENSION(EXT_sampler_filter_minmax),
    MAGMA_CHECK_EXTENSION(EXT_shader_stencil_export),
    MAGMA_CHECK_EXTENSION(EXT_shader_subgroup_ballot),
    MAGMA_CHECK_EXTENSION(EXT_shader_subgroup_vote),
    MAGMA_CHECK_EXTENSION(EXT_shader_viewport_index_layer),
    MAGMA_CHECK_EXTENSION(EXT_swapchain_colorspace),
    MAGMA_CHECK_EXTENSION(EXT_validation_cache),
    MAGMA_CHECK_EXTENSION(EXT_validation_flags),

    MAGMA_CHECK_EXTENSION(IMG_filter_cubic),
    MAGMA_CHECK_EXTENSION(IMG_format_pvrtc),

    MAGMA_CHECK_EXTENSION(KHR_maintenance1),
    MAGMA_CHECK_EXTENSION(KHR_maintenance2),
    MAGMA_CHECK_EXTENSION(KHR_shader_draw_parameters),

    MAGMA_CHECK_EXTENSION(NV_clip_space_w_scaling),
    MAGMA_CHECK_EXTENSION(NV_dedicated_allocation),
    MAGMA_CHECK_EXTENSION(NV_external_memory),
    MAGMA_CHECK_EXTENSION(NV_external_memory_capabilities),
    MAGMA_CHECK_EXTENSION(NV_external_memory_win32),
    MAGMA_CHECK_EXTENSION(NV_fill_rectangle),
    MAGMA_CHECK_EXTENSION(NV_fragment_coverage_to_color),
    MAGMA_CHECK_EXTENSION(NV_framebuffer_mixed_samples),
    MAGMA_CHECK_EXTENSION(NV_geometry_shader_passthrough),
    MAGMA_CHECK_EXTENSION(NV_glsl_shader),
    MAGMA_CHECK_EXTENSION(NV_sample_mask_override_coverage),
    MAGMA_CHECK_EXTENSION(NV_viewport_array2),
    MAGMA_CHECK_EXTENSION(NV_viewport_swizzle),
    MAGMA_CHECK_EXTENSION(NV_win32_keyed_mutex)
{}
} // namespace magma
