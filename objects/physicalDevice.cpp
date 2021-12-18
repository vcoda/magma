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
#include "pch.h"
#pragma hdrstop
#include "physicalDevice.h"
#include "instance.h"
#include "device.h"
#include "surface.h"
#include "display.h"
#include "pipelineCache.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"
#include "../core/compare.h"

namespace magma
{
PhysicalDevice::PhysicalDevice(std::shared_ptr<Instance> instance, VkPhysicalDevice handle,
    std::shared_ptr<IAllocator> allocator) noexcept:
    Dispatchable<VkPhysicalDevice>(VK_OBJECT_TYPE_PHYSICAL_DEVICE, nullptr, std::move(allocator)),
    instance(std::move(instance))
{
    this->handle = handle;
}

std::shared_ptr<Device> PhysicalDevice::createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& enabledLayers, const std::vector<const char *>& enabledExtensions,
    const VkPhysicalDeviceFeatures& deviceFeatures, const std::vector<void *>& extendedDeviceFeatures /* {} */,
    const CreateInfo& chainedInfo /* default */) const
{   // std::make_shared() couldn't be used because of private constructor
    return std::shared_ptr<Device>(new Device(std::const_pointer_cast<PhysicalDevice>(shared_from_this()),
        queueDescriptors, enabledLayers, enabledExtensions, deviceFeatures, extendedDeviceFeatures,
        chainedInfo, this->hostAllocator));
}

VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const noexcept
{
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(handle, &features);
    return features;
}

VkFormatProperties PhysicalDevice::getFormatProperties(VkFormat format) const noexcept
{
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(handle, format, &formatProperties);
    return formatProperties;
}

VkImageFormatProperties PhysicalDevice::getImageFormatProperties(VkFormat format,
    VkImageType imageType, bool optimalTiling, VkImageUsageFlags usage,
    VkImageCreateFlags flags /* 0 */) const
{
    VkImageFormatProperties imageFormatProperties;
    const VkResult result = vkGetPhysicalDeviceImageFormatProperties(handle, format, imageType,
        optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR,
        usage, flags, &imageFormatProperties);
    MAGMA_THROW_FAILURE(result, "failed to get image format properties");
    return imageFormatProperties;
}

VkPhysicalDeviceProperties PhysicalDevice::getProperties() const noexcept
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(handle, &properties);
    return properties;
}

std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const
{
    uint32_t propertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(propertyCount);
    if (propertyCount > 0)
        vkGetPhysicalDeviceQueueFamilyProperties(handle, &propertyCount, queueFamilyProperties.data());
    return queueFamilyProperties;
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties() const noexcept
{
    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    return memoryProperties;
}

std::vector<VkLayerProperties> PhysicalDevice::enumerateLayers() const
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateDeviceLayerProperties(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count physical device layers");
    std::vector<VkLayerProperties> layers(propertyCount);
    if (propertyCount > 0)
    {
        result = vkEnumerateDeviceLayerProperties(handle, &propertyCount, layers.data());
        MAGMA_THROW_FAILURE(result, "failed to enumerate physical device layers");
    }
    return layers;
}

std::vector<VkExtensionProperties> PhysicalDevice::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count physical device extensions");
    std::vector<VkExtensionProperties> extensions(propertyCount);
    if (propertyCount > 0)
    {
        result = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, extensions.data());
        MAGMA_THROW_FAILURE(result, "failed to enumerate physical device extensions");
    }
    return extensions;
}

bool PhysicalDevice::getSurfaceSupport(std::shared_ptr<const Surface> surface) const noexcept
{
    VkBool32 supported;
    const VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(handle, 0, *surface, &supported);
    return (VK_SUCCESS == result) && (VK_TRUE == supported);
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesKHR surfaceCaps;
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, *surface, &surfaceCaps);
    MAGMA_THROW_FAILURE(result, "failed to get surface capabilities");
    return surfaceCaps;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(std::shared_ptr<const Surface> surface) const
{
    uint32_t formatCount;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count surface formats");
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    if (formatCount > 0)
    {
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, surfaceFormats.data());
        MAGMA_THROW_FAILURE(result, "failed to get surface formats");
    }
    return surfaceFormats;
}

