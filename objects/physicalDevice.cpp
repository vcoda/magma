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
#include "../misc/instanceExtension.h"
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
    const VkResult get = vkGetPhysicalDeviceImageFormatProperties(handle, format, imageType,
        optimalTiling ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR,
        usage, flags, &imageFormatProperties);
    MAGMA_THROW_FAILURE(get, "failed to get image format properties");
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

std::shared_ptr<Device> PhysicalDevice::createDevice(
    const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers,
    const std::vector<const char *>& extensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures /* {} */) const
{
    return std::shared_ptr<Device>(new Device(
        std::const_pointer_cast<PhysicalDevice>(shared_from_this()),
        queueDescriptors, layers, extensions,
        deviceFeatures, extendedDeviceFeatures,
        this->hostAllocator));
}

std::vector<VkLayerProperties> PhysicalDevice::enumerateLayers() const
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateDeviceLayerProperties(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count physical device layers");
    std::vector<VkLayerProperties> layers(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult enumerate = vkEnumerateDeviceLayerProperties(handle, &propertyCount, layers.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate physical device layers");
    }
    return layers;
}

std::vector<VkExtensionProperties> PhysicalDevice::enumerateExtensions(const char *layerName /* nullptr */) const
{
    uint32_t propertyCount = 0;
    const VkResult count = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count physical device extensions");
    std::vector<VkExtensionProperties> extensions(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult enumerate = vkEnumerateDeviceExtensionProperties(handle, layerName, &propertyCount, extensions.data());
        MAGMA_THROW_FAILURE(enumerate, "failed to enumerate physical device extensions");
    }
    return extensions;
}

bool PhysicalDevice::getSurfaceSupport(std::shared_ptr<const Surface> surface) const noexcept
{
    VkBool32 supported;
    const VkResult get = vkGetPhysicalDeviceSurfaceSupportKHR(handle, 0, *surface, &supported);
    return (VK_SUCCESS == get) && (VK_TRUE == supported);
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(std::shared_ptr<const Surface> surface) const
{
    VkSurfaceCapabilitiesKHR caps;
    const VkResult get = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle, *surface, &caps);
    MAGMA_THROW_FAILURE(get, "failed to get surface capabilities");
    return caps;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats(std::shared_ptr<const Surface> surface) const
{
    uint32_t formatCount;
    const VkResult count = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count surface formats");
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    if (formatCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceSurfaceFormatsKHR(handle, *surface, &formatCount, surfaceFormats.data());
        MAGMA_THROW_FAILURE(get, "failed to get surface formats");
    }
    return surfaceFormats;
}

std::vector<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(std::shared_ptr<const Surface> surface) const
{
    uint32_t presentModeCount;
    const VkResult count = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count surface present modes");
    std::vector<VkPresentModeKHR> surfacePresentModes(presentModeCount);
    if (presentModeCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceSurfacePresentModesKHR(handle, *surface, &presentModeCount, surfacePresentModes.data());
        MAGMA_THROW_FAILURE(get, "failed to get surface present modes");
    }
    return surfacePresentModes;
}

bool PhysicalDevice::getPresentationSupport(uint32_t queueFamilyIndex,
    void *display /* nullptr */,
    const void *visualID /* nullptr */) const noexcept
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    MAGMA_UNUSED(display);
    MAGMA_UNUSED(visualID);
    const VkBool32 get = vkGetPhysicalDeviceWin32PresentationSupportKHR(handle, queueFamilyIndex);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    const VkBool32 get = vkGetPhysicalDeviceXlibPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<::Display *>(display),
        *reinterpret_cast<const VisualID *>(visualID));
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    MAGMA_ASSERT(display);
    MAGMA_ASSERT(visualID);
    const VkBool32 get = vkGetPhysicalDeviceXcbPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<xcb_connection_t *>(display),
        *reinterpret_cast<const xcb_visualid_t *>(visualID));
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    const VkBool32 get = vkGetPhysicalDeviceWaylandPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<wl_display *>(display));
#elif defined(VK_USE_PLATFORM_MIR_KHR)
    MAGMA_ASSERT(display);
    MAGMA_UNUSED(visualID);
    const VkBool32 get = vkGetPhysicalDeviceMirPresentationSupportKHR(handle, queueFamilyIndex,
        reinterpret_cast<MirConnection *>(display));
#endif
    return (get ? true : false);
}

