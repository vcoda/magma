/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "model/dispatchable.h"
#include "../misc/deviceQueueDescriptor.h"

namespace magma
{
    class Instance;
    class Device;
    class Surface;
    class Display;
    class DeviceFeatures;

    /* A physical device usually represents a single complete
       implementation of Vulkan (excluding instance-level functionality)
       available to the host, of which there are a finite number. */

    class PhysicalDevice : public Dispatchable<VkPhysicalDevice>,
        public std::enable_shared_from_this<PhysicalDevice>
    {
    public:
        ~PhysicalDevice();
        std::shared_ptr<Device> createDevice(const std::set<DeviceQueueDescriptor>& queueDescriptors,
            const NullTerminatedStringArray& enabledLayers,
            const NullTerminatedStringArray& enabledExtensions,
            const VkPhysicalDeviceFeatures& enabledFeatures,
            const StructureChain& enabledExtendedFeatures = StructureChain(),
            const StructureChain& extendedInfo = StructureChain()) const;
        Instance *getInstance() const noexcept { return instance; }
        VkPhysicalDeviceFeatures getFeatures() const noexcept;
        VkFormatProperties getFormatProperties(VkFormat format) const noexcept;
        VkImageFormatProperties getImageFormatProperties(VkFormat format,
            VkImageType type,
            VkImageUsageFlags usage,
            bool optimalTiling,
            VkImageCreateFlags flags = 0) const;
        std::vector<VkSparseImageFormatProperties> getSparseImageFormatProperties(VkFormat format,
            VkImageType type,
            VkImageUsageFlags usage,
            bool optimalTiling,
            uint32_t samples = 1) const;
        VkPhysicalDeviceProperties getProperties() const noexcept;
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
        VkPhysicalDeviceMemoryProperties getMemoryProperties() const noexcept;
        std::vector<VkLayerProperties> enumerateLayers() const;
        std::vector<VkExtensionProperties> enumerateExtensions(const char *layerName = nullptr) const;
    #ifdef VK_KHR_surface
        bool getSurfaceSupport(lent_ptr<const Surface> surface,
            uint32_t queueFamilyIndex) const noexcept;
        VkSurfaceCapabilitiesKHR getSurfaceCapabilities(lent_ptr<const Surface> surface) const;
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats(lent_ptr<const Surface> surface) const;
        std::vector<VkPresentModeKHR> getSurfacePresentModes(lent_ptr<const Surface> surface) const;
    #ifdef VK_EXT_full_screen_exclusive
        bool getFullScreenExclusiveSurfaceSupport(lent_ptr<const Surface> surface) const;
        std::vector<VkPresentModeKHR> getFullScreenExclusiveSurfacePresentModes(lent_ptr<const Surface> surface,
            VkFullScreenExclusiveEXT fullScreenExclusive
        #ifdef VK_KHR_win32_surface
           ,HMONITOR hMonitor = NULL
        #endif
            ) const;
    #endif // VK_EXT_full_screen_exclusive
    #ifdef VK_KHR_shared_presentable_image
        VkImageUsageFlags getSurfaceSharedPresentFlags(lent_ptr<const Surface> surface) const;
    #endif
    #ifdef VK_KHR_device_group
        std::vector<VkRect2D> getPresentRectangles(lent_ptr<const Surface> surface) const;
    #endif
    #ifdef VK_KHR_performance_query
        uint32_t getNumQueueFamilyPerformanceCounters(uint32_t queueFamilyIndex) const noexcept;
        std::vector<VkPerformanceCounterKHR> enumerateQueueFamilyPerformanceCounters(uint32_t queueFamilyIndex) const;
        std::vector<VkPerformanceCounterDescriptionKHR> enumerateQueueFamilyPerformanceCounterDescriptions(uint32_t queueFamilyIndex) const;
        uint32_t getNumQueueFamilyPerformanceQueryPasses(uint32_t queueFamilyIndex,
            const std::vector<uint32_t>& counterIndices) const;
    #endif // VK_KHR_performance_query
    #ifdef VK_AMD_display_native_hdr
        bool getSurfaceLocalDimmingSupport(lent_ptr<const Surface> surface) const;
    #endif
    #endif // VK_KHR_surface
        // VK_KHR_xxx_surface
        bool getPresentationSupport(uint32_t queueFamilyIndex,
            void *display = nullptr,
            const void *visualID = nullptr) const noexcept;
    #ifdef VK_EXT_calibrated_timestamps
        std::vector<VkTimeDomainEXT> getCalibrateableTimeDomains() const;
    #endif
    #ifdef VK_EXT_tooling_info
        std::vector<VkPhysicalDeviceToolPropertiesEXT> getToolProperties() const;
    #endif
    #ifdef VK_KHR_display
        std::vector<VkDisplayPropertiesKHR> getDisplayProperties() const;
        std::vector<VkDisplayPlanePropertiesKHR> getDisplayPlaneProperties() const;
        std::vector<std::unique_ptr<Display>> getSupportedDisplays(uint32_t planeIndex) const;
    #endif // VK_KHR_display
    #ifdef VK_KHR_external_memory_capabilities
        VkExternalMemoryPropertiesKHR getExternalBufferProperties(VkExternalMemoryHandleTypeFlagBitsKHR handleType,
            VkBufferUsageFlags usage,
            VkBufferCreateFlags flags = 0) const;
        #ifdef VK_KHR_get_physical_device_properties2
        VkExternalMemoryPropertiesKHR getExternalImageFormatProperties(VkExternalMemoryHandleTypeFlagBitsKHR handleType,
            VkFormat format, VkImageType type, bool optimalTiling, VkImageUsageFlags usage,
            VkImageCreateFlags flags = 0) const;
        #endif // VK_KHR_get_physical_device_properties2
    #endif // VK_KHR_external_memory_capabilities
    #ifdef VK_KHR_external_fence_capabilities
        VkExternalFencePropertiesKHR getExternalFenceProperties(VkExternalFenceHandleTypeFlagBitsKHR handleType) const;
    #endif
    #ifdef VK_KHR_external_semaphore_capabilities
        VkExternalSemaphorePropertiesKHR getExternalSemaphoreProperties(VkExternalSemaphoreHandleTypeFlagBitsKHR handleType) const;
    #endif
    #if defined(VK_ANDROID_external_memory_android_hardware_buffer) && defined(VK_KHR_get_physical_device_properties2)
        uint64_t getAndroidHardwareBufferUsage(VkFormat format,
            VkImageUsageFlags usage,
            VkImageCreateFlags flags = 0) const;
    #endif
    // Generated by utility tool. DO NOT MODIFY!
    #ifdef VK_AMD_device_coherent_memory
        VkPhysicalDeviceCoherentMemoryFeaturesAMD getCoherentMemoryFeatures() const;
    #endif
    #ifdef VK_AMD_shader_core_properties
        VkPhysicalDeviceShaderCorePropertiesAMD getShaderCoreProperties() const;
    #endif
    #ifdef VK_AMD_shader_core_properties2
        VkPhysicalDeviceShaderCoreProperties2AMD getShaderCoreProperties2() const;
    #endif
    #ifdef VK_AMD_shader_early_and_late_fragment_tests
        VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD getShaderEarlyAndLateFragmentTestsFeatures() const;
    #endif
    #ifdef VK_ARM_rasterization_order_attachment_access
        VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesARM getRasterizationOrderAttachmentAccessFeatures() const;
    #endif
    #ifdef VK_EXT_4444_formats
        VkPhysicalDevice4444FormatsFeaturesEXT get4444FormatsFeatures() const;
    #endif
    #ifdef VK_EXT_astc_decode_mode
        VkPhysicalDeviceASTCDecodeFeaturesEXT getASTCDecodeFeatures() const;
    #endif
    #ifdef VK_EXT_attachment_feedback_loop_layout
        VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT getAttachmentFeedbackLoopLayoutFeatures() const;
    #endif
    #ifdef VK_EXT_blend_operation_advanced
        VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT getBlendOperationAdvancedFeatures() const;
        VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT getBlendOperationAdvancedProperties() const;
    #endif
    #ifdef VK_EXT_border_color_swizzle
        VkPhysicalDeviceBorderColorSwizzleFeaturesEXT getBorderColorSwizzleFeatures() const;
    #endif
    #ifdef VK_EXT_buffer_device_address
        VkPhysicalDeviceBufferAddressFeaturesEXT getBufferAddressFeatures() const;
    #endif
    #ifdef VK_EXT_color_write_enable
        VkPhysicalDeviceColorWriteEnableFeaturesEXT getColorWriteEnableFeatures() const;
    #endif
    #ifdef VK_EXT_conditional_rendering
        VkPhysicalDeviceConditionalRenderingFeaturesEXT getConditionalRenderingFeatures() const;
    #endif
    #ifdef VK_EXT_conservative_rasterization
        VkPhysicalDeviceConservativeRasterizationPropertiesEXT getConservativeRasterizationProperties() const;
    #endif
    #ifdef VK_EXT_custom_border_color
        VkPhysicalDeviceCustomBorderColorFeaturesEXT getCustomBorderColorFeatures() const;
        VkPhysicalDeviceCustomBorderColorPropertiesEXT getCustomBorderColorProperties() const;
    #endif
    #ifdef VK_EXT_depth_clip_control
        VkPhysicalDeviceDepthClipControlFeaturesEXT getDepthClipControlFeatures() const;
    #endif
    #ifdef VK_EXT_depth_clip_enable
        VkPhysicalDeviceDepthClipEnableFeaturesEXT getDepthClipEnableFeatures() const;
    #endif
    #ifdef VK_EXT_descriptor_indexing
        VkPhysicalDeviceDescriptorIndexingFeaturesEXT getDescriptorIndexingFeatures() const;
        VkPhysicalDeviceDescriptorIndexingPropertiesEXT getDescriptorIndexingProperties() const;
    #endif
    #ifdef VK_EXT_device_memory_report
        VkPhysicalDeviceDeviceMemoryReportFeaturesEXT getDeviceMemoryReportFeatures() const;
    #endif
    #ifdef VK_EXT_discard_rectangles
        VkPhysicalDeviceDiscardRectanglePropertiesEXT getDiscardRectangleProperties() const;
    #endif
    #ifdef VK_EXT_extended_dynamic_state
        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT getExtendedDynamicStateFeatures() const;
    #endif
    #ifdef VK_EXT_extended_dynamic_state2
        VkPhysicalDeviceExtendedDynamicState2FeaturesEXT getExtendedDynamicState2Features() const;
    #endif
    #ifdef VK_EXT_external_memory_host
        VkPhysicalDeviceExternalMemoryHostPropertiesEXT getExternalMemoryHostProperties() const;
    #endif
    #ifdef VK_EXT_fragment_density_map
        VkPhysicalDeviceFragmentDensityMapFeaturesEXT getFragmentDensityMapFeatures() const;
        VkPhysicalDeviceFragmentDensityMapPropertiesEXT getFragmentDensityMapProperties() const;
    #endif
    #ifdef VK_EXT_fragment_density_map2
        VkPhysicalDeviceFragmentDensityMap2FeaturesEXT getFragmentDensityMap2Features() const;
        VkPhysicalDeviceFragmentDensityMap2PropertiesEXT getFragmentDensityMap2Properties() const;
    #endif
    #ifdef VK_EXT_fragment_shader_interlock
        VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT getFragmentShaderInterlockFeatures() const;
    #endif
    #ifdef VK_EXT_graphics_pipeline_library
        VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT getGraphicsPipelineLibraryFeatures() const;
        VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT getGraphicsPipelineLibraryProperties() const;
    #endif
    #ifdef VK_EXT_host_query_reset
        VkPhysicalDeviceHostQueryResetFeaturesEXT getHostQueryResetFeatures() const;
    #endif
    #ifdef VK_EXT_image_2d_view_of_3d
        VkPhysicalDeviceImage2DViewOf3DFeaturesEXT getImage2DViewOf3DFeatures() const;
    #endif
    #ifdef VK_EXT_image_compression_control
        VkPhysicalDeviceImageCompressionControlFeaturesEXT getImageCompressionControlFeatures() const;
    #endif
    #ifdef VK_EXT_image_compression_control_swapchain
        VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT getImageCompressionControlSwapchainFeatures() const;
    #endif
    #ifdef VK_EXT_image_robustness
        VkPhysicalDeviceImageRobustnessFeaturesEXT getImageRobustnessFeatures() const;
    #endif
    #ifdef VK_EXT_image_view_min_lod
        VkPhysicalDeviceImageViewMinLodFeaturesEXT getImageViewMinLodFeatures() const;
    #endif
    #ifdef VK_EXT_index_type_uint8
        VkPhysicalDeviceIndexTypeUint8FeaturesEXT getIndexTypeUint8Features() const;
    #endif
    #ifdef VK_EXT_inline_uniform_block
        VkPhysicalDeviceInlineUniformBlockFeaturesEXT getInlineUniformBlockFeatures() const;
        VkPhysicalDeviceInlineUniformBlockPropertiesEXT getInlineUniformBlockProperties() const;
    #endif
    #ifdef VK_EXT_line_rasterization
        VkPhysicalDeviceLineRasterizationFeaturesEXT getLineRasterizationFeatures() const;
        VkPhysicalDeviceLineRasterizationPropertiesEXT getLineRasterizationProperties() const;
    #endif
    #ifdef VK_EXT_memory_budget
        VkPhysicalDeviceMemoryBudgetPropertiesEXT getMemoryBudgetProperties() const;
    #endif
    #ifdef VK_EXT_memory_priority
        VkPhysicalDeviceMemoryPriorityFeaturesEXT getMemoryPriorityFeatures() const;
    #endif
    #ifdef VK_EXT_multi_draw
        VkPhysicalDeviceMultiDrawFeaturesEXT getMultiDrawFeatures() const;
        VkPhysicalDeviceMultiDrawPropertiesEXT getMultiDrawProperties() const;
    #endif
    #ifdef VK_EXT_multisampled_render_to_single_sampled
        VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT getMultisampledRenderToSingleSampledFeatures() const;
    #endif
    #ifdef VK_EXT_non_seamless_cube_map
        VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT getNonSeamlessCubeMapFeatures() const;
    #endif
    #ifdef VK_EXT_pageable_device_local_memory
        VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT getPageableDeviceLocalMemoryFeatures() const;
    #endif
    #ifdef VK_EXT_pci_bus_info
        VkPhysicalDevicePCIBusInfoPropertiesEXT getPCIBusInfoProperties() const;
    #endif
    #ifdef VK_EXT_physical_device_drm
        VkPhysicalDeviceDrmPropertiesEXT getDrmProperties() const;
    #endif
    #ifdef VK_EXT_pipeline_creation_cache_control
        VkPhysicalDevicePipelineCreationCacheControlFeaturesEXT getPipelineCreationCacheControlFeatures() const;
    #endif
    #ifdef VK_EXT_pipeline_properties
        VkPhysicalDevicePipelinePropertiesFeaturesEXT getPipelinePropertiesFeatures() const;
    #endif
    #ifdef VK_EXT_pipeline_robustness
        VkPhysicalDevicePipelineRobustnessFeaturesEXT getPipelineRobustnessFeatures() const;
        VkPhysicalDevicePipelineRobustnessPropertiesEXT getPipelineRobustnessProperties() const;
    #endif
    #ifdef VK_EXT_primitive_topology_list_restart
        VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT getPrimitiveTopologyListRestartFeatures() const;
    #endif
    #ifdef VK_EXT_primitives_generated_query
        VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT getPrimitivesGeneratedQueryFeatures() const;
    #endif
    #ifdef VK_EXT_private_data
        VkPhysicalDevicePrivateDataFeaturesEXT getPrivateDataFeatures() const;
    #endif
    #ifdef VK_EXT_provoking_vertex
        VkPhysicalDeviceProvokingVertexFeaturesEXT getProvokingVertexFeatures() const;
        VkPhysicalDeviceProvokingVertexPropertiesEXT getProvokingVertexProperties() const;
    #endif
    #ifdef VK_EXT_rgba10x6_formats
        VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT getRGBA10X6FormatsFeatures() const;
    #endif
    #ifdef VK_EXT_robustness2
        VkPhysicalDeviceRobustness2FeaturesEXT getRobustness2Features() const;
        VkPhysicalDeviceRobustness2PropertiesEXT getRobustness2Properties() const;
    #endif
    #ifdef VK_EXT_sample_locations
        VkPhysicalDeviceSampleLocationsPropertiesEXT getSampleLocationsProperties() const;
    #endif
    #ifdef VK_EXT_sampler_filter_minmax
        VkPhysicalDeviceSamplerFilterMinmaxPropertiesEXT getSamplerFilterMinmaxProperties() const;
    #endif
    #ifdef VK_EXT_scalar_block_layout
        VkPhysicalDeviceScalarBlockLayoutFeaturesEXT getScalarBlockLayoutFeatures() const;
    #endif
    #ifdef VK_EXT_shader_atomic_float
        VkPhysicalDeviceShaderAtomicFloatFeaturesEXT getShaderAtomicFloatFeatures() const;
    #endif
    #ifdef VK_EXT_shader_atomic_float2
        VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT getShaderAtomicFloat2Features() const;
    #endif
    #ifdef VK_EXT_shader_demote_to_helper_invocation
        VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT getShaderDemoteToHelperInvocationFeatures() const;
    #endif
    #ifdef VK_EXT_shader_image_atomic_int64
        VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT getShaderImageAtomicInt64Features() const;
    #endif
    #ifdef VK_EXT_shader_module_identifier
        VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT getShaderModuleIdentifierFeatures() const;
        VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT getShaderModuleIdentifierProperties() const;
    #endif
    #ifdef VK_EXT_subgroup_size_control
        VkPhysicalDeviceSubgroupSizeControlFeaturesEXT getSubgroupSizeControlFeatures() const;
        VkPhysicalDeviceSubgroupSizeControlPropertiesEXT getSubgroupSizeControlProperties() const;
    #endif
    #ifdef VK_EXT_subpass_merge_feedback
        VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT getSubpassMergeFeedbackFeatures() const;
    #endif
    #ifdef VK_EXT_texel_buffer_alignment
        VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT getTexelBufferAlignmentFeatures() const;
        VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT getTexelBufferAlignmentProperties() const;
    #endif
    #ifdef VK_EXT_texture_compression_astc_hdr
        VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT getTextureCompressionASTCHDRFeatures() const;
    #endif
    #ifdef VK_EXT_transform_feedback
        VkPhysicalDeviceTransformFeedbackFeaturesEXT getTransformFeedbackFeatures() const;
        VkPhysicalDeviceTransformFeedbackPropertiesEXT getTransformFeedbackProperties() const;
    #endif
    #ifdef VK_EXT_vertex_attribute_divisor
        VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT getVertexAttributeDivisorFeatures() const;
        VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT getVertexAttributeDivisorProperties() const;
    #endif
    #ifdef VK_EXT_vertex_input_dynamic_state
        VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT getVertexInputDynamicStateFeatures() const;
    #endif
    #ifdef VK_EXT_ycbcr_2plane_444_formats
        VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT getYcbcr2Plane444FormatsFeatures() const;
    #endif
    #ifdef VK_EXT_ycbcr_image_arrays
        VkPhysicalDeviceYcbcrImageArraysFeaturesEXT getYcbcrImageArraysFeatures() const;
    #endif
    #ifdef VK_HUAWEI_invocation_mask
        VkPhysicalDeviceInvocationMaskFeaturesHUAWEI getInvocationMaskFeatures() const;
    #endif
    #ifdef VK_HUAWEI_subpass_shading
        VkPhysicalDeviceSubpassShadingFeaturesHUAWEI getSubpassShadingFeatures() const;
        VkPhysicalDeviceSubpassShadingPropertiesHUAWEI getSubpassShadingProperties() const;
    #endif
    #ifdef VK_INTEL_shader_integer_functions2
        VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL getShaderIntegerFunctions2Features() const;
    #endif
    #ifdef VK_KHR_16bit_storage
        VkPhysicalDevice16BitStorageFeaturesKHR get16BitStorageFeatures() const;
    #endif
    #ifdef VK_KHR_8bit_storage
        VkPhysicalDevice8BitStorageFeaturesKHR get8BitStorageFeatures() const;
    #endif
    #ifdef VK_KHR_acceleration_structure
        VkPhysicalDeviceAccelerationStructureFeaturesKHR getAccelerationStructureFeatures() const;
        VkPhysicalDeviceAccelerationStructurePropertiesKHR getAccelerationStructureProperties() const;
    #endif
    #ifdef VK_KHR_buffer_device_address
        VkPhysicalDeviceBufferDeviceAddressFeaturesKHR getBufferDeviceAddressFeatures() const;
    #endif
    #ifdef VK_KHR_depth_stencil_resolve
        VkPhysicalDeviceDepthStencilResolvePropertiesKHR getDepthStencilResolveProperties() const;
    #endif
    #ifdef VK_KHR_device_group_creation
        VkPhysicalDeviceGroupPropertiesKHR getGroupProperties() const;
    #endif
    #ifdef VK_KHR_driver_properties
        VkPhysicalDeviceDriverPropertiesKHR getDriverProperties() const;
    #endif
    #ifdef VK_KHR_dynamic_rendering
        VkPhysicalDeviceDynamicRenderingFeaturesKHR getDynamicRenderingFeatures() const;
    #endif
    #ifdef VK_KHR_external_memory_capabilities
        VkPhysicalDeviceIDPropertiesKHR getIDProperties() const;
    #endif
    #ifdef VK_KHR_fragment_shader_barycentric
        VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR getFragmentShaderBarycentricFeatures() const;
        VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR getFragmentShaderBarycentricProperties() const;
    #endif
    #ifdef VK_KHR_fragment_shading_rate
        VkPhysicalDeviceFragmentShadingRateFeaturesKHR getFragmentShadingRateFeatures() const;
        VkPhysicalDeviceFragmentShadingRatePropertiesKHR getFragmentShadingRateProperties() const;
    #endif
    #ifdef VK_KHR_global_priority
        VkPhysicalDeviceGlobalPriorityQueryFeaturesKHR getGlobalPriorityQueryFeatures() const;
    #endif
    #ifdef VK_KHR_imageless_framebuffer
        VkPhysicalDeviceImagelessFramebufferFeaturesKHR getImagelessFramebufferFeatures() const;
    #endif
    #ifdef VK_KHR_maintenance2
        VkPhysicalDevicePointClippingPropertiesKHR getPointClippingProperties() const;
    #endif
    #ifdef VK_KHR_maintenance3
        VkPhysicalDeviceMaintenance3PropertiesKHR getMaintenance3Properties() const;
    #endif
    #ifdef VK_KHR_maintenance4
        VkPhysicalDeviceMaintenance4FeaturesKHR getMaintenance4Features() const;
        VkPhysicalDeviceMaintenance4PropertiesKHR getMaintenance4Properties() const;
    #endif
    #ifdef VK_KHR_multiview
        VkPhysicalDeviceMultiviewFeaturesKHR getMultiviewFeatures() const;
        VkPhysicalDeviceMultiviewPropertiesKHR getMultiviewProperties() const;
    #endif
    #ifdef VK_KHR_performance_query
        VkPhysicalDevicePerformanceQueryFeaturesKHR getPerformanceQueryFeatures() const;
        VkPhysicalDevicePerformanceQueryPropertiesKHR getPerformanceQueryProperties() const;
    #endif
    #ifdef VK_KHR_pipeline_executable_properties
        VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR getPipelineExecutablePropertiesFeatures() const;
    #endif
    #ifdef VK_KHR_present_id
        VkPhysicalDevicePresentIdFeaturesKHR getPresentIdFeatures() const;
    #endif
    #ifdef VK_KHR_present_wait
        VkPhysicalDevicePresentWaitFeaturesKHR getPresentWaitFeatures() const;
    #endif
    #ifdef VK_KHR_push_descriptor
        VkPhysicalDevicePushDescriptorPropertiesKHR getPushDescriptorProperties() const;
    #endif
    #ifdef VK_KHR_ray_query
        VkPhysicalDeviceRayQueryFeaturesKHR getRayQueryFeatures() const;
    #endif
    #ifdef VK_KHR_ray_tracing_maintenance1
        VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR getRayTracingMaintenance1Features() const;
    #endif
    #ifdef VK_KHR_ray_tracing_pipeline
        VkPhysicalDeviceRayTracingPipelineFeaturesKHR getRayTracingPipelineFeatures() const;
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR getRayTracingPipelineProperties() const;
    #endif
    #ifdef VK_KHR_sampler_ycbcr_conversion
        VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR getSamplerYcbcrConversionFeatures() const;
    #endif
    #ifdef VK_KHR_separate_depth_stencil_layouts
        VkPhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR getSeparateDepthStencilLayoutsFeatures() const;
    #endif
    #ifdef VK_KHR_shader_atomic_int64
        VkPhysicalDeviceShaderAtomicInt64FeaturesKHR getShaderAtomicInt64Features() const;
    #endif
    #ifdef VK_KHR_shader_clock
        VkPhysicalDeviceShaderClockFeaturesKHR getShaderClockFeatures() const;
    #endif
    #ifdef VK_KHR_shader_float16_int8
        VkPhysicalDeviceFloat16Int8FeaturesKHR getFloat16Int8Features() const;
    #endif
    #ifdef VK_KHR_shader_float_controls
        VkPhysicalDeviceFloatControlsPropertiesKHR getFloatControlsProperties() const;
    #endif
    #ifdef VK_KHR_shader_integer_dot_product
        VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR getShaderIntegerDotProductFeatures() const;
        VkPhysicalDeviceShaderIntegerDotProductPropertiesKHR getShaderIntegerDotProductProperties() const;
    #endif
    #ifdef VK_KHR_shader_subgroup_extended_types
        VkPhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR getShaderSubgroupExtendedTypesFeatures() const;
    #endif
    #ifdef VK_KHR_shader_subgroup_uniform_control_flow
        VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR getShaderSubgroupUniformControlFlowFeatures() const;
    #endif
    #ifdef VK_KHR_shader_terminate_invocation
        VkPhysicalDeviceShaderTerminateInvocationFeaturesKHR getShaderTerminateInvocationFeatures() const;
    #endif
    #ifdef VK_KHR_synchronization2
        VkPhysicalDeviceSynchronization2FeaturesKHR getSynchronization2Features() const;
    #endif
    #ifdef VK_KHR_timeline_semaphore
        VkPhysicalDeviceTimelineSemaphoreFeaturesKHR getTimelineSemaphoreFeatures() const;
        VkPhysicalDeviceTimelineSemaphorePropertiesKHR getTimelineSemaphoreProperties() const;
    #endif
    #ifdef VK_KHR_uniform_buffer_standard_layout
        VkPhysicalDeviceUniformBufferStandardLayoutFeaturesKHR getUniformBufferStandardLayoutFeatures() const;
    #endif
    #ifdef VK_KHR_variable_pointers
        VkPhysicalDeviceVariablePointersFeaturesKHR getVariablePointersFeatures() const;
    #endif
    #ifdef VK_KHR_vulkan_memory_model
        VkPhysicalDeviceVulkanMemoryModelFeaturesKHR getVulkanMemoryModelFeatures() const;
    #endif
    #ifdef VK_KHR_workgroup_memory_explicit_layout
        VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR getWorkgroupMemoryExplicitLayoutFeatures() const;
    #endif
    #ifdef VK_KHR_zero_initialize_workgroup_memory
        VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR getZeroInitializeWorkgroupMemoryFeatures() const;
    #endif
    #ifdef VK_NVX_multiview_per_view_attributes
        VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX getMultiviewPerViewAttributesProperties() const;
    #endif
    #ifdef VK_NV_compute_shader_derivatives
        VkPhysicalDeviceComputeShaderDerivativesFeaturesNV getComputeShaderDerivativesFeatures() const;
    #endif
    #ifdef VK_NV_cooperative_matrix
        VkPhysicalDeviceCooperativeMatrixFeaturesNV getCooperativeMatrixFeatures() const;
        VkPhysicalDeviceCooperativeMatrixPropertiesNV getCooperativeMatrixProperties() const;
    #endif
    #ifdef VK_NV_corner_sampled_image
        VkPhysicalDeviceCornerSampledImageFeaturesNV getCornerSampledImageFeatures() const;
    #endif
    #ifdef VK_NV_coverage_reduction_mode
        VkPhysicalDeviceCoverageReductionModeFeaturesNV getCoverageReductionModeFeatures() const;
    #endif
    #ifdef VK_NV_dedicated_allocation_image_aliasing
        VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV getDedicatedAllocationImageAliasingFeatures() const;
    #endif
    #ifdef VK_NV_device_diagnostics_config
        VkPhysicalDeviceDiagnosticsConfigFeaturesNV getDiagnosticsConfigFeatures() const;
    #endif
    #ifdef VK_NV_device_generated_commands
        VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV getDeviceGeneratedCommandsFeatures() const;
        VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV getDeviceGeneratedCommandsProperties() const;
    #endif
    #ifdef VK_NV_external_memory_rdma
        VkPhysicalDeviceExternalMemoryRDMAFeaturesNV getExternalMemoryRDMAFeatures() const;
    #endif
    #ifdef VK_NV_fragment_shading_rate_enums
        VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV getFragmentShadingRateEnumsFeatures() const;
        VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV getFragmentShadingRateEnumsProperties() const;
    #endif
    #ifdef VK_NV_inherited_viewport_scissor
        VkPhysicalDeviceInheritedViewportScissorFeaturesNV getInheritedViewportScissorFeatures() const;
    #endif
    #ifdef VK_NV_linear_color_attachment
        VkPhysicalDeviceLinearColorAttachmentFeaturesNV getLinearColorAttachmentFeatures() const;
    #endif
    #ifdef VK_NV_mesh_shader
        VkPhysicalDeviceMeshShaderFeaturesNV getMeshShaderFeatures() const;
        VkPhysicalDeviceMeshShaderPropertiesNV getMeshShaderProperties() const;
    #endif
    #ifdef VK_NV_ray_tracing
        VkPhysicalDeviceRayTracingPropertiesNV getRayTracingProperties() const;
    #endif
    #ifdef VK_NV_ray_tracing_motion_blur
        VkPhysicalDeviceRayTracingMotionBlurFeaturesNV getRayTracingMotionBlurFeatures() const;
    #endif
    #ifdef VK_NV_representative_fragment_test
        VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV getRepresentativeFragmentTestFeatures() const;
    #endif
    #ifdef VK_NV_scissor_exclusive
        VkPhysicalDeviceExclusiveScissorFeaturesNV getExclusiveScissorFeatures() const;
    #endif
    #ifdef VK_NV_shader_image_footprint
        VkPhysicalDeviceShaderImageFootprintFeaturesNV getShaderImageFootprintFeatures() const;
    #endif
    #ifdef VK_NV_shader_sm_builtins
        VkPhysicalDeviceShaderSMBuiltinsFeaturesNV getShaderSMBuiltinsFeatures() const;
        VkPhysicalDeviceShaderSMBuiltinsPropertiesNV getShaderSMBuiltinsProperties() const;
    #endif
    #ifdef VK_NV_shading_rate_image
        VkPhysicalDeviceShadingRateImageFeaturesNV getShadingRateImageFeatures() const;
        VkPhysicalDeviceShadingRateImagePropertiesNV getShadingRateImageProperties() const;
    #endif
    #ifdef VK_QCOM_fragment_density_map_offset
        VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM getFragmentDensityMapOffsetFeatures() const;
        VkPhysicalDeviceFragmentDensityMapOffsetPropertiesQCOM getFragmentDensityMapOffsetProperties() const;
    #endif
    #ifdef VK_QCOM_image_processing
        VkPhysicalDeviceImageProcessingFeaturesQCOM getImageProcessingFeatures() const;
        VkPhysicalDeviceImageProcessingPropertiesQCOM getImageProcessingProperties() const;
    #endif
    #ifdef VK_QCOM_tile_properties
        VkPhysicalDeviceTilePropertiesFeaturesQCOM getTilePropertiesFeatures() const;
    #endif
    #ifdef VK_SEC_amigo_profiling
        VkPhysicalDeviceAmigoProfilingFeaturesSEC getAmigoProfilingFeatures() const;
    #endif
    #ifdef VK_VALVE_descriptor_set_host_mapping
        VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE getDescriptorSetHostMappingFeatures() const;
    #endif
    #ifdef VK_VALVE_mutable_descriptor_type
        VkPhysicalDeviceMutableDescriptorTypeFeaturesVALVE getMutableDescriptorTypeFeatures() const;
    #endif
        // End of code generated by utility tool.
        std::shared_ptr<Device> createDefaultDevice() const;
        const std::unique_ptr<DeviceFeatures>& features() const;
        bool extensionSupported(const char *extensionName) const noexcept;
        VkDeviceSize getDeviceLocalHeapSize() const noexcept;
        VkDeviceSize getHostVisibleHeapSize() const noexcept;
        VkDeviceSize getDeviceLocalHostVisibleHeapSize() const noexcept;
    #ifdef VK_KHR_device_group
        VkDeviceSize getDeviceGroupSharedHeapSize() const noexcept;
    #endif
    #ifdef VK_NV_cluster_acceleration_structure
        VkPhysicalDeviceClusterAccelerationStructureFeaturesNV getClusterAccelerationStructureFeatures() const;
        VkPhysicalDeviceClusterAccelerationStructurePropertiesNV getClusterAccelerationStructureProperties() const;
    #endif
        bool getPipelineCacheCompatibility(const VkPipelineCacheHeaderVersionOne *header) const noexcept;

    private:
        MAGMA_MAKE_SHARED(PhysicalDevice)
        PhysicalDevice(Instance *instance,
            VkPhysicalDevice handle,
            std::shared_ptr<IAllocator> allocator) noexcept;
        VkInstance getNativeInstance() const noexcept;
        VkPhysicalDeviceFeatures getFeatures2(void *features) const;
        VkPhysicalDeviceProperties getProperties2(void *properties) const;
    #ifdef VK_KHR_surface
        VkSurfaceCapabilitiesKHR getSurfaceCapabilities2(lent_ptr<const Surface> surface,
            void *surfaceCapabilities) const;
    #endif // VK_KHR_surface

        Instance *instance;
        std::set<std::string> extensions;
        mutable std::unique_ptr<DeviceFeatures> deviceFeatures;
        friend Instance;
    };
} // namespace magma
