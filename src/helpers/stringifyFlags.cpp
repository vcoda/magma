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
#include "stringifyFlags.h"
#include "streamInsertOperators.h"

namespace magma
{
namespace helpers
{
std::string stringifyMask(uint32_t mask)
{
    std::string str;
    for (int i = 32 - 1; i >= 0; --i)
        str += (mask & (1 << i) ? "1" : "0");
    return str;
}

template<class Bit>
static void stringifyOredBit(VkFlags flags, Bit bit, std::ostringstream& out)
{
    if (flags & bit)
    {
        if (out.tellp())
            out << " | ";
        out << bit;
    }
}

std::string stringifyImageFlags(VkImageCreateFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkImageCreateFlagBits bit: {
        VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
        VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
        VK_IMAGE_CREATE_SPARSE_ALIASED_BIT,
        VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

std::string stringifyImageUsage(VkImageUsageFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkImageUsageFlagBits bit: {
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_USAGE_STORAGE_BIT,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
    #ifdef VK_EXT_fragment_density_map
        VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT,
    #endif
    #ifdef VK_KHR_fragment_shading_rate
        VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
    #endif
    #ifdef VK_EXT_attachment_feedback_loop_layout
        VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT,
    #endif
    #ifdef VK_HUAWEI_invocation_mask
        VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI,
    #endif
    #ifdef VK_QCOM_image_processing
        VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM,
        VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM,
    #endif
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

std::string stringifyBufferFlags(VkBufferCreateFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkBufferCreateFlagBits bit: {
        VK_BUFFER_CREATE_SPARSE_BINDING_BIT,
        VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT,
        VK_BUFFER_CREATE_SPARSE_ALIASED_BIT,
    #ifdef VK_VERSION_1_2
        VK_BUFFER_CREATE_PROTECTED_BIT,
    #endif
    #ifdef VK_EXT_buffer_device_address
        VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_EXT,
    #endif
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

std::string stringifyBufferUsage(VkBufferUsageFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkBufferUsageFlagBits bit: {
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
        VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
    #ifdef VK_EXT_transform_feedback
        VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT,
        VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT,
    #endif
    #ifdef VK_EXT_conditional_rendering
        VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT,
    #endif
    #ifdef VK_KHR_acceleration_structure
        VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR,
        VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR,
    #endif
    #ifdef VK_KHR_ray_tracing_pipeline
        VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR,
    #endif
    #ifdef VK_NV_ray_tracing
        VK_BUFFER_USAGE_RAY_TRACING_BIT_NV,
    #endif
    #ifdef VK_EXT_buffer_device_address
        VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT,
    #endif
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

std::string stringifyPipelineShaderStageFlags(VkPipelineShaderStageCreateFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
#ifdef VK_EXT_subgroup_size_control
    for (VkPipelineShaderStageCreateFlagBits bit: {
        VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT_EXT,
        VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT_EXT,
        VK_PIPELINE_SHADER_STAGE_CREATE_FLAG_BITS_MAX_ENUM
    }) stringifyOredBit(flags, bit, out);
#endif // VK_EXT_subgroup_size_control
    return out.str();
}

std::string stringifyShaderStageFlags(VkShaderStageFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkShaderStageFlagBits bit: {
        VK_SHADER_STAGE_VERTEX_BIT,
        VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
        VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
        VK_SHADER_STAGE_GEOMETRY_BIT,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        VK_SHADER_STAGE_COMPUTE_BIT,
    #ifdef VK_KHR_ray_tracing_pipeline
        VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
        VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
        VK_SHADER_STAGE_MISS_BIT_KHR,
        VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
        VK_SHADER_STAGE_CALLABLE_BIT_KHR,
    #endif // VK_KHR_ray_tracing_pipeline
    #ifdef VK_NV_mesh_shader
        VK_SHADER_STAGE_TASK_BIT_NV,
        VK_SHADER_STAGE_MESH_BIT_NV,
    #endif // VK_NV_mesh_shader
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

std::string strinfigyRenderPassFlags(VkRenderPassCreateFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
#ifdef VK_QCOM_render_pass_transform
    for (VkAttachmentDescriptionFlags bit: {
        VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM
    }) stringifyOredBit(flags, bit, out);
#endif // VK_QCOM_render_pass_transform
    return out.str();
}

std::string strinfigyAttachmentDescriptionFlags(VkAttachmentDescriptionFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkAttachmentDescriptionFlags bit: {
        VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

std::string stringifyColorMask(VkColorComponentFlags flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkColorComponentFlagBits bit: {
        VK_COLOR_COMPONENT_R_BIT,
        VK_COLOR_COMPONENT_G_BIT,
        VK_COLOR_COMPONENT_B_BIT,
        VK_COLOR_COMPONENT_A_BIT
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}

#ifdef VK_KHR_swapchain
std::string stringifySwapchainFlags(VkSwapchainCreateFlagsKHR flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkSwapchainCreateFlagBitsKHR bit: {
        VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR,
        VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR,
        VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}
#endif // VK_KHR_swapchain

#ifdef VK_KHR_device_group
std::string stringifyDeviceGroupPresentMode(VkDeviceGroupPresentModeFlagsKHR flags)
{
    if (!flags)
        return "0";
    std::ostringstream out;
    for (VkDeviceGroupPresentModeFlagBitsKHR bit: {
        VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_BIT_KHR,
        VK_DEVICE_GROUP_PRESENT_MODE_REMOTE_BIT_KHR,
        VK_DEVICE_GROUP_PRESENT_MODE_SUM_BIT_KHR,
        VK_DEVICE_GROUP_PRESENT_MODE_LOCAL_MULTI_DEVICE_BIT_KHR
    }) stringifyOredBit(flags, bit, out);
    return out.str();
}
#endif // VK_KHR_device_group
} // namespace helpers
} // namespace magma