std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(std::shared_ptr<const Surface> surface) const
{
    uint32_t presentModeCount;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count surface present modes");
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    if (presentModeCount > 0)
    {
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, presentModes.data());
        MAGMA_THROW_FAILURE(result, "failed to get surface present modes");
    }
    return presentModes;
}

#ifdef VK_EXT_full_screen_exclusive
std::vector<VkPresentModeKHR> PhysicalDevice::getSurfaceFullScreenExclusivePresentModes(std::shared_ptr<const Surface> surface,
    VkFullScreenExclusiveEXT fullScreenExclusive, void *hMonitor /* nullptr */) const
{
    std::vector<VkPresentModeKHR> presentModes;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    VkSurfaceFullScreenExclusiveWin32InfoEXT fullScreenExclusiveWin32SurfaceInfo;
    fullScreenExclusiveWin32SurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT;
    fullScreenExclusiveWin32SurfaceInfo.pNext = nullptr;
    fullScreenExclusiveWin32SurfaceInfo.hmonitor = reinterpret_cast<HMONITOR>(hMonitor);
#endif // VK_USE_PLATFORM_WIN32_KHR
    VkSurfaceFullScreenExclusiveInfoEXT fullScreenExclusiveSurfaceInfo;
    fullScreenExclusiveSurfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
#ifdef VK_USE_PLATFORM_WIN32_KHR
    fullScreenExclusiveSurfaceInfo.pNext = hMonitor ? &fullScreenExclusiveWin32SurfaceInfo : nullptr;
#else
    fullScreenExclusiveSurfaceInfo.pNext = nullptr;
    MAGMA_UNUSED(hMonitor);
#endif
    fullScreenExclusiveSurfaceInfo.fullScreenExclusive = fullScreenExclusive;
#ifdef VK_KHR_get_surface_capabilities2
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = &fullScreenExclusiveSurfaceInfo;
    surfaceInfo.surface = *surface;
    uint32_t presentModeCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceSurfacePresentModes2EXT, VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModeCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count full-screen exclusive surface present modes");
    if (presentModeCount > 0)
    {
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModes2EXT(handle, &surfaceInfo, &presentModeCount, presentModes.data());
        MAGMA_THROW_FAILURE(result, "failed to get full-screen exclusive surface present modes");
    }
#endif // VK_KHR_get_surface_capabilities2
    return presentModes;
}

bool PhysicalDevice::getSurfaceFullScreenExclusiveSupport(std::shared_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesFullScreenExclusiveEXT surfaceFullScreenCaps = {};
    surfaceFullScreenCaps.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT;
#ifdef VK_KHR_get_surface_capabilities2
    VkSurfaceCapabilities2KHR surfaceCaps;
    surfaceCaps.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
    surfaceCaps.pNext = &surfaceFullScreenCaps;
    getSurfaceCapabilities2(std::move(surface), surfaceCaps);
#endif
    return (VK_TRUE == surfaceFullScreenCaps.fullScreenExclusiveSupported);
}
#endif // VK_EXT_full_screen_exclusive

bool PhysicalDevice::getPresentationSupport(uint32_t queueFamilyIndex,
    void *display /* nullptr */,
    const void *visualID /* nullptr */) const noexcept
{
    VkBool32 result;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    MAGMA_UNUSED(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceWin32PresentationSupportKHR(handle, queueFamilyIndex);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    result = vkGetPhysicalDeviceXlibPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<::Display *>(display),
        *reinterpret_cast<const VisualID *>(visualID));
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    result = vkGetPhysicalDeviceXcbPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<xcb_connection_t *>(display),
        *reinterpret_cast<const xcb_visualid_t *>(visualID));
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceWaylandPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<wl_display *>(display));
#elif defined(VK_USE_PLATFORM_MIR_KHR)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceMirPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<MirConnection *>(display));
#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    result = vkGetPhysicalDeviceScreenPresentationSupportQNX(handle, queueFamilyIndex,
        reinterpret_cast<_screen_window *>(display));
#endif
    return (VK_TRUE == result);
}

