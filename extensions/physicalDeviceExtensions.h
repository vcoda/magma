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
        const bool AMD_buffer_marker;
        const bool AMD_calibrated_timestamps;
        const bool AMD_device_coherent_memory;
        const bool AMD_display_native_hdr;
        const bool AMD_draw_indirect_count;
        const bool AMD_gcn_shader;
        const bool AMD_gpa_interface;
        const bool AMD_gpu_shader_half_float;
        const bool AMD_gpu_shader_int16;
        const bool AMD_memory_overallocation_behavior;
        const bool AMD_mixed_attachment_samples;
        const bool AMD_negative_viewport_height;
        const bool AMD_pipeline_compiler_control;
        const bool AMD_rasterization_order;
        const bool AMD_shader_ballot;
        const bool AMD_shader_core_properties;
        const bool AMD_shader_core_properties2;
        const bool AMD_shader_explicit_vertex_parameter;
        const bool AMD_shader_fragment_mask;
        const bool AMD_shader_image_load_store_lod;
        const bool AMD_shader_info;
        const bool AMD_shader_trinary_minmax;
        const bool AMD_texture_gather_bias_lod;
        const bool AMD_wave_limits;

        const bool ANDROID_external_memory_android_hardware_buffer;

        const bool EXT_4444_formats;
        const bool EXT_astc_decode_mode;
        const bool EXT_blend_operation_advanced;
        const bool EXT_buffer_device_address;
        const bool EXT_calibrated_timestamps;
        const bool EXT_conditional_rendering;
        const bool EXT_conservative_rasterization;
        const bool EXT_custom_border_color;
        const bool EXT_debug_marker;
        const bool EXT_depth_clip_enable;
        const bool EXT_depth_range_unrestricted;
        const bool EXT_descriptor_indexing;
        const bool EXT_discard_rectangles;
        const bool EXT_display_control;
        const bool EXT_display_surface_counter;
        const bool EXT_extended_dynamic_state;
        const bool EXT_external_memory_dma_buf;
        const bool EXT_external_memory_host;
        const bool EXT_filter_cubic;
        const bool EXT_fragment_density_map;
        const bool EXT_fragment_shader_interlock;
        const bool EXT_full_screen_exclusive;
        const bool EXT_global_priority;
        const bool EXT_hdr_metadata;
        const bool EXT_headless_surface;
        const bool EXT_host_query_reset;
        const bool EXT_image_drm_format_modifier;
        const bool EXT_index_type_uint8;
        const bool EXT_inline_uniform_block;
        const bool EXT_line_rasterization;
        const bool EXT_memory_budget;
        const bool EXT_memory_priority;
        const bool EXT_pci_bus_info;
        const bool EXT_pipeline_creation_cache_control;
        const bool EXT_pipeline_creation_feedback;
        const bool EXT_post_depth_coverage;
        const bool EXT_private_data;
        const bool EXT_queue_family_foreign;
        const bool EXT_robustness2;
        const bool EXT_sample_locations;
        const bool EXT_sampler_filter_minmax;
        const bool EXT_scalar_block_layout;
        const bool EXT_separate_stencil_usage;
        const bool EXT_shader_demote_to_helper_invocation;
        const bool EXT_shader_stencil_export;
        const bool EXT_shader_subgroup_ballot;
        const bool EXT_shader_subgroup_vote;
        const bool EXT_shader_viewport_index_layer;
        const bool EXT_subgroup_size_control;
        const bool EXT_texel_buffer_alignment;
        const bool EXT_texture_compression_astc_hdr;
        const bool EXT_tooling_info;
        const bool EXT_transform_feedback;
        const bool EXT_validation_cache;
        const bool EXT_validation_features;
        const bool EXT_validation_flags;
        const bool EXT_vertex_attribute_divisor;
        const bool EXT_ycbcr_image_arrays;

        const bool GOOGLE_decorate_string;
        const bool GOOGLE_display_timing;
        const bool GOOGLE_hlsl_functionality1;
        const bool GOOGLE_user_type;

        const bool IMG_filter_cubic;
        const bool IMG_format_pvrtc;

        const bool INTEL_performance_query;
        const bool INTEL_shader_integer_functions2;

        const bool KHR_16bit_storage;
        const bool KHR_8bit_storage;
        const bool KHR_acceleration_structure;
        const bool KHR_bind_memory2;
        const bool KHR_buffer_device_address;
        const bool KHR_copy_commands2;
        const bool KHR_create_renderpass2;
        const bool KHR_dedicated_allocation;
        const bool KHR_deferred_host_operations;
        const bool KHR_depth_stencil_resolve;
        const bool KHR_descriptor_update_template;
        const bool KHR_device_group;
        const bool KHR_display_swapchain;
        const bool KHR_draw_indirect_count;
        const bool KHR_driver_properties;
        const bool KHR_external_fence;
        const bool KHR_external_fence_fd;
        const bool KHR_external_fence_win32;
        const bool KHR_external_memory;
        const bool KHR_external_memory_fd;
        const bool KHR_external_memory_win32;
        const bool KHR_external_semaphore;
        const bool KHR_external_semaphore_capabilities;
        const bool KHR_external_semaphore_fd;
        const bool KHR_external_semaphore_win32;
        const bool KHR_get_memory_requirements2;
        const bool KHR_image_format_list;
        const bool KHR_imageless_framebuffer;
        const bool KHR_incremental_present;
        const bool KHR_maintenance1;
        const bool KHR_maintenance2;
        const bool KHR_maintenance3;
        const bool KHR_multiview;
        const bool KHR_performance_query;
        const bool KHR_pipeline_executable_properties;
        const bool KHR_pipeline_library;
        const bool KHR_push_descriptor;
        const bool KHR_ray_query;
        const bool KHR_ray_tracing;
        const bool KHR_ray_tracing_pipeline;
        const bool KHR_relaxed_block_layout;
        const bool KHR_sampler_mirror_clamp_to_edge;
        const bool KHR_sampler_ycbcr_conversion;
        const bool KHR_separate_depth_stencil_layouts;
        const bool KHR_shader_atomic_int64;
        const bool KHR_shader_clock;
        const bool KHR_shader_draw_parameters;
        const bool KHR_shader_float16_int8;
        const bool KHR_shader_float_controls;
        const bool KHR_shader_non_semantic_info;
        const bool KHR_shader_subgroup_extended_types;
        const bool KHR_shared_presentable_image;
        const bool KHR_spirv_1_4;
        const bool KHR_storage_buffer_storage_class;
        const bool KHR_swapchain;
        const bool KHR_swapchain_mutable_format;
        const bool KHR_timeline_semaphore;
        const bool KHR_uniform_buffer_standard_layout;
        const bool KHR_variable_pointers;
        const bool KHR_vulkan_memory_model;
        const bool KHR_win32_keyed_mutex;

        const bool NVX_binary_import;
        const bool NVX_device_generated_commands;
        const bool NVX_image_view_handle;
        const bool NVX_multiview_per_view_attributes;

        const bool NV_acquire_winrt_display;
        const bool NV_clip_space_w_scaling;
        const bool NV_compute_shader_derivatives;
        const bool NV_cooperative_matrix;
        const bool NV_corner_sampled_image;
        const bool NV_coverage_reduction_mode;
        const bool NV_cuda_kernel_launch;
        const bool NV_dedicated_allocation;
        const bool NV_dedicated_allocation_image_aliasing;
        const bool NV_device_diagnostic_checkpoints;
        const bool NV_device_diagnostics_config;
        const bool NV_device_generated_commands;
        const bool NV_external_memory;
        const bool NV_external_memory_capabilities;
        const bool NV_external_memory_win32;
        const bool NV_fill_rectangle;
        const bool NV_fragment_coverage_to_color;
        const bool NV_fragment_shader_barycentric;
        const bool NV_framebuffer_mixed_samples;
        const bool NV_geometry_shader_passthrough;
        const bool NV_glsl_shader;
        const bool NV_mesh_shader;
        const bool NV_ray_tracing;
        const bool NV_representative_fragment_test;
        const bool NV_sample_mask_override_coverage;
        const bool NV_scissor_exclusive;
        const bool NV_shader_image_footprint;
        const bool NV_shader_sm_builtins;
        const bool NV_shader_subgroup_partitioned;
        const bool NV_shading_rate_image;
        const bool NV_viewport_array2;
        const bool NV_viewport_swizzle;
        const bool NV_win32_keyed_mutex;

        const bool QCOM_render_pass_shader_resolve;
        const bool QCOM_render_pass_store_ops;
        const bool QCOM_render_pass_transform;

        PhysicalDeviceExtensions(std::shared_ptr<const PhysicalDevice> device);
    };
} // namespace magma
