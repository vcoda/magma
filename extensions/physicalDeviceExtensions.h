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
#include "extensions.h"

namespace magma
{
    class PhysicalDevice;

    /* Functionality that is restricted to the device-level must be implemented
       as a device extension. For the list of extensions see:
       https://www.khronos.org/registry/vulkan/
       Keep extensions list in alphabetical order. */

    class PhysicalDeviceExtensions : public Extensions
    {
    public:
        MAGMA_EXTENSION(AMD_buffer_marker)
        MAGMA_EXTENSION(AMD_device_coherent_memory)
        MAGMA_EXTENSION(AMD_display_native_hdr)
        MAGMA_EXTENSION(AMD_draw_indirect_count)
        MAGMA_EXTENSION(AMD_gcn_shader)
        MAGMA_EXTENSION(AMD_gpu_shader_half_float)
        MAGMA_EXTENSION(AMD_gpu_shader_int16)
        MAGMA_EXTENSION(AMD_memory_overallocation_behavior)
        MAGMA_EXTENSION(AMD_mixed_attachment_samples)
        MAGMA_EXTENSION(AMD_negative_viewport_height)
        MAGMA_EXTENSION(AMD_pipeline_compiler_control)
        MAGMA_EXTENSION(AMD_rasterization_order)
        MAGMA_EXTENSION(AMD_shader_ballot)
        MAGMA_EXTENSION(AMD_shader_core_properties)
        MAGMA_EXTENSION(AMD_shader_core_properties2)
        MAGMA_EXTENSION(AMD_shader_explicit_vertex_parameter)
        MAGMA_EXTENSION(AMD_shader_fragment_mask)
        MAGMA_EXTENSION(AMD_shader_image_load_store_lod)
        MAGMA_EXTENSION(AMD_shader_info)
        MAGMA_EXTENSION(AMD_shader_trinary_minmax)
        MAGMA_EXTENSION(AMD_texture_gather_bias_lod)

        MAGMA_EXTENSION(ANDROID_external_memory_android_hardware_buffer)

        MAGMA_EXTENSION(EXT_astc_decode_mode)
        MAGMA_EXTENSION(EXT_blend_operation_advanced)
        MAGMA_EXTENSION(EXT_buffer_device_address)
        MAGMA_EXTENSION(EXT_calibrated_timestamps)
        MAGMA_EXTENSION(EXT_conditional_rendering)
        MAGMA_EXTENSION(EXT_conservative_rasterization)
        MAGMA_EXTENSION(EXT_debug_marker)
        MAGMA_EXTENSION(EXT_depth_clip_enable)
        MAGMA_EXTENSION(EXT_depth_range_unrestricted)
        MAGMA_EXTENSION(EXT_descriptor_indexing)
        MAGMA_EXTENSION(EXT_discard_rectangles)
        MAGMA_EXTENSION(EXT_display_control)
        MAGMA_EXTENSION(EXT_display_surface_counter)
        MAGMA_EXTENSION(EXT_external_memory_dma_buf)
        MAGMA_EXTENSION(EXT_external_memory_host)
        MAGMA_EXTENSION(EXT_filter_cubic)
        MAGMA_EXTENSION(EXT_fragment_density_map)
        MAGMA_EXTENSION(EXT_fragment_shader_interlock)
        MAGMA_EXTENSION(EXT_full_screen_exclusive)
        MAGMA_EXTENSION(EXT_global_priority)
        MAGMA_EXTENSION(EXT_hdr_metadata)
        MAGMA_EXTENSION(EXT_headless_surface)
        MAGMA_EXTENSION(EXT_host_query_reset)
        MAGMA_EXTENSION(EXT_image_drm_format_modifier)
        MAGMA_EXTENSION(EXT_index_type_uint8)
        MAGMA_EXTENSION(EXT_inline_uniform_block)
        MAGMA_EXTENSION(EXT_line_rasterization)
        MAGMA_EXTENSION(EXT_memory_budget)
        MAGMA_EXTENSION(EXT_memory_priority)
        MAGMA_EXTENSION(EXT_pci_bus_info)
        MAGMA_EXTENSION(EXT_pipeline_creation_cache_control)
        MAGMA_EXTENSION(EXT_pipeline_creation_feedback)
        MAGMA_EXTENSION(EXT_post_depth_coverage)
        MAGMA_EXTENSION(EXT_queue_family_foreign)
        MAGMA_EXTENSION(EXT_sample_locations)
        MAGMA_EXTENSION(EXT_sampler_filter_minmax)
        MAGMA_EXTENSION(EXT_scalar_block_layout)
        MAGMA_EXTENSION(EXT_separate_stencil_usage)
        MAGMA_EXTENSION(EXT_shader_demote_to_helper_invocation)
        MAGMA_EXTENSION(EXT_shader_stencil_export)
        MAGMA_EXTENSION(EXT_shader_subgroup_ballot)
        MAGMA_EXTENSION(EXT_shader_subgroup_vote)
        MAGMA_EXTENSION(EXT_shader_viewport_index_layer)
        MAGMA_EXTENSION(EXT_subgroup_size_control)
        MAGMA_EXTENSION(EXT_texel_buffer_alignment)
        MAGMA_EXTENSION(EXT_texture_compression_astc_hdr)
        MAGMA_EXTENSION(EXT_tooling_info)
        MAGMA_EXTENSION(EXT_transform_feedback)
        MAGMA_EXTENSION(EXT_validation_cache)
        MAGMA_EXTENSION(EXT_validation_features)
        MAGMA_EXTENSION(EXT_validation_flags)
        MAGMA_EXTENSION(EXT_vertex_attribute_divisor)
        MAGMA_EXTENSION(EXT_ycbcr_image_arrays)