#ifdef VK_KHR_display
std::vector<VkDisplayPropertiesKHR> PhysicalDevice::getDisplayProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count display properties");
    std::vector<VkDisplayPropertiesKHR> displayProperties(propertyCount);
    if (propertyCount > 0)
    {
        result = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, displayProperties.data());
        MAGMA_THROW_FAILURE(result, "failed to get display properties");
    }
    return displayProperties;
}

std::vector<VkDisplayPlanePropertiesKHR> PhysicalDevice::getDisplayPlaneProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count display plane properties");
    std::vector<VkDisplayPlanePropertiesKHR> displayPlaneProperties(propertyCount);
    if (propertyCount > 0)
    {
        result = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, displayPlaneProperties.data());
        MAGMA_THROW_FAILURE(result, "failed to get display plane properties");
    }
    return displayPlaneProperties;
}

std::vector<std::shared_ptr<Display>> PhysicalDevice::getSupportedDisplays(uint32_t planeIndex) const
{
    uint32_t displayCount;
    MAGMA_INSTANCE_EXTENSION(vkGetDisplayPlaneSupportedDisplaysKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    VkResult result = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, nullptr);
    MAGMA_THROW_FAILURE(result, "failed to count supported displays");
    std::vector<std::shared_ptr<Display>> supportedDisplays;
    if (displayCount > 0)
    {
        MAGMA_STACK_ARRAY(VkDisplayKHR, displays, displayCount);
        result = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, displays);
        MAGMA_THROW_FAILURE(result, "failed to get supported displays");
        for (const VkDisplayKHR handle : displays)
            supportedDisplays.emplace_back(new Display(shared_from_this(), handle, planeIndex));
    }
    return supportedDisplays;
}
#endif // VK_KHR_display

// Macro to shorten query expressions
#define PHYSICAL_DEVICE_GET_FEATURES(StructureType, EnumField) getFeatures<VkPhysicalDevice##StructureType>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_##EnumField)
#define PHYSICAL_DEVICE_GET_PROPERTIES(StructureType, EnumField) getProperties<VkPhysicalDevice##StructureType>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_##EnumField)
#define PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(StructureType, EnumField, member) features.member = getFeatures<VkPhysicalDevice##StructureType>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_##EnumField).member

#ifdef VK_AMD_shader_core_properties
VkPhysicalDeviceShaderCorePropertiesAMD PhysicalDevice::getShaderCoreProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(ShaderCorePropertiesAMD, SHADER_CORE_PROPERTIES_AMD);
}
#endif // VK_AMD_shader_core_properties

#ifdef VK_AMD_shader_core_properties2
VkPhysicalDeviceShaderCoreProperties2AMD PhysicalDevice::getShaderCoreProperties2() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(ShaderCoreProperties2AMD, SHADER_CORE_PROPERTIES_2_AMD);
}
#endif // VK_AMD_shader_core_properties2

#ifdef VK_EXT_blend_operation_advanced
VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT PhysicalDevice::getBlendOperationAdvancedProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(BlendOperationAdvancedPropertiesEXT, BLEND_OPERATION_ADVANCED_PROPERTIES_EXT);
}
#endif // VK_EXT_blend_operation_advanced

#ifdef VK_EXT_conditional_rendering
VkPhysicalDeviceConditionalRenderingFeaturesEXT PhysicalDevice::getConditionalRenderingFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(ConditionalRenderingFeaturesEXT, CONDITIONAL_RENDERING_FEATURES_EXT);
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_conservative_rasterization
VkPhysicalDeviceConservativeRasterizationPropertiesEXT PhysicalDevice::getConservativeRasterizationProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(ConservativeRasterizationPropertiesEXT, CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT);
}
#endif // VK_EXT_conservative_rasterization

#ifdef VK_EXT_custom_border_color
VkPhysicalDeviceCustomBorderColorFeaturesEXT PhysicalDevice::getCustomBorderColorFeatures() const
{
   return PHYSICAL_DEVICE_GET_FEATURES(CustomBorderColorFeaturesEXT, CUSTOM_BORDER_COLOR_FEATURES_EXT);
}

VkPhysicalDeviceCustomBorderColorPropertiesEXT PhysicalDevice::getCustomBorderColorProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(CustomBorderColorPropertiesEXT, CUSTOM_BORDER_COLOR_PROPERTIES_EXT);
}
#endif // VK_EXT_custom_border_color

