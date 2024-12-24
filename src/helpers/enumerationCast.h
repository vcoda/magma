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
#pragma once
#include "../misc/compatibility.h"
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

namespace magma::helpers
{
constexpr VkDebugReportObjectTypeEXT objectToDebugReportType(const VkObjectType objectType) noexcept
{
    switch (objectType)
    {
    case VK_OBJECT_TYPE_UNKNOWN:
        return VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
    case VK_OBJECT_TYPE_INSTANCE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT;
    case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
    case VK_OBJECT_TYPE_DEVICE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT;
    case VK_OBJECT_TYPE_QUEUE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT;
    case VK_OBJECT_TYPE_SEMAPHORE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT;
    case VK_OBJECT_TYPE_COMMAND_BUFFER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
    case VK_OBJECT_TYPE_FENCE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT;
    case VK_OBJECT_TYPE_DEVICE_MEMORY:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT;
    case VK_OBJECT_TYPE_BUFFER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
    case VK_OBJECT_TYPE_IMAGE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT;
    case VK_OBJECT_TYPE_EVENT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT;
    case VK_OBJECT_TYPE_QUERY_POOL:
        return VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT;
    case VK_OBJECT_TYPE_BUFFER_VIEW:
        return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT;
    case VK_OBJECT_TYPE_IMAGE_VIEW:
        return VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT;
    case VK_OBJECT_TYPE_SHADER_MODULE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT;
    case VK_OBJECT_TYPE_PIPELINE_CACHE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT;
    case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT;
    case VK_OBJECT_TYPE_RENDER_PASS:
        return VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT;
    case VK_OBJECT_TYPE_PIPELINE:
        return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT;
    case VK_OBJECT_TYPE_SAMPLER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
    case VK_OBJECT_TYPE_DESCRIPTOR_SET:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT;
    case VK_OBJECT_TYPE_FRAMEBUFFER:
        return VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT;
    case VK_OBJECT_TYPE_COMMAND_POOL:
        return VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT;
#ifdef VK_KHR_sampler_ycbcr_conversion
    case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR_EXT;
#endif
#ifdef VK_KHR_descriptor_update_template
    case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR_EXT;
#endif
#ifdef VK_KHR_surface
    case VK_OBJECT_TYPE_SURFACE_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT;
#endif
#ifdef VK_KHR_swapchain
    case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT;
#endif
#ifdef VK_KHR_display
    case VK_OBJECT_TYPE_DISPLAY_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT;
    case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT;
#endif
#ifdef VK_EXT_debug_report
    case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:
    #if VK_HEADER_VERSION <= 37 // TODO: exact version
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT;
    #else
        return VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
    #endif
#endif // VK_EXT_debug_report
#ifdef VK_NVX_binary_import
    case VK_OBJECT_TYPE_CU_MODULE_NVX:
        return VK_DEBUG_REPORT_OBJECT_TYPE_CU_MODULE_NVX_EXT;
    case VK_OBJECT_TYPE_CU_FUNCTION_NVX:
        return VK_DEBUG_REPORT_OBJECT_TYPE_CU_FUNCTION_NVX_EXT;
#endif // VK_NVX_binary_import
#ifdef VK_EXT_validation_cache
    case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:
        return VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT;
#endif
#ifdef VK_KHR_acceleration_structure
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR:
        return VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR_EXT;
#endif
#ifdef VK_NV_ray_tracing
    case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:
        return VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV_EXT;
#endif
#ifdef VK_FUCHSIA_buffer_collection
    case VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA:
        return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA_EXT;
#endif
    default:
        MAGMA_FAILURE("unknown object type");
        return VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
    }
}

constexpr VkDescriptorType spirvToDescriptorType(const SpvReflectDescriptorType descriptorType) noexcept
{
    switch (descriptorType)
    {
    case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
#ifdef VK_KHR_acceleration_structure
    case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR:
        return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
#endif
    default:
        MAGMA_FAILURE("unknown descriptor type");
        return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

constexpr VkImageType spirvDimToImageType(const SpvDim dim) noexcept
{
    switch (dim)
    {
    case SpvDim1D:
        return VK_IMAGE_TYPE_1D;
    case SpvDim2D:
    case SpvDimCube:
        return VK_IMAGE_TYPE_2D;
    case SpvDim3D:
        return VK_IMAGE_TYPE_3D;
    default:
        MAGMA_FAILURE("unknown spirv dimension");
        return VK_IMAGE_TYPE_MAX_ENUM;
    }
}
} // namespace magma::helpers
