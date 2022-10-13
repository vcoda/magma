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
#include "dispatchable.h"
#include "../extensions/physicalDeviceExtendedFeatures.h"
#include "../misc/deviceQueueDescriptor.h"

namespace magma
{
    class Instance;
    class Surface;
    class Display;

    /* A physical device usually represents a single complete implementation of Vulkan
       (excluding instance-level functionality) available to the host,
       of which there are a finite number. */

    class PhysicalDevice : public Dispatchable<VkPhysicalDevice>,
        public std::enable_shared_from_this<PhysicalDevice>
    {
        explicit PhysicalDevice(std::shared_ptr<Instance> instance, VkPhysicalDevice handle,
            std::shared_ptr<IAllocator> allocator) noexcept;
        friend class Instance;

    public:
        std::shared_ptr<Device> createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
            const std::vector<const char *>& enabledLayers,
            const std::vector<const char *>& enabledExtensions,
            const VkPhysicalDeviceFeatures& deviceFeatures,
            const std::vector<void *>& extendedDeviceFeatures = {},
            const StructureChain& extendedInfo = StructureChain()) const;
        VkPhysicalDeviceFeatures getFeatures() const noexcept;
        VkFormatProperties getFormatProperties(VkFormat format) const noexcept;
        VkImageFormatProperties getImageFormatProperties(VkFormat format,
            VkImageType, bool optimalTiling, VkImageUsageFlags usage,
            VkImageCreateFlags flags = 0) const;
        VkPhysicalDeviceProperties getProperties() const noexcept;
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
        VkPhysicalDeviceMemoryProperties getMemoryProperties() const noexcept;
        std::vector<VkLayerProperties> enumerateLayers() const;
        std::vector<VkExtensionProperties> enumerateExtensions(const char *layerName = nullptr) const;
    #ifdef VK_KHR_surface
        bool getSurfaceSupport(std::shared_ptr<const Surface> surface,
            uint32_t queueFamilyIndex) const noexcept;
        VkSurfaceCapabilitiesKHR getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const;
        std::vector<VkSurfaceFormatKHR> getSurfaceFormats(std::shared_ptr<const Surface> surface) const;
        std::vector<VkPresentModeKHR> getSurfacePresentModes(std::shared_ptr<const Surface> surface) const;
    #endif // VK_KHR_surface
    #ifdef VK_AMD_display_native_hdr
        bool getSurfaceLocalDimmingSupport(std::shared_ptr<const Surface> surface) const;
    #endif
#ifdef VK_EXT_full_screen_exclusive
        std::vector<VkPresentModeKHR> getSurfaceFullScreenExclusivePresentModes(std::shared_ptr<const Surface> surface,
            VkFullScreenExclusiveEXT fullScreenExclusive,
            void *hMonitor = nullptr) const;
        bool getSurfaceFullScreenExclusiveSupport(std::shared_ptr<const Surface> surface) const;
#endif // VK_EXT_full_screen_exclusive
        // VK_KHR_xxx_surface
        bool getPresentationSupport(uint32_t queueFamilyIndex,
            void *display = nullptr,
            const void *visualID = nullptr) const noexcept;
#ifdef VK_EXT_tooling_info
        std::vector<VkPhysicalDeviceToolPropertiesEXT> getToolProperties() const;
#endif
#ifdef VK_KHR_display
        std::vector<VkDisplayPropertiesKHR> getDisplayProperties() const;
        std::vector<VkDisplayPlanePropertiesKHR> getDisplayPlaneProperties() const;
        std::vector<std::shared_ptr<Display>> getSupportedDisplays(uint32_t planeIndex) const;
#endif
#ifdef VK_AMD_shader_core_properties
        VkPhysicalDeviceShaderCorePropertiesAMD getShaderCoreProperties() const;
#endif
#ifdef VK_AMD_shader_core_properties2
        VkPhysicalDeviceShaderCoreProperties2AMD getShaderCoreProperties2() const;
#endif
#ifdef VK_EXT_blend_operation_advanced
        VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT getBlendOperationAdvancedProperties() const;
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
#ifdef VK_EXT_descriptor_indexing
        VkPhysicalDeviceDescriptorIndexingFeaturesEXT getDescriptorIndexingFeatures() const;
        VkPhysicalDeviceDescriptorIndexingPropertiesEXT getDescriptorIndexingProperties() const;
#endif
#ifdef VK_EXT_fragment_shader_interlock
        VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT getFragmentShaderInterlockFeatures() const;
#endif
#ifdef VK_EXT_inline_uniform_block
        VkPhysicalDeviceInlineUniformBlockFeaturesEXT getInlineUniformBlockFeatures() const;
        VkPhysicalDeviceInlineUniformBlockPropertiesEXT getInlineUniformBlockProperties() const;
#endif
#ifdef VK_EXT_line_rasterization
        VkPhysicalDeviceLineRasterizationFeaturesEXT getLineRasterizationFeatures() const;
        VkPhysicalDeviceLineRasterizationPropertiesEXT getLineRasterizationProperties() const;
#endif
#ifdef VK_EXT_primitive_topology_list_restart
       VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT getPrimitiveTopologyListRestartFeatures() const;
#endif
#ifdef VK_EXT_sampler_filter_minmax
        VkPhysicalDeviceSamplerFilterMinmaxPropertiesEXT getSamplerFilterMinmaxProperties() const;
#endif
#ifdef VK_EXT_subgroup_size_control
        VkPhysicalDeviceSubgroupSizeControlFeaturesEXT getSubgroupSizeControlFeatures() const;
        VkPhysicalDeviceSubgroupSizeControlPropertiesEXT getSubgroupSizeControlProperties() const;
#endif
#ifdef VK_EXT_transform_feedback
        VkPhysicalDeviceTransformFeedbackFeaturesEXT getTransformFeedbackFeatures() const;
        VkPhysicalDeviceTransformFeedbackPropertiesEXT getTransformFeedbackProperties() const;
#endif
#ifdef VK_EXT_vertex_attribute_divisor
        VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT getVertexAttributeDivisorProperties() const;
        VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT getVertexAttributeDivisorFeatures() const;
#endif
#ifdef VK_KHR_8bit_storage
        VkPhysicalDevice8BitStorageFeaturesKHR get8BitStorageFeatures() const;
#endif
#ifdef VK_KHR_16bit_storage
        VkPhysicalDevice16BitStorageFeaturesKHR get16BitStorageFeatures() const;
#endif
#ifdef VK_KHR_buffer_device_address
        VkPhysicalDeviceBufferDeviceAddressFeaturesKHR getBufferDeviceAddressFeatures() const;
#endif
#ifdef VK_KHR_driver_properties
        VkPhysicalDeviceDriverPropertiesKHR getDriverProperties() const;
#endif
#ifdef VK_KHR_multiview
        VkPhysicalDeviceMultiviewFeaturesKHR getMultiviewFeatures() const;
        VkPhysicalDeviceMultiviewPropertiesKHR getMultiviewProperties() const;
#endif
#ifdef VK_KHR_shader_atomic_int64
        VkPhysicalDeviceShaderAtomicInt64FeaturesKHR getShaderAtomicInt64Features() const;
#endif
#ifdef VK_KHR_shader_clock
        VkPhysicalDeviceShaderClockFeaturesKHR getShaderClockFeatures() const;
#endif
#ifdef VK_KHR_shader_float16_int8
        VkPhysicalDeviceFloat16Int8FeaturesKHR getShaderFloat16Int8Features() const;
#endif
#ifdef VK_KHR_variable_pointers
        VkPhysicalDeviceVariablePointerFeaturesKHR getVariablePointersFeatures() const;
#endif
#ifdef VK_KHR_vulkan_memory_model
        VkPhysicalDeviceVulkanMemoryModelFeaturesKHR getVulkanMemoryModelFeatures() const;
#endif
#ifdef VK_NV_compute_shader_derivatives
        VkPhysicalDeviceComputeShaderDerivativesFeaturesNV getComputeShaderDerivativesFeatures() const;
#endif
#ifdef VK_NV_cooperative_matrix
        VkPhysicalDeviceCooperativeMatrixFeaturesNV getCooperativeMatrixFeatures() const;
        VkPhysicalDeviceCooperativeMatrixPropertiesNV getCooperativeMatrixProperties() const;
#endif
#ifdef VK_NV_mesh_shader
        VkPhysicalDeviceMeshShaderFeaturesNV getMeshShaderFeatures() const;
        VkPhysicalDeviceMeshShaderPropertiesNV getMeshShaderProperties() const;
#endif
#ifdef VK_NV_ray_tracing
        VkPhysicalDeviceRayTracingPropertiesNV getRayTracingProperties() const;
#endif
#ifdef VK_NV_shading_rate_image
        VkPhysicalDeviceShadingRateImageFeaturesNV getShadingRateImageFeatures() const;
#endif
#ifdef VK_NV_shader_sm_builtins
        VkPhysicalDeviceShaderSMBuiltinsPropertiesNV getShaderSMBuiltinsProperties() const;
#endif
        PhysicalDeviceExtendedFeatures getExtendedFeatures() const;
        std::shared_ptr<Instance> getInstance() const noexcept { return instance; }
        std::shared_ptr<Device> createDefaultDevice() const;
        bool checkExtensionSupport(const char *extensionName) const;
        bool checkPinnedMemorySupport() const noexcept;
        bool checkPipelineCacheDataCompatibility(const void *cacheData) const noexcept;

    private:
    #ifdef VK_KHR_get_physical_device_properties2
        void getFeatures2(void *physicalDeviceFeatures) const;
        void getProperties2(void *physicalDeviceProperties) const;
    #endif
    #ifdef VK_KHR_get_surface_capabilities2
        void getSurfaceCapabilities2(std::shared_ptr<const Surface> surface,
            void *surfaceCaps) const;
    #endif
        template<typename PhysicalDeviceFeatures>
        PhysicalDeviceFeatures getFeatures(VkStructureType sType) const;
        template<typename PhysicalDeviceProperties>
        PhysicalDeviceProperties getProperties(VkStructureType sType) const;

        std::shared_ptr<Instance> instance;
        mutable std::set<std::string> extensions; // Cached implicit extensions
    };
} // namespace magma