#ifdef VK_EXT_descriptor_indexing
VkPhysicalDeviceDescriptorIndexingFeaturesEXT PhysicalDevice::getDescriptorIndexingFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(DescriptorIndexingFeaturesEXT, DESCRIPTOR_INDEXING_FEATURES_EXT);
}

VkPhysicalDeviceDescriptorIndexingPropertiesEXT PhysicalDevice::getDescriptorIndexingProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(DescriptorIndexingPropertiesEXT, DESCRIPTOR_INDEXING_PROPERTIES_EXT);
}
#endif // VK_EXT_descriptor_indexing

#ifdef VK_EXT_fragment_shader_interlock
VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT PhysicalDevice::getFragmentShaderInterlockFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(FragmentShaderInterlockFeaturesEXT, FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT);
}
#endif // VK_EXT_fragment_shader_interlock

#ifdef VK_EXT_inline_uniform_block
VkPhysicalDeviceInlineUniformBlockFeaturesEXT PhysicalDevice::getInlineUniformBlockFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(InlineUniformBlockFeaturesEXT, INLINE_UNIFORM_BLOCK_FEATURES_EXT);
}

VkPhysicalDeviceInlineUniformBlockPropertiesEXT PhysicalDevice::getInlineUniformBlockProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(InlineUniformBlockPropertiesEXT, INLINE_UNIFORM_BLOCK_PROPERTIES_EXT);
}
#endif // VK_EXT_inline_uniform_block

#ifdef VK_EXT_line_rasterization
VkPhysicalDeviceLineRasterizationFeaturesEXT PhysicalDevice::getLineRasterizationFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(LineRasterizationFeaturesEXT, LINE_RASTERIZATION_FEATURES_EXT);
}

VkPhysicalDeviceLineRasterizationPropertiesEXT PhysicalDevice::getLineRasterizationProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(LineRasterizationPropertiesEXT, LINE_RASTERIZATION_PROPERTIES_EXT);
}
#endif // VK_EXT_line_rasterization

#ifdef VK_EXT_subgroup_size_control
VkPhysicalDeviceSubgroupSizeControlFeaturesEXT PhysicalDevice::getSubgroupSizeControlFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(SubgroupSizeControlFeaturesEXT, SUBGROUP_SIZE_CONTROL_FEATURES_EXT);
}

VkPhysicalDeviceSubgroupSizeControlPropertiesEXT PhysicalDevice::getSubgroupSizeControlProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(SubgroupSizeControlPropertiesEXT, SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT);
}
#endif // VK_EXT_subgroup_size_control

#ifdef VK_EXT_transform_feedback
VkPhysicalDeviceTransformFeedbackFeaturesEXT PhysicalDevice::getTransformFeedbackFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(TransformFeedbackFeaturesEXT, TRANSFORM_FEEDBACK_FEATURES_EXT);
}

VkPhysicalDeviceTransformFeedbackPropertiesEXT PhysicalDevice::getTransformFeedbackProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(TransformFeedbackPropertiesEXT, TRANSFORM_FEEDBACK_PROPERTIES_EXT);
}
#endif // VK_EXT_transform_feedback

#ifdef VK_EXT_vertex_attribute_divisor
VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT PhysicalDevice::getVertexAttributeDivisorFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(VertexAttributeDivisorFeaturesEXT, VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT);
}
#endif // VK_EXT_vertex_attribute_divisor

#ifdef VK_KHR_8bit_storage
VkPhysicalDevice8BitStorageFeaturesKHR PhysicalDevice::get8BitStorageFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(8BitStorageFeaturesKHR, 8BIT_STORAGE_FEATURES_KHR);
}
#endif // VK_KHR_8bit_storage

#ifdef VK_KHR_16bit_storage
VkPhysicalDevice16BitStorageFeaturesKHR PhysicalDevice::get16BitStorageFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(16BitStorageFeaturesKHR, 16BIT_STORAGE_FEATURES_KHR);
}
#endif // VK_KHR_16bit_storage