#ifdef VK_KHR_display
std::vector<VkDisplayPropertiesKHR> PhysicalDevice::getDisplayProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult count = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count display properties");
    std::vector<VkDisplayPropertiesKHR> displayProperties(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceDisplayPropertiesKHR(handle, &propertyCount, displayProperties.data());
        MAGMA_THROW_FAILURE(get, "failed to get display properties");
    }
    return displayProperties;
}

std::vector<VkDisplayPlanePropertiesKHR> PhysicalDevice::getDisplayPlaneProperties() const
{
    uint32_t propertyCount;
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceDisplayPropertiesKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult count = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count display plane properties");
    std::vector<VkDisplayPlanePropertiesKHR> displayPlaneProperties(propertyCount);
    if (propertyCount > 0)
    {
        const VkResult get = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(handle, &propertyCount, displayPlaneProperties.data());
        MAGMA_THROW_FAILURE(get, "failed to get display plane properties");
    }
    return displayPlaneProperties;
}

std::vector<std::shared_ptr<Display>> PhysicalDevice::getSupportedDisplays(uint32_t planeIndex) const
{
    uint32_t displayCount;
    MAGMA_INSTANCE_EXTENSION(vkGetDisplayPlaneSupportedDisplaysKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult count = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, nullptr);
    MAGMA_THROW_FAILURE(count, "failed to count supported displays");
    std::vector<std::shared_ptr<Display>> supportedDisplays;
    if (displayCount > 0)
    {
        MAGMA_STACK_ARRAY(VkDisplayKHR, displays, displayCount);
        const VkResult get = vkGetDisplayPlaneSupportedDisplaysKHR(handle, planeIndex, &displayCount, displays);
        MAGMA_THROW_FAILURE(get, "failed to get supported displays");
        for (const VkDisplayKHR handle : displays)
            supportedDisplays.emplace_back(new Display(shared_from_this(), handle, planeIndex));
    }
    return supportedDisplays;
}
#endif // VK_KHR_display

#ifdef VK_AMD_shader_core_properties
VkPhysicalDeviceShaderCorePropertiesAMD PhysicalDevice::getShaderCoreProperties() const
{
    return getProperties<VkPhysicalDeviceShaderCorePropertiesAMD>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD);
}
#endif // VK_AMD_shader_core_properties

#ifdef VK_AMD_shader_core_properties2
VkPhysicalDeviceShaderCoreProperties2AMD PhysicalDevice::getShaderCoreProperties2() const
{
    return getProperties<VkPhysicalDeviceShaderCoreProperties2AMD>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD);
}
#endif // VK_AMD_shader_core_properties2

#ifdef VK_EXT_blend_operation_advanced
VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT PhysicalDevice::getBlendOperationAdvancedFeatures() const
{
    return getFeatures<VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT);
}
        
VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT PhysicalDevice::getBlendOperationAdvancedProperties() const
{
    return getProperties<VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT);
}
#endif // VK_EXT_blend_operation_advanced

#ifdef VK_EXT_conditional_rendering
VkPhysicalDeviceConditionalRenderingFeaturesEXT PhysicalDevice::getConditionalRenderingFeatures() const
{
    return getFeatures<VkPhysicalDeviceConditionalRenderingFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT);
}
#endif // VK_EXT_conditional_rendering

#ifdef VK_EXT_conservative_rasterization
VkPhysicalDeviceConservativeRasterizationPropertiesEXT PhysicalDevice::getConservativeRasterizationProperties() const
{
    return getProperties<VkPhysicalDeviceConservativeRasterizationPropertiesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT);
}
#endif // VK_EXT_conservative_rasterization

#ifdef VK_EXT_descriptor_indexing
VkPhysicalDeviceDescriptorIndexingFeaturesEXT PhysicalDevice::getDescriptorIndexingFeatures() const
{
    return getFeatures<VkPhysicalDeviceDescriptorIndexingFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT);
}

VkPhysicalDeviceDescriptorIndexingPropertiesEXT PhysicalDevice::getDescriptorIndexingProperties() const
{
    return getProperties<VkPhysicalDeviceDescriptorIndexingPropertiesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES_EXT);
}
#endif // VK_EXT_descriptor_indexing

#ifdef VK_EXT_fragment_shader_interlock
VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT PhysicalDevice::getFragmentShaderInterlockFeatures() const
{
    return getFeatures<VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT);
}
#endif // VK_EXT_fragment_shader_interlock

#ifdef VK_EXT_inline_uniform_block
VkPhysicalDeviceInlineUniformBlockFeaturesEXT PhysicalDevice::getInlineUniformBlockFeatures() const
{
    return getFeatures<VkPhysicalDeviceInlineUniformBlockFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT);
}