        MAGMA_EXTENSION(GOOGLE_decorate_string)
        MAGMA_EXTENSION(GOOGLE_display_timing)
        MAGMA_EXTENSION(GOOGLE_hlsl_functionality1)
        MAGMA_EXTENSION(GOOGLE_user_type)

        MAGMA_EXTENSION(IMG_filter_cubic)
        MAGMA_EXTENSION(IMG_format_pvrtc)

        MAGMA_EXTENSION(INTEL_performance_query)
        MAGMA_EXTENSION(INTEL_shader_integer_functions2)

        MAGMA_EXTENSION(KHR_16bit_storage)
        MAGMA_EXTENSION(KHR_8bit_storage)
        MAGMA_EXTENSION(KHR_bind_memory2)
        MAGMA_EXTENSION(KHR_buffer_device_address)
        MAGMA_EXTENSION(KHR_create_renderpass2)
        MAGMA_EXTENSION(KHR_dedicated_allocation)
        MAGMA_EXTENSION(KHR_deferred_host_operations)
        MAGMA_EXTENSION(KHR_depth_stencil_resolve)
        MAGMA_EXTENSION(KHR_descriptor_update_template)
        MAGMA_EXTENSION(KHR_device_group)
        MAGMA_EXTENSION(KHR_display_swapchain)
        MAGMA_EXTENSION(KHR_draw_indirect_count)
        MAGMA_EXTENSION(KHR_driver_properties)
        MAGMA_EXTENSION(KHR_external_fence)
        MAGMA_EXTENSION(KHR_external_fence_fd)
        MAGMA_EXTENSION(KHR_external_fence_win32)
        MAGMA_EXTENSION(KHR_external_memory)
        MAGMA_EXTENSION(KHR_external_memory_fd)
        MAGMA_EXTENSION(KHR_external_memory_win32)
        MAGMA_EXTENSION(KHR_external_semaphore)
        MAGMA_EXTENSION(KHR_external_semaphore_capabilities)
        MAGMA_EXTENSION(KHR_external_semaphore_fd)
        MAGMA_EXTENSION(KHR_external_semaphore_win32)
        MAGMA_EXTENSION(KHR_get_memory_requirements2)
        MAGMA_EXTENSION(KHR_image_format_list)
        MAGMA_EXTENSION(KHR_imageless_framebuffer)
        MAGMA_EXTENSION(KHR_incremental_present)
        MAGMA_EXTENSION(KHR_maintenance1)
        MAGMA_EXTENSION(KHR_maintenance2)
        MAGMA_EXTENSION(KHR_maintenance3)
        MAGMA_EXTENSION(KHR_multiview)
        MAGMA_EXTENSION(KHR_performance_query)
        MAGMA_EXTENSION(KHR_pipeline_executable_properties)
        MAGMA_EXTENSION(KHR_pipeline_library)
        MAGMA_EXTENSION(KHR_push_descriptor)
        MAGMA_EXTENSION(KHR_ray_tracing)
        MAGMA_EXTENSION(KHR_relaxed_block_layout)
        MAGMA_EXTENSION(KHR_sampler_mirror_clamp_to_edge)
        MAGMA_EXTENSION(KHR_sampler_ycbcr_conversion)
        MAGMA_EXTENSION(KHR_separate_depth_stencil_layouts)
        MAGMA_EXTENSION(KHR_shader_atomic_int64)
        MAGMA_EXTENSION(KHR_shader_clock)
        MAGMA_EXTENSION(KHR_shader_draw_parameters)
        MAGMA_EXTENSION(KHR_shader_float16_int8)
        MAGMA_EXTENSION(KHR_shader_float_controls)
        MAGMA_EXTENSION(KHR_shader_non_semantic_info)
        MAGMA_EXTENSION(KHR_shader_subgroup_extended_types)
        MAGMA_EXTENSION(KHR_shared_presentable_image)
        MAGMA_EXTENSION(KHR_spirv_1_4)
        MAGMA_EXTENSION(KHR_storage_buffer_storage_class)
        MAGMA_EXTENSION(KHR_swapchain)
        MAGMA_EXTENSION(KHR_swapchain_mutable_format)
        MAGMA_EXTENSION(KHR_timeline_semaphore)
        MAGMA_EXTENSION(KHR_uniform_buffer_standard_layout)
        MAGMA_EXTENSION(KHR_variable_pointers)
        MAGMA_EXTENSION(KHR_vulkan_memory_model)
        MAGMA_EXTENSION(KHR_win32_keyed_mutex)