#ifdef VK_KHR_buffer_device_address
VkPhysicalDeviceBufferDeviceAddressFeaturesKHR PhysicalDevice::getBufferDeviceAddressFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(BufferDeviceAddressFeaturesKHR, BUFFER_DEVICE_ADDRESS_FEATURES_KHR);
}
#endif // VK_KHR_buffer_device_address

#ifdef VK_KHR_driver_properties
VkPhysicalDeviceDriverPropertiesKHR PhysicalDevice::getDriverProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(DriverPropertiesKHR, DRIVER_PROPERTIES_KHR);
}
#endif // VK_KHR_driver_properties

#ifdef VK_KHR_multiview
VkPhysicalDeviceMultiviewFeaturesKHR PhysicalDevice::getMultiviewFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(MultiviewFeaturesKHR, MULTIVIEW_FEATURES_KHR);
}

VkPhysicalDeviceMultiviewPropertiesKHR PhysicalDevice::getMultiviewProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(MultiviewPropertiesKHR, MULTIVIEW_PROPERTIES_KHR);
}
#endif // VK_KHR_multiview

#ifdef VK_KHR_shader_atomic_int64
VkPhysicalDeviceShaderAtomicInt64FeaturesKHR PhysicalDevice::getShaderAtomicInt64Features() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(ShaderAtomicInt64FeaturesKHR, SHADER_ATOMIC_INT64_FEATURES_KHR);
}
#endif // VK_KHR_shader_atomic_int64

#ifdef VK_KHR_shader_clock
VkPhysicalDeviceShaderClockFeaturesKHR PhysicalDevice::getShaderClockFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(ShaderClockFeaturesKHR, SHADER_CLOCK_FEATURES_KHR);
}
#endif // VK_KHR_shader_clock

#ifdef VK_KHR_shader_float16_int8
VkPhysicalDeviceFloat16Int8FeaturesKHR PhysicalDevice::getShaderFloat16Int8Features() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(Float16Int8FeaturesKHR, FLOAT16_INT8_FEATURES_KHR);
}
#endif // VK_KHR_shader_float16_int8

#ifdef VK_KHR_variable_pointers
VkPhysicalDeviceVariablePointerFeaturesKHR PhysicalDevice::getVariablePointersFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(VariablePointerFeaturesKHR, VARIABLE_POINTER_FEATURES_KHR);
}
#endif // VK_KHR_variable_pointers

#ifdef VK_KHR_vulkan_memory_model
VkPhysicalDeviceVulkanMemoryModelFeaturesKHR PhysicalDevice::getVulkanMemoryModelFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(VulkanMemoryModelFeaturesKHR, VULKAN_MEMORY_MODEL_FEATURES_KHR);
}
#endif // VK_KHR_vulkan_memory_model

#ifdef VK_NV_compute_shader_derivatives
VkPhysicalDeviceComputeShaderDerivativesFeaturesNV PhysicalDevice::getComputeShaderDerivativesFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(ComputeShaderDerivativesFeaturesNV, COMPUTE_SHADER_DERIVATIVES_FEATURES_NV);
}
#endif // VK_NV_compute_shader_derivatives

#ifdef VK_NV_cooperative_matrix
VkPhysicalDeviceCooperativeMatrixFeaturesNV PhysicalDevice::getCooperativeMatrixFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(CooperativeMatrixFeaturesNV, COOPERATIVE_MATRIX_FEATURES_NV);
}

VkPhysicalDeviceCooperativeMatrixPropertiesNV PhysicalDevice::getCooperativeMatrixProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(CooperativeMatrixPropertiesNV, COOPERATIVE_MATRIX_PROPERTIES_NV);
}
#endif // VK_NV_cooperative_matrix

#ifdef VK_NV_mesh_shader
VkPhysicalDeviceMeshShaderFeaturesNV PhysicalDevice::getMeshShaderFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(MeshShaderFeaturesNV, MESH_SHADER_FEATURES_NV);
}

VkPhysicalDeviceMeshShaderPropertiesNV PhysicalDevice::getMeshShaderProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(MeshShaderPropertiesNV, MESH_SHADER_PROPERTIES_NV);
}
#endif // VK_NV_mesh_shader