VkPhysicalDeviceInlineUniformBlockPropertiesEXT PhysicalDevice::getInlineUniformBlockProperties() const
{
    return getProperties<VkPhysicalDeviceInlineUniformBlockPropertiesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT);
}
#endif // VK_EXT_inline_uniform_block

#ifdef VK_EXT_subgroup_size_control
VkPhysicalDeviceSubgroupSizeControlFeaturesEXT PhysicalDevice::getSubgroupSizeControlFeatures() const
{
    return getFeatures<VkPhysicalDeviceSubgroupSizeControlFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES_EXT);
}

VkPhysicalDeviceSubgroupSizeControlPropertiesEXT PhysicalDevice::getSubgroupSizeControlProperties() const
{
    return getProperties<VkPhysicalDeviceSubgroupSizeControlPropertiesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES_EXT);
}
#endif // VK_EXT_subgroup_size_control

#ifdef VK_EXT_transform_feedback
VkPhysicalDeviceTransformFeedbackFeaturesEXT PhysicalDevice::getTransformFeedbackFeatures() const
{
    return getFeatures<VkPhysicalDeviceTransformFeedbackFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT);
}

VkPhysicalDeviceTransformFeedbackPropertiesEXT PhysicalDevice::getTransformFeedbackProperties() const
{
    return getProperties<VkPhysicalDeviceTransformFeedbackPropertiesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT);
}
#endif // VK_EXT_transform_feedback

#ifdef VK_EXT_vertex_attribute_divisor
VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT PhysicalDevice::getVertexAttributeDivisorFeatures() const
{
    return getFeatures<VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT);
}
#endif // VK_EXT_vertex_attribute_divisor

#ifdef VK_KHR_8bit_storage
VkPhysicalDevice8BitStorageFeaturesKHR PhysicalDevice::get8BitStorageFeatures() const
{
    return getFeatures<VkPhysicalDevice8BitStorageFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR);
}
#endif // VK_KHR_8bit_storage

#ifdef VK_KHR_16bit_storage
VkPhysicalDevice16BitStorageFeaturesKHR PhysicalDevice::get16BitStorageFeatures() const
{
    return getFeatures<VkPhysicalDevice16BitStorageFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR);
}
#endif // VK_KHR_16bit_storage

#ifdef VK_KHR_buffer_device_address
VkPhysicalDeviceBufferDeviceAddressFeaturesKHR PhysicalDevice::getBufferDeviceAddressFeatures() const
{
    return getFeatures<VkPhysicalDeviceBufferDeviceAddressFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR);
}
#endif // VK_KHR_buffer_device_address

#ifdef VK_KHR_multiview
VkPhysicalDeviceMultiviewFeaturesKHR PhysicalDevice::getMultiviewFeatures() const
{
    return getFeatures<VkPhysicalDeviceMultiviewFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR);
}
#endif // VK_KHR_multiview

#ifdef VK_KHR_shader_atomic_int64
VkPhysicalDeviceShaderAtomicInt64FeaturesKHR PhysicalDevice::getShaderAtomicInt64Features() const
{
    return getFeatures<VkPhysicalDeviceShaderAtomicInt64FeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES_KHR);
}
#endif // VK_KHR_shader_atomic_int64

#ifdef VK_KHR_shader_clock
VkPhysicalDeviceShaderClockFeaturesKHR PhysicalDevice::getShaderClockFeatures() const
{
    return getFeatures<VkPhysicalDeviceShaderClockFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR);
}
#endif // VK_KHR_shader_clock

#ifdef VK_KHR_shader_float16_int8
VkPhysicalDeviceShaderFloat16Int8FeaturesKHR PhysicalDevice::getShaderFloat16Int8Features() const
{
    return getFeatures<VkPhysicalDeviceShaderFloat16Int8FeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES_KHR);
}
#endif // VK_KHR_shader_float16_int8

#ifdef VK_KHR_variable_pointers
VkPhysicalDeviceVariablePointersFeaturesKHR PhysicalDevice::getVariablePointersFeatures() const
{
    return getFeatures<VkPhysicalDeviceVariablePointersFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES_KHR);
}
#endif // VK_KHR_variable_pointers

#ifdef VK_KHR_vulkan_memory_model
VkPhysicalDeviceVulkanMemoryModelFeaturesKHR PhysicalDevice::getVulkanMemoryModelFeatures() const
{
    return getFeatures<VkPhysicalDeviceVulkanMemoryModelFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES_KHR);
}
#endif // VK_KHR_vulkan_memory_model

