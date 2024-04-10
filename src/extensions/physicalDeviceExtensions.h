/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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

    /* Functionality that is restricted to the device-level
       must be implemented as a device extension. For the list
       of extensions see: https://registry.khronos.org/vulkan/
       Keep list of extensions in alphabetical order! */

    class PhysicalDeviceExtensions : public Extensions
    {
    public:
        // Advanced Micro Devices, Inc.
        const VkBool32 AMD_buffer_marker : 1;
        const VkBool32 AMD_calibrated_timestamps : 1;
        const VkBool32 AMD_device_coherent_memory : 1;
        const VkBool32 AMD_display_native_hdr : 1;
        const VkBool32 AMD_draw_indirect_count : 1;
        const VkBool32 AMD_gcn_shader : 1;
        const VkBool32 AMD_gpa_interface : 1;
        const VkBool32 AMD_gpu_shader_half_float : 1;
        const VkBool32 AMD_gpu_shader_int16 : 1;
        const VkBool32 AMD_memory_overallocation_behavior : 1;
        const VkBool32 AMD_mixed_attachment_samples : 1;
        const VkBool32 AMD_negative_viewport_height : 1;
        const VkBool32 AMD_pipeline_compiler_control : 1;
        const VkBool32 AMD_programmable_sample_locations : 1;
        const VkBool32 AMD_rasterization_order : 1;
        const VkBool32 AMD_shader_ballot : 1;
        const VkBool32 AMD_shader_core_properties : 1;
        const VkBool32 AMD_shader_core_properties2 : 1;
        const VkBool32 AMD_shader_early_and_late_fragment_tests : 1;
        const VkBool32 AMD_shader_explicit_vertex_parameter : 1;
        const VkBool32 AMD_shader_fragment_mask : 1;
        const VkBool32 AMD_shader_image_load_store_lod : 1;
        const VkBool32 AMD_shader_info : 1;
        const VkBool32 AMD_shader_trinary_minmax : 1;
        const VkBool32 AMD_texture_gather_bias_lod : 1;
        const VkBool32 AMD_wave_limits : 1;

        // AMD experimental extensions
        const VkBool32 AMDX_shader_enqueue : 1;

        // ANDROID (Google LLC)
        const VkBool32 ANDROID_external_format_resolve : 1;
        const VkBool32 ANDROID_external_memory_android_hardware_buffer : 1;

        // ARM Limited
        const VkBool32 ARM_rasterization_order_attachment_access : 1;
        const VkBool32 ARM_render_pass_striped : 1;
        const VkBool32 ARM_scheduling_controls : 1;
        const VkBool32 ARM_shader_core_builtins : 1;
        const VkBool32 ARM_shader_core_properties : 1;

        // Multivendor
        const VkBool32 EXT_4444_formats : 1;
        const VkBool32 EXT_astc_decode_mode : 1;
        const VkBool32 EXT_attachment_feedback_loop_layout : 1;
        const VkBool32 EXT_blend_operation_advanced : 1;
        const VkBool32 EXT_border_color_swizzle : 1;
        const VkBool32 EXT_buffer_device_address : 1;
        const VkBool32 EXT_calibrated_timestamps : 1;
        const VkBool32 EXT_color_write_enable : 1;
        const VkBool32 EXT_conditional_rendering : 1;
        const VkBool32 EXT_conservative_rasterization : 1;
        const VkBool32 EXT_custom_border_color : 1;
        const VkBool32 EXT_debug_marker : 1;
        const VkBool32 EXT_depth_bias_control : 1;
        const VkBool32 EXT_depth_clamp_zero_one : 1;
        const VkBool32 EXT_depth_clip_control : 1;
        const VkBool32 EXT_depth_clip_enable : 1;
        const VkBool32 EXT_depth_range_unrestricted : 1;
        const VkBool32 EXT_descriptor_indexing : 1;
        const VkBool32 EXT_device_address_binding_report : 1;
        const VkBool32 EXT_device_fault : 1;
        const VkBool32 EXT_device_memory_report : 1;
        const VkBool32 EXT_discard_rectangles : 1;
        const VkBool32 EXT_display_control : 1;
        const VkBool32 EXT_display_surface_counter : 1;
        const VkBool32 EXT_dynamic_rendering_unused_attachments : 1;
        const VkBool32 EXT_extended_dynamic_state : 1;
        const VkBool32 EXT_extended_dynamic_state2 : 1;
        const VkBool32 EXT_extended_dynamic_state3 : 1;
        const VkBool32 EXT_external_memory_acquire_unmodified : 1;
        const VkBool32 EXT_external_memory_dma_buf : 1;
        const VkBool32 EXT_external_memory_host : 1;
        const VkBool32 EXT_filter_cubic : 1;
        const VkBool32 EXT_fragment_density_map : 1;
        const VkBool32 EXT_fragment_density_map2 : 1;
        const VkBool32 EXT_fragment_shader_interlock : 1;
        const VkBool32 EXT_full_screen_exclusive : 1;
        const VkBool32 EXT_global_priority : 1;
        const VkBool32 EXT_global_priority_query : 1;
        const VkBool32 EXT_graphics_pipeline_library : 1;
        const VkBool32 EXT_hdr_metadata : 1;
        const VkBool32 EXT_headless_surface : 1;
        const VkBool32 EXT_host_image_copy : 1;
        const VkBool32 EXT_host_query_reset : 1;
        const VkBool32 EXT_image_2d_view_of_3d : 1;
        const VkBool32 EXT_image_compression_control : 1;
        const VkBool32 EXT_image_compression_control_swapchain : 1;
        const VkBool32 EXT_image_drm_format_modifier : 1;
        const VkBool32 EXT_image_robustness : 1;
        const VkBool32 EXT_image_view_min_lod : 1;
        const VkBool32 EXT_index_type_uint8 : 1;
        const VkBool32 EXT_inline_uniform_block : 1;
        const VkBool32 EXT_legacy_dithering : 1;
        const VkBool32 EXT_line_rasterization : 1;
        const VkBool32 EXT_load_store_op_none : 1;
        const VkBool32 EXT_memory_budget : 1;
        const VkBool32 EXT_memory_priority : 1;
        const VkBool32 EXT_mesh_shader : 1;
        const VkBool32 EXT_metal_objects : 1;
        const VkBool32 EXT_multi_draw : 1;
        const VkBool32 EXT_multisampled_render_to_single_sampled : 1;
        const VkBool32 EXT_mutable_descriptor_type : 1;
        const VkBool32 EXT_non_seamless_cube_map : 1;
        const VkBool32 EXT_opacity_micromap : 1;
        const VkBool32 EXT_pageable_device_local_memory : 1;
        const VkBool32 EXT_pci_bus_info : 1;
        const VkBool32 EXT_physical_device_drm : 1;
        const VkBool32 EXT_pipeline_creation_cache_control : 1;
        const VkBool32 EXT_pipeline_creation_feedback : 1;
        const VkBool32 EXT_pipeline_properties : 1;
        const VkBool32 EXT_pipeline_protected_access : 1;
        const VkBool32 EXT_pipeline_robustness : 1;
        const VkBool32 EXT_post_depth_coverage : 1;
        const VkBool32 EXT_primitive_topology_list_restart : 1;
        const VkBool32 EXT_primitives_generated_query : 1;
        const VkBool32 EXT_private_data : 1;
        const VkBool32 EXT_provoking_vertex : 1;
        const VkBool32 EXT_queue_family_foreign : 1;
        const VkBool32 EXT_rasterization_order_attachment_access : 1;
        const VkBool32 EXT_rgba10x6_formats : 1;
        const VkBool32 EXT_robustness2 : 1;
        const VkBool32 EXT_sample_locations : 1;
        const VkBool32 EXT_sampler_filter_minmax : 1;
        const VkBool32 EXT_scalar_block_layout : 1;
        const VkBool32 EXT_separate_stencil_usage : 1;
        const VkBool32 EXT_shader_atomic_float : 1;
        const VkBool32 EXT_shader_atomic_float2 : 1;
        const VkBool32 EXT_shader_demote_to_helper_invocation : 1;
        const VkBool32 EXT_shader_image_atomic_int64 : 1;
        const VkBool32 EXT_shader_module_identifier : 1;
        const VkBool32 EXT_shader_object : 1;
        const VkBool32 EXT_shader_stencil_export : 1;
        const VkBool32 EXT_shader_subgroup_ballot : 1;
        const VkBool32 EXT_shader_subgroup_vote : 1;
        const VkBool32 EXT_shader_tile_image : 1;
        const VkBool32 EXT_shader_viewport_index_layer : 1;
        const VkBool32 EXT_subgroup_size_control : 1;
        const VkBool32 EXT_subpass_merge_feedback : 1;
        const VkBool32 EXT_swapchain_maintenance1 : 1;
        const VkBool32 EXT_texel_buffer_alignment : 1;
        const VkBool32 EXT_texture_compression_astc_hdr : 1;
        const VkBool32 EXT_tooling_info : 1;
        const VkBool32 EXT_transform_feedback : 1;
        const VkBool32 EXT_validation_cache : 1;
        const VkBool32 EXT_validation_features : 1;
        const VkBool32 EXT_validation_flags : 1;
        const VkBool32 EXT_vertex_attribute_divisor : 1;
        const VkBool32 EXT_vertex_input_dynamic_state : 1;
        const VkBool32 EXT_video_decode_h264 : 1;
        const VkBool32 EXT_video_decode_h265 : 1;
        const VkBool32 EXT_video_encode_h264 : 1;
        const VkBool32 EXT_video_encode_h265 : 1;
        const VkBool32 EXT_ycbcr_2plane_444_formats : 1;
        const VkBool32 EXT_ycbcr_image_arrays : 1;

        // FUCHSIA (Google LLC)
        const VkBool32 FUCHSIA_buffer_collection : 1;
        const VkBool32 FUCHSIA_external_memory : 1;
        const VkBool32 FUCHSIA_external_semaphore : 1;

        // GGP (Google, LLC)
        const VkBool32 GGP_frame_token : 1;

        // Google LLC
        const VkBool32 GOOGLE_decorate_string : 1;
        const VkBool32 GOOGLE_display_timing : 1;
        const VkBool32 GOOGLE_hlsl_functionality1 : 1;
        const VkBool32 GOOGLE_user_type : 1;

        // Huawei Technologies Co. Ltd.
        const VkBool32 HUAWEI_cluster_culling_shader : 1;
        const VkBool32 HUAWEI_invocation_mask : 1;
        const VkBool32 HUAWEI_subpass_shading : 1;

        // Imagination Technologies
        const VkBool32 IMG_filter_cubic : 1;
        const VkBool32 IMG_format_pvrtc : 1;
        const VkBool32 IMG_relaxed_line_rasterization : 1;

        // Intel Corporation
        const VkBool32 INTEL_performance_query : 1;
        const VkBool32 INTEL_shader_integer_functions2 : 1;

        // Khronos Group
        const VkBool32 KHR_16bit_storage : 1;
        const VkBool32 KHR_8bit_storage : 1;
        const VkBool32 KHR_acceleration_structure : 1;
        const VkBool32 KHR_bind_memory2 : 1;
        const VkBool32 KHR_buffer_device_address : 1;
        const VkBool32 KHR_cooperative_matrix : 1;
        const VkBool32 KHR_copy_commands2 : 1;
        const VkBool32 KHR_create_renderpass2 : 1;
        const VkBool32 KHR_dedicated_allocation : 1;
        const VkBool32 KHR_deferred_host_operations : 1;
        const VkBool32 KHR_depth_stencil_resolve : 1;
        const VkBool32 KHR_descriptor_update_template : 1;
        const VkBool32 KHR_device_group : 1;
        const VkBool32 KHR_display_swapchain : 1;
        const VkBool32 KHR_draw_indirect_count : 1;
        const VkBool32 KHR_driver_properties : 1;
        const VkBool32 KHR_dynamic_rendering : 1;
        const VkBool32 KHR_external_fence : 1;
        const VkBool32 KHR_external_fence_fd : 1;
        const VkBool32 KHR_external_fence_win32 : 1;
        const VkBool32 KHR_external_memory : 1;
        const VkBool32 KHR_external_memory_fd : 1;
        const VkBool32 KHR_external_memory_win32 : 1;
        const VkBool32 KHR_external_semaphore : 1;
        const VkBool32 KHR_external_semaphore_fd : 1;
        const VkBool32 KHR_external_semaphore_win32 : 1;
        const VkBool32 KHR_format_feature_flags2 : 1;
        const VkBool32 KHR_fragment_shader_barycentric : 1;
        const VkBool32 KHR_fragment_shading_rate : 1;
        const VkBool32 KHR_get_memory_requirements2 : 1;
        const VkBool32 KHR_global_priority : 1;
        const VkBool32 KHR_image_format_list : 1;
        const VkBool32 KHR_imageless_framebuffer : 1;
        const VkBool32 KHR_incremental_present : 1;
        const VkBool32 KHR_maintenance1 : 1;
        const VkBool32 KHR_maintenance2 : 1;
        const VkBool32 KHR_maintenance3 : 1;
        const VkBool32 KHR_maintenance4 : 1;
        const VkBool32 KHR_maintenance5 : 1;
        const VkBool32 KHR_map_memory2 : 1;
        const VkBool32 KHR_multiview : 1;
        const VkBool32 KHR_performance_query : 1;
        const VkBool32 KHR_pipeline_executable_properties : 1;
        const VkBool32 KHR_pipeline_library : 1;
        const VkBool32 KHR_portability_enumeration : 1;
        const VkBool32 KHR_portability_subset : 1;
        const VkBool32 KHR_present_id : 1;
        const VkBool32 KHR_present_wait : 1;
        const VkBool32 KHR_push_descriptor : 1;
        const VkBool32 KHR_ray_query : 1;
        const VkBool32 KHR_ray_tracing_maintenance1 : 1;
        const VkBool32 KHR_ray_tracing_pipeline : 1;
        const VkBool32 KHR_ray_tracing_position_fetch : 1;
        const VkBool32 KHR_relaxed_block_layout : 1;
        const VkBool32 KHR_sampler_mirror_clamp_to_edge : 1;
        const VkBool32 KHR_sampler_ycbcr_conversion : 1;
        const VkBool32 KHR_separate_depth_stencil_layouts : 1;
        const VkBool32 KHR_shader_atomic_int64 : 1;
        const VkBool32 KHR_shader_clock : 1;
        const VkBool32 KHR_shader_draw_parameters : 1;
        const VkBool32 KHR_shader_float16_int8 : 1;
        const VkBool32 KHR_shader_float_controls : 1;
        const VkBool32 KHR_shader_integer_dot_product : 1;
        const VkBool32 KHR_shader_non_semantic_info : 1;
        const VkBool32 KHR_shader_subgroup_extended_types : 1;
        const VkBool32 KHR_shader_subgroup_uniform_control_flow : 1;
        const VkBool32 KHR_shader_terminate_invocation : 1;
        const VkBool32 KHR_shared_presentable_image : 1;
        const VkBool32 KHR_spirv_1_4 : 1;
        const VkBool32 KHR_storage_buffer_storage_class : 1;
        const VkBool32 KHR_swapchain : 1;
        const VkBool32 KHR_swapchain_mutable_format : 1;
        const VkBool32 KHR_synchronization2 : 1;
        const VkBool32 KHR_timeline_semaphore : 1;
        const VkBool32 KHR_uniform_buffer_standard_layout : 1;
        const VkBool32 KHR_variable_pointers : 1;
        const VkBool32 KHR_video_decode_h264 : 1;
        const VkBool32 KHR_video_decode_h265 : 1;
        const VkBool32 KHR_video_decode_queue : 1;
        const VkBool32 KHR_video_encode_queue : 1;
        const VkBool32 KHR_video_queue : 1;
        const VkBool32 KHR_vulkan_memory_model : 1;
        const VkBool32 KHR_win32_keyed_mutex : 1;
        const VkBool32 KHR_workgroup_memory_explicit_layout : 1;
        const VkBool32 KHR_zero_initialize_workgroup_memory : 1;

        // Microsoft Corporation
        const VkBool32 MSFT_layered_driver : 1;

        // NVIDIA Corporation
        const VkBool32 NV_acquire_winrt_display : 1;
        const VkBool32 NV_clip_space_w_scaling : 1;
        const VkBool32 NV_compute_shader_derivatives : 1;
        const VkBool32 NV_cooperative_matrix : 1;
        const VkBool32 NV_copy_memory_indirect : 1;
        const VkBool32 NV_corner_sampled_image : 1;
        const VkBool32 NV_coverage_reduction_mode : 1;
        const VkBool32 NV_cuda_kernel_launch : 1;
        const VkBool32 NV_dedicated_allocation : 1;
        const VkBool32 NV_dedicated_allocation_image_aliasing : 1;
        const VkBool32 NV_descriptor_pool_overallocation : 1;
        const VkBool32 NV_device_diagnostic_checkpoints : 1;
        const VkBool32 NV_device_diagnostics_config : 1;
        const VkBool32 NV_device_generated_commands : 1;
        const VkBool32 NV_device_generated_commands_compute : 1;
        const VkBool32 NV_displacement_micromap : 1;
        const VkBool32 NV_extended_sparse_address_space : 1;
        const VkBool32 NV_external_memory : 1;
        const VkBool32 NV_external_memory_capabilities : 1;
        const VkBool32 NV_external_memory_rdma : 1;
        const VkBool32 NV_external_memory_win32 : 1;
        const VkBool32 NV_fill_rectangle : 1;
        const VkBool32 NV_fragment_coverage_to_color : 1;
        const VkBool32 NV_fragment_shader_barycentric : 1;
        const VkBool32 NV_fragment_shading_rate_enums : 1;
        const VkBool32 NV_framebuffer_mixed_samples : 1;
        const VkBool32 NV_geometry_shader_passthrough : 1;
        const VkBool32 NV_glsl_shader : 1;
        const VkBool32 NV_inherited_viewport_scissor : 1;
        const VkBool32 NV_linear_color_attachment : 1;
        const VkBool32 NV_low_latency : 1;
        const VkBool32 NV_low_latency2 : 1;
        const VkBool32 NV_memory_decompression : 1;
        const VkBool32 NV_mesh_shader : 1;
        const VkBool32 NV_optical_flow : 1;
        const VkBool32 NV_per_stage_descriptor_set : 1;
        const VkBool32 NV_present_barrier : 1;
        const VkBool32 NV_raw_access_chains : 1;
        const VkBool32 NV_ray_tracing : 1;
        const VkBool32 NV_ray_tracing_invocation_reorder : 1;
        const VkBool32 NV_ray_tracing_motion_blur : 1;
        const VkBool32 NV_ray_tracing_validation : 1;
        const VkBool32 NV_representative_fragment_test : 1;
        const VkBool32 NV_sample_mask_override_coverage : 1;
        const VkBool32 NV_scissor_exclusive : 1;
        const VkBool32 NV_shader_atomic_float16_vector : 1;
        const VkBool32 NV_shader_image_footprint : 1;
        const VkBool32 NV_shader_sm_builtins : 1;
        const VkBool32 NV_shader_subgroup_partitioned : 1;
        const VkBool32 NV_shading_rate_image : 1;
        const VkBool32 NV_viewport_array2 : 1;
        const VkBool32 NV_viewport_swizzle : 1;
        const VkBool32 NV_win32_keyed_mutex : 1;

        // NVIDIA experimental extensions
        const VkBool32 NVX_binary_import : 1;
        const VkBool32 NVX_device_generated_commands : 1;
        const VkBool32 NVX_display_timing : 1;
        const VkBool32 NVX_image_clear_padding : 1;
        const VkBool32 NVX_image_view_handle : 1;
        const VkBool32 NVX_multiview_per_view_attributes : 1;
        const VkBool32 NVX_raytracing : 1;

        // Samsung Electronics Co., Ltd.
        const VkBool32 SEC_amigo_profiling : 1;

        // Qualcomm Technologies, Inc.
        const VkBool32 QCOM_filter_cubic_clamp : 1;
        const VkBool32 QCOM_filter_cubic_weights : 1;
        const VkBool32 QCOM_fragment_density_map_offset : 1;
        const VkBool32 QCOM_image_processing : 1;
        const VkBool32 QCOM_image_processing2 : 1;
        const VkBool32 QCOM_multiview_per_view_render_areas : 1;
        const VkBool32 QCOM_multiview_per_view_viewports : 1;
        const VkBool32 QCOM_render_pass_shader_resolve : 1;
        const VkBool32 QCOM_render_pass_store_ops : 1;
        const VkBool32 QCOM_render_pass_transform : 1;
        const VkBool32 QCOM_rotated_copy_commands : 1;
        const VkBool32 QCOM_tile_properties : 1;
        const VkBool32 QCOM_ycbcr_degamma : 1;

        // BlackBerry Limited
        const VkBool32 QNX_external_memory_screen_buffer : 1;
        const VkBool32 QNX_screen_surface : 1;

        // Valve Corporation
        const VkBool32 VALVE_descriptor_set_host_mapping : 1;
        const VkBool32 VALVE_mutable_descriptor_type : 1;

        explicit PhysicalDeviceExtensions(std::shared_ptr<const PhysicalDevice> device);
    };

    MAGMA_TYPEDEF_UNIQUE_PTR(PhysicalDeviceExtensions);
} // namespace magma