#ifdef VK_NV_ray_tracing
VkPhysicalDeviceRayTracingPropertiesNV PhysicalDevice::getRayTracingProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(RayTracingPropertiesNV, RAY_TRACING_PROPERTIES_NV);
}
#endif // VK_NV_ray_tracing

#ifdef VK_NV_shading_rate_image
VkPhysicalDeviceShadingRateImageFeaturesNV PhysicalDevice::getShadingRateImageFeatures() const
{
    return PHYSICAL_DEVICE_GET_FEATURES(ShadingRateImageFeaturesNV, SHADING_RATE_IMAGE_FEATURES_NV);
}
#endif // VK_NV_shading_rate_image

#ifdef VK_NV_shader_sm_builtins
VkPhysicalDeviceShaderSMBuiltinsPropertiesNV PhysicalDevice::getShaderSMBuiltinsProperties() const
{
    return PHYSICAL_DEVICE_GET_PROPERTIES(ShaderSMBuiltinsPropertiesNV, SHADER_SM_BUILTINS_PROPERTIES_NV);
}
#endif // VK_NV_shader_sm_builtins

std::shared_ptr<Device> PhysicalDevice::createDefaultDevice() const
{
    const std::vector<float> defaultQueuePriorities = {1.0f};
    const std::vector<DeviceQueueDescriptor> queueDescriptors = {
        DeviceQueueDescriptor(shared_from_this(), VK_QUEUE_GRAPHICS_BIT, defaultQueuePriorities)
    };
    const std::vector<const char*> noLayers;
    const std::vector<const char*> swapchainExtension = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const VkPhysicalDeviceFeatures noDeviceFeatures = {};
    const std::vector<void *> noExtendedDeviceFeatures;
    return createDevice(queueDescriptors, noLayers, swapchainExtension, noDeviceFeatures, noExtendedDeviceFeatures);
}

bool PhysicalDevice::checkExtensionSupport(const char *extensionName) const
{
    MAGMA_ASSERT(extensionName);
    if (!extensionName || !strlen(extensionName))
        return false;
    if (extensions.empty())
    {   // Query once and cache
        const std::vector<VkExtensionProperties> extensionProperties = enumerateExtensions();
        for (const auto property : extensionProperties)
            extensions.emplace(property.extensionName);
    }
    return extensions.find(extensionName) != extensions.end();
}