        MAGMA_EXTENSION(NVX_device_generated_commands)
        MAGMA_EXTENSION(NVX_multiview_per_view_attributes)
        MAGMA_EXTENSION(NV_clip_space_w_scaling)
        MAGMA_EXTENSION(NV_compute_shader_derivatives)
        MAGMA_EXTENSION(NV_cooperative_matrix)
        MAGMA_EXTENSION(NV_corner_sampled_image)
        MAGMA_EXTENSION(NV_coverage_reduction_mode)
        MAGMA_EXTENSION(NV_dedicated_allocation)
        MAGMA_EXTENSION(NV_dedicated_allocation_image_aliasing)
        MAGMA_EXTENSION(NV_device_diagnostic_checkpoints)
        MAGMA_EXTENSION(NV_device_diagnostics_config)
        MAGMA_EXTENSION(NV_device_generated_commands)
        MAGMA_EXTENSION(NV_external_memory)
        MAGMA_EXTENSION(NV_external_memory_capabilities)
        MAGMA_EXTENSION(NV_external_memory_win32)
        MAGMA_EXTENSION(NV_fill_rectangle)
        MAGMA_EXTENSION(NV_fragment_coverage_to_color)
        MAGMA_EXTENSION(NV_fragment_shader_barycentric)
        MAGMA_EXTENSION(NV_framebuffer_mixed_samples)
        MAGMA_EXTENSION(NV_geometry_shader_passthrough)
        MAGMA_EXTENSION(NV_glsl_shader)
        MAGMA_EXTENSION(NV_mesh_shader)
        MAGMA_EXTENSION(NV_ray_tracing)
        MAGMA_EXTENSION(NV_representative_fragment_test)
        MAGMA_EXTENSION(NV_sample_mask_override_coverage)
        MAGMA_EXTENSION(NV_scissor_exclusive)
        MAGMA_EXTENSION(NV_shader_image_footprint)
        MAGMA_EXTENSION(NV_shader_sm_builtins)
        MAGMA_EXTENSION(NV_shader_subgroup_partitioned)
        MAGMA_EXTENSION(NV_shading_rate_image)
        MAGMA_EXTENSION(NV_viewport_array2)
        MAGMA_EXTENSION(NV_viewport_swizzle)
        MAGMA_EXTENSION(NV_win32_keyed_mutex)

        MAGMA_EXTENSION(QCOM_render_pass_transform)

        PhysicalDeviceExtensions(std::shared_ptr<const PhysicalDevice> device);
    };
} // namespace magma
