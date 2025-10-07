/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#ifndef MAGMA_NO_EXCEPTIONS
#include "errorResult.h"

namespace magma::exception
{
const char *Error::description() const noexcept
{   // https://registry.khronos.org/vulkan/specs/latest/man/html/VkResult.html
    switch (error)
    {
    case VK_SUCCESS: return "Command successfully completed";
    case VK_NOT_READY: return "A fence or query has not yet completed";
    case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
    case VK_EVENT_SET: return "An event is signaled";
    case VK_EVENT_RESET: return "An event is unsignaled";
    case VK_INCOMPLETE: return "A return array was too small for the result";
    case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
    case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
    case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
    case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
    case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
    case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
    case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation-specific reasons";
    case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
    case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
    case VK_ERROR_FRAGMENTED_POOL:
        return "A pool allocation has failed due to fragmentation of the pool's memory. "
            "This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation";
    case VK_ERROR_UNKNOWN:
        return "An unknown error has occurred; either the application has provided invalid input, or an implementation failure has occurred";
#ifdef VK_KHR_maintenance1
    case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
        return "A pool memory allocation has failed. "
            "This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation.";
#endif // VK_KHR_maintenance1
#ifdef VK_KHR_external_memory
    case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return "An external handle is not a valid handle of the specified type";
#endif
#ifdef VK_EXT_descriptor_indexing
    case VK_ERROR_FRAGMENTATION_EXT: return "A descriptor pool creation has failed due to fragmentation";
#endif
#ifdef VK_KHR_buffer_device_address
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR:
        return "A buffer creation or memory allocation failed because the requested address is not available. "
            "A shader group handle assignment failed because the requested shader group handle information is no longer valid";
#endif // VK_KHR_buffer_device_address
#ifdef VK_EXT_pipeline_creation_cache_control
    case VK_PIPELINE_COMPILE_REQUIRED_EXT:
        return "A requested pipeline creation would have required compilation, but the application requested compilation to not be performed";
#endif // VK_EXT_pipeline_creation_cache_control
#ifdef VK_KHR_surface
    case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again";
#endif // VK_KHR_surface
#ifdef VK_KHR_swapchain
    case VK_SUBOPTIMAL_KHR:
        return "A swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail. "
            "Applications must query the new surface properties and recreate their swapchain if they wish to continue presenting to the surface";
#endif // VK_KHR_swapchain
#ifdef VK_KHR_display_swapchain
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image";
#endif // VK_KHR_display_swapchain
    default:
        return "Unknown error code";
    }
}

FeatureNotPresent::FeatureNotPresent(const VkPhysicalDeviceFeatures& enabledFeatures,
    const VkPhysicalDeviceFeatures& supportedFeatures) noexcept:
    Error(VK_ERROR_FEATURE_NOT_PRESENT, [&]() noexcept -> const char*
    {
        static const char *const featureNames[] = {
            "robustBufferAccess",
            "fullDrawIndexUint32",
            "imageCubeArray",
            "independentBlend",
            "geometryShader",
            "tessellationShader",
            "sampleRateShading",
            "dualSrcBlend",
            "logicOp",
            "multiDrawIndirect",
            "drawIndirectFirstInstance",
            "depthClamp",
            "depthBiasClamp",
            "fillModeNonSolid",
            "depthBounds",
            "wideLines",
            "largePoints",
            "alphaToOne",
            "multiViewport",
            "samplerAnisotropy",
            "textureCompressionETC2",
            "textureCompressionASTC_LDR",
            "textureCompressionBC",
            "occlusionQueryPrecise",
            "pipelineStatisticsQuery",
            "vertexPipelineStoresAndAtomics",
            "fragmentStoresAndAtomics",
            "shaderTessellationAndGeometryPointSize",
            "shaderImageGatherExtended",
            "shaderStorageImageExtendedFormats",
            "shaderStorageImageMultisample",
            "shaderStorageImageReadWithoutFormat",
            "shaderStorageImageWriteWithoutFormat",
            "shaderUniformBufferArrayDynamicIndexing",
            "shaderSampledImageArrayDynamicIndexing",
            "shaderStorageBufferArrayDynamicIndexing",
            "shaderStorageImageArrayDynamicIndexing",
            "shaderClipDistance",
            "shaderCullDistance",
            "shaderFloat64",
            "shaderInt64",
            "shaderInt16",
            "shaderResourceResidency",
            "shaderResourceMinLod",
            "sparseBinding",
            "sparseResidencyBuffer",
            "sparseResidencyImage2D",
            "sparseResidencyImage3D",
            "sparseResidency2Samples",
            "sparseResidency4Samples",
            "sparseResidency8Samples",
            "sparseResidency16Samples",
            "sparseResidencyAliased",
            "variableMultisampleRate",
            "inheritedQueries"
        };
        const VkBool32 *enabledFlags = reinterpret_cast<const VkBool32 *>(&enabledFeatures);
        const VkBool32 *supportedFlags = reinterpret_cast<const VkBool32 *>(&supportedFeatures);
        constexpr std::size_t featureCount = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);
        auto it = std::find_if(supportedFlags, supportedFlags + featureCount,
            [&enabledFlags](VkBool32 supportedFlag) {
                return *enabledFlags++ && !supportedFlag;
            });
        if (it < supportedFlags + featureCount)
            return featureNames[it - supportedFlags];
        return "unknown feature";
    }())
{}
} // namespace magma::exception
#endif // !MAGMA_NO_EXCEPTIONS