PhysicalDeviceExtensionFeatures PhysicalDevice::checkExtensionFeaturesSupport() const
{
    PhysicalDeviceExtensionFeatures features;
#ifdef VK_KHR_get_physical_device_properties2
    if (instance->checkExtensionSupport(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)) {
#ifdef VK_AMD_device_coherent_memory
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(CoherentMemoryFeaturesAMD, COHERENT_MEMORY_FEATURES_AMD, deviceCoherentMemory);
#endif
#ifdef VK_EXT_blend_operation_advanced
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(BlendOperationAdvancedFeaturesEXT, BLEND_OPERATION_ADVANCED_FEATURES_EXT, advancedBlendCoherentOperations);
#endif
#ifdef VK_EXT_depth_clip_enable
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(DepthClipEnableFeaturesEXT, DEPTH_CLIP_ENABLE_FEATURES_EXT, depthClipEnable);
#endif
#ifdef VK_EXT_extended_dynamic_state
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ExtendedDynamicStateFeaturesEXT, EXTENDED_DYNAMIC_STATE_FEATURES_EXT, extendedDynamicState);
#endif
#ifdef VK_EXT_host_query_reset
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(HostQueryResetFeaturesEXT, HOST_QUERY_RESET_FEATURES_EXT, hostQueryReset);
#endif
#ifdef VK_EXT_image_robustness
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ImageRobustnessFeaturesEXT, IMAGE_ROBUSTNESS_FEATURES_EXT, robustImageAccess);
#endif
#ifdef VK_EXT_index_type_uint8
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(IndexTypeUint8FeaturesEXT, INDEX_TYPE_UINT8_FEATURES_EXT, indexTypeUint8);
#endif
#ifdef VK_EXT_memory_priority
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(MemoryPriorityFeaturesEXT, MEMORY_PRIORITY_FEATURES_EXT, memoryPriority);
#endif
#ifdef VK_EXT_pipeline_creation_cache_control
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(PipelineCreationCacheControlFeaturesEXT, PIPELINE_CREATION_CACHE_CONTROL_FEATURES_EXT, pipelineCreationCacheControl);
#endif
#ifdef VK_EXT_private_data
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(PrivateDataFeaturesEXT, PRIVATE_DATA_FEATURES_EXT, privateData);
#endif
#ifdef VK_EXT_scalar_block_layout
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ScalarBlockLayoutFeaturesEXT, SCALAR_BLOCK_LAYOUT_FEATURES_EXT, scalarBlockLayout);
#endif
#ifdef VK_EXT_shader_demote_to_helper_invocation
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ShaderDemoteToHelperInvocationFeaturesEXT, SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT, shaderDemoteToHelperInvocation);
#endif
#ifdef VK_EXT_texel_buffer_alignment
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(TexelBufferAlignmentFeaturesEXT, TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT, texelBufferAlignment);
#endif
#ifdef VK_EXT_ycbcr_image_arrays
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(YcbcrImageArraysFeaturesEXT, YCBCR_IMAGE_ARRAYS_FEATURES_EXT, ycbcrImageArrays);
#endif
#ifdef VK_KHR_imageless_framebuffer
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ImagelessFramebufferFeaturesKHR, IMAGELESS_FRAMEBUFFER_FEATURES_KHR, imagelessFramebuffer);
#endif
#ifdef VK_KHR_pipeline_executable_properties
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(PipelineExecutablePropertiesFeaturesKHR, PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR, pipelineExecutableInfo);
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(SamplerYcbcrConversionFeaturesKHR, SAMPLER_YCBCR_CONVERSION_FEATURES_KHR, samplerYcbcrConversion);
#endif
#ifdef VK_KHR_separate_depth_stencil_layouts
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(SeparateDepthStencilLayoutsFeaturesKHR, SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES_KHR, separateDepthStencilLayouts);
#endif
#ifdef VK_KHR_shader_draw_parameters
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ShaderDrawParametersFeatures, SHADER_DRAW_PARAMETERS_FEATURES, shaderDrawParameters);
#endif
#ifdef VK_KHR_shader_subgroup_extended_types
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ShaderSubgroupExtendedTypesFeaturesKHR, SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES_KHR, shaderSubgroupExtendedTypes);
#endif
#ifdef VK_KHR_shader_terminate_invocation
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ShaderTerminateInvocationFeaturesKHR, SHADER_TERMINATE_INVOCATION_FEATURES_KHR, shaderTerminateInvocation);
#endif
#ifdef VK_KHR_timeline_semaphore
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(TimelineSemaphoreFeaturesKHR, TIMELINE_SEMAPHORE_FEATURES_KHR, timelineSemaphore);
#endif
#ifdef VK_KHR_uniform_buffer_standard_layout
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(UniformBufferStandardLayoutFeaturesKHR, UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES_KHR, uniformBufferStandardLayout);
#endif
#ifdef VK_NV_corner_sampled_image
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(CornerSampledImageFeaturesNV, CORNER_SAMPLED_IMAGE_FEATURES_NV, cornerSampledImage);
#endif
#ifdef VK_NV_coverage_reduction_mode
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(CoverageReductionModeFeaturesNV, COVERAGE_REDUCTION_MODE_FEATURES_NV, coverageReductionMode);
#endif
#ifdef VK_NV_dedicated_allocation_image_aliasing
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(DedicatedAllocationImageAliasingFeaturesNV, DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV, dedicatedAllocationImageAliasing);
#endif
#ifdef VK_NV_device_diagnostics_config
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(DiagnosticsConfigFeaturesNV, DIAGNOSTICS_CONFIG_FEATURES_NV, diagnosticsConfig);
#endif
#ifdef VK_NV_fragment_shader_barycentric
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(FragmentShaderBarycentricFeaturesNV, FRAGMENT_SHADER_BARYCENTRIC_FEATURES_NV, fragmentShaderBarycentric);
#endif
#ifdef VK_NV_representative_fragment_test
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(RepresentativeFragmentTestFeaturesNV, REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV, representativeFragmentTest);
#endif
#ifdef VK_NV_scissor_exclusive
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ExclusiveScissorFeaturesNV, EXCLUSIVE_SCISSOR_FEATURES_NV, exclusiveScissor);
#endif
#ifdef VK_NV_shader_image_footprint
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ShaderImageFootprintFeaturesNV, SHADER_IMAGE_FOOTPRINT_FEATURES_NV, imageFootprint);
#endif
#ifdef VK_NV_shader_sm_builtins
    PHYSICAL_DEVICE_GET_FEATURE_BOOLEAN(ShaderSMBuiltinsFeaturesNV, SHADER_SM_BUILTINS_FEATURES_NV, shaderSMBuiltins);