#ifdef VK_NV_compute_shader_derivatives
VkPhysicalDeviceComputeShaderDerivativesFeaturesNV PhysicalDevice::getComputeShaderDerivativesFeatures() const
{
    return getFeatures<VkPhysicalDeviceComputeShaderDerivativesFeaturesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_NV);
}
#endif // VK_NV_compute_shader_derivatives

#ifdef VK_NV_cooperative_matrix
VkPhysicalDeviceCooperativeMatrixFeaturesNV PhysicalDevice::getCooperativeMatrixFeatures() const
{
    return getFeatures<VkPhysicalDeviceCooperativeMatrixFeaturesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV);
}

VkPhysicalDeviceCooperativeMatrixPropertiesNV PhysicalDevice::getCooperativeMatrixProperties() const
{
    return getProperties<VkPhysicalDeviceCooperativeMatrixPropertiesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV);
}
#endif // VK_NV_cooperative_matrix

#ifdef VK_NV_mesh_shader
VkPhysicalDeviceMeshShaderFeaturesNV PhysicalDevice::getMeshShaderFeatures() const
{
    return getFeatures<VkPhysicalDeviceMeshShaderFeaturesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV);
}

VkPhysicalDeviceMeshShaderPropertiesNV PhysicalDevice::getMeshShaderProperties() const
{
    return getProperties<VkPhysicalDeviceMeshShaderPropertiesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV);
}
#endif // VK_NV_mesh_shader

#ifdef VK_NV_ray_tracing
VkPhysicalDeviceRayTracingPropertiesNV PhysicalDevice::getRayTracingProperties() const
{
    return getProperties<VkPhysicalDeviceRayTracingPropertiesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV);
}
#endif // VK_NV_ray_tracing

#ifdef VK_NV_shading_rate_image
VkPhysicalDeviceShadingRateImageFeaturesNV PhysicalDevice::getShadingRateImageFeatures() const
{
    return getFeatures<VkPhysicalDeviceShadingRateImageFeaturesNV>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV);
}
#endif // VK_NV_shading_rate_image

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

void PhysicalDevice::getFeatures2(VkPhysicalDeviceFeatures2KHR& physicalDeviceFeatures2) const
{
    MAGMA_UNUSED(physicalDeviceFeatures2);
#ifdef VK_KHR_get_physical_device_properties2
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceFeatures2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceFeatures2KHR(handle, &physicalDeviceFeatures2);
#endif // VK_KHR_get_physical_device_properties2
}

void PhysicalDevice::getProperties2(VkPhysicalDeviceProperties2KHR& physicalDeviceProperties2) const
{   
    MAGMA_UNUSED(physicalDeviceProperties2);
#ifdef VK_KHR_get_physical_device_properties2
    MAGMA_INSTANCE_EXTENSION(vkGetPhysicalDeviceProperties2KHR, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    vkGetPhysicalDeviceProperties2KHR(handle, &physicalDeviceProperties2);
#endif // VK_KHR_get_physical_device_properties2
}

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

#ifdef VK_KHR_device_group
PhysicalDeviceGroup::PhysicalDeviceGroup(const std::vector<std::shared_ptr<PhysicalDevice>>& physicalDevices, uint32_t groupId) noexcept:
    physicalDevices(physicalDevices),
    groupId(groupId)
{}

std::shared_ptr<Device> PhysicalDeviceGroup::createDevice(const std::vector<DeviceQueueDescriptor>& queueDescriptors,
    const std::vector<const char *>& layers,
    const std::vector<const char *>& extensions,
    const VkPhysicalDeviceFeatures& deviceFeatures,
    const std::vector<void *>& extendedDeviceFeatures /* {} */) const
{
    VkDeviceGroupDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO;
    info.pNext = nullptr;
    info.physicalDeviceCount = physicalDeviceCount();
    MAGMA_STACK_ARRAY(VkPhysicalDevice, dereferencedPhysicalDevices, info.physicalDeviceCount);
    for (const auto& physicalDevice : physicalDevices)
        dereferencedPhysicalDevices.put(*physicalDevice);
    info.pPhysicalDevices = dereferencedPhysicalDevices;
    std::vector<void *> extendedDeviceGroupFeatures = extendedDeviceFeatures;
    extendedDeviceGroupFeatures.push_back(&info);
    return physicalDevices.front()->createDevice(queueDescriptors,
        layers, extensions,
        deviceFeatures, extendedDeviceGroupFeatures);
}
#endif // VK_KHR_device_group
} // namespace magma