#endif
    }
#endif // VK_KHR_get_physical_device_properties2
    return features;
}

bool PhysicalDevice::checkPinnedMemorySupport() const noexcept
{
    const VkPhysicalDeviceMemoryProperties memoryProperties = getMemoryProperties();
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        constexpr VkMemoryPropertyFlags pinnedMemoryFlags =
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        const VkMemoryType& memoryType = memoryProperties.memoryTypes[i];
        if ((memoryType.propertyFlags & pinnedMemoryFlags) == pinnedMemoryFlags)
            return true;
    }
    return false;
}

bool PhysicalDevice::checkPipelineCacheDataCompatibility(const void *cacheData) const noexcept
{
    MAGMA_ASSERT(cacheData);
    if (!cacheData)
        return false;
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(handle, &properties);
    PipelineCache::Header header;
    header.size = sizeof(PipelineCache::Header);
    header.version = VK_PIPELINE_CACHE_HEADER_VERSION_ONE;
    header.vendorID = properties.vendorID;
    header.deviceID = properties.deviceID;
    memcpy(header.cacheUUID, properties.pipelineCacheUUID, VK_UUID_SIZE);
    const PipelineCache::Header *cacheHeader = reinterpret_cast<const PipelineCache::Header *>(cacheData);
    return core::compare(cacheHeader, &header);
}

#ifdef VK_KHR_get_physical_device_properties2
void PhysicalDevice::getFeatures2(VkPhysicalDeviceFeatures2KHR& physicalDeviceFeatures) const
{
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceFeatures2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceFeatures2KHR(handle, &physicalDeviceFeatures);
}

void PhysicalDevice::getProperties2(VkPhysicalDeviceProperties2KHR& physicalDeviceProperties) const
{
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceProperties2KHR(handle, &physicalDeviceProperties);
}
#endif // VK_KHR_get_physical_device_properties2

#ifdef VK_KHR_get_surface_capabilities2
void PhysicalDevice::getSurfaceCapabilities2(std::shared_ptr<const Surface> surface, VkSurfaceCapabilities2KHR& surfaceCaps) const
{
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.surface = *surface;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceSurfaceCapabilities2KHR, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilities2KHR(handle, &surfaceInfo, &surfaceCaps);
    MAGMA_THROW_FAILURE(result, "failed to get surface capabilities");
}
#endif // VK_KHR_get_surface_capabilities2

template<typename PhysicalDeviceFeatures>
inline PhysicalDeviceFeatures PhysicalDevice::getFeatures(VkStructureType sType) const
{
    PhysicalDeviceFeatures physicalDeviceFeatures = {};
    physicalDeviceFeatures.sType = sType;
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceFeatures2KHR physicalDeviceFeatures2;
    physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
    physicalDeviceFeatures2.pNext = &physicalDeviceFeatures;
    getFeatures2(physicalDeviceFeatures2);
#endif
    return physicalDeviceFeatures;
}

template<typename PhysicalDeviceProperties>
inline PhysicalDeviceProperties PhysicalDevice::getProperties(VkStructureType sType) const
{
    PhysicalDeviceProperties physicalDeviceProperties = {};
    physicalDeviceProperties.sType = sType;
#ifdef VK_KHR_get_physical_device_properties2
    VkPhysicalDeviceProperties2KHR physicalDeviceProperties2;
    physicalDeviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
    physicalDeviceProperties2.pNext = &physicalDeviceProperties;
    getProperties2(physicalDeviceProperties2);
#endif
    return physicalDeviceProperties;
}
} // namespace magma
