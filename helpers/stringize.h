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
#include "../objects/objectType.h"
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

namespace magma
{
    namespace helpers
    {
        const char *stringize(VkBool32) noexcept;
        const char *stringize(VkResult) noexcept;
        const char *stringize(VkSystemAllocationScope) noexcept;
        const char *stringize(VkInternalAllocationType) noexcept;
        const char *stringize(VkFormat) noexcept;
        const char *stringize(VkImageType) noexcept;
        const char *stringize(VkImageTiling) noexcept;
        const char *stringize(VkPhysicalDeviceType) noexcept;
        const char *stringize(VkQueryType) noexcept;
        const char *stringize(VkSharingMode) noexcept;
        const char *stringize(VkImageLayout) noexcept;
        const char *stringize(VkImageViewType) noexcept;
        const char *stringize(VkComponentSwizzle) noexcept;
        const char *stringize(VkVertexInputRate) noexcept;
        const char *stringize(VkPrimitiveTopology) noexcept;
        const char *stringize(VkPolygonMode) noexcept;
        const char *stringize(VkFrontFace) noexcept;
        const char *stringize(VkCompareOp) noexcept;
        const char *stringize(VkStencilOp) noexcept;
        const char *stringize(VkLogicOp) noexcept;
        const char *stringize(VkBlendFactor) noexcept;
        const char *stringize(VkBlendOp) noexcept;
        const char *stringize(VkDynamicState) noexcept;
        const char *stringize(VkFilter) noexcept;
        const char *stringize(VkSamplerMipmapMode) noexcept;
        const char *stringize(VkSamplerAddressMode) noexcept;
        const char *stringize(VkBorderColor) noexcept;
        const char *stringize(VkDescriptorType) noexcept;
        const char *stringize(VkAttachmentLoadOp) noexcept;
        const char *stringize(VkAttachmentStoreOp) noexcept;
        const char *stringize(VkPipelineBindPoint) noexcept;
        const char *stringize(VkCommandBufferLevel) noexcept;
        const char *stringize(VkIndexType) noexcept;
        const char *stringize(VkSubpassContents) noexcept;
        const char *stringize(VkObjectType) noexcept;
        const char *stringize(VkFormatFeatureFlagBits) noexcept;
        const char *stringize(VkImageUsageFlagBits) noexcept;
        const char *stringize(VkImageCreateFlagBits) noexcept;
        const char *stringize(VkSampleCountFlagBits) noexcept;
        const char *stringize(VkQueueFlagBits) noexcept;
        const char *stringize(VkMemoryPropertyFlagBits) noexcept;
        const char *stringize(VkMemoryHeapFlagBits) noexcept;
#ifdef VK_VERSION_1_1
        const char *stringize(VkDeviceQueueCreateFlagBits) noexcept;
#endif
        const char *stringize(VkPipelineStageFlagBits) noexcept;
        const char *stringize(VkImageAspectFlagBits) noexcept;
        const char *stringize(VkSparseImageFormatFlagBits) noexcept;
        const char *stringize(VkSparseMemoryBindFlagBits) noexcept;
        const char *stringize(VkFenceCreateFlagBits) noexcept;
        const char *stringize(VkQueryPipelineStatisticFlagBits) noexcept;
        const char *stringize(VkQueryResultFlagBits) noexcept;
        const char *stringize(VkBufferCreateFlagBits) noexcept;
        const char *stringize(VkBufferUsageFlagBits) noexcept;
        const char *stringize(VkPipelineCreateFlagBits) noexcept;
        const char *stringize(VkShaderStageFlagBits) noexcept;
        const char *stringize(VkCullModeFlagBits) noexcept;
        const char *stringize(VkColorComponentFlagBits) noexcept;
#ifdef VK_KHR_push_descriptor
        const char *stringize(VkDescriptorSetLayoutCreateFlagBits) noexcept;
#endif
        const char *stringize(VkDescriptorPoolCreateFlagBits) noexcept;
        const char *stringize(VkAttachmentDescriptionFlagBits) noexcept;
        const char *stringize(VkAccessFlagBits) noexcept;
        const char *stringize(VkCommandPoolCreateFlagBits) noexcept;
        const char *stringize(VkCommandPoolResetFlagBits) noexcept;
        const char *stringize(VkCommandBufferUsageFlagBits) noexcept;
        const char *stringize(VkQueryControlFlagBits) noexcept;
        const char *stringize(VkCommandBufferResetFlagBits) noexcept;
        const char *stringize(VkStencilFaceFlagBits) noexcept;
        const char *stringize(VkColorSpaceKHR) noexcept;
        const char *stringize(VkPresentModeKHR) noexcept;
        const char *stringize(VkSurfaceTransformFlagBitsKHR) noexcept;
        const char *stringize(VkCompositeAlphaFlagBitsKHR) noexcept;
    #ifdef VK_EXT_fullscreen_exclusive
        const char *stringize(VkFullScreenExclusiveEXT) noexcept;
    #endif
#ifdef VK_KHR_driver_properties
        const char *stringize(VkDriverIdKHR) noexcept;
#endif
#ifdef VK_NV_ray_tracing
        const char *stringize(VkRayTracingShaderGroupTypeNV) noexcept;
        const char *stringize(VkGeometryTypeNV) noexcept;
        const char *stringize(VkAccelerationStructureTypeNV) noexcept;
        const char *stringize(VkCopyAccelerationStructureModeNV) noexcept;
        const char *stringize(VkAccelerationStructureMemoryRequirementsTypeNV) noexcept;
        const char *stringize(VkGeometryFlagBitsNV) noexcept;
        const char *stringize(VkGeometryInstanceFlagBitsNV) noexcept;
        const char *stringize(VkBuildAccelerationStructureFlagBitsNV) noexcept;
#endif // VK_NV_ray_tracing

        const char *stringize(SpvReflectResult) noexcept;
        const char *stringize(SpvReflectTypeFlagBits) noexcept;
        const char *stringize(SpvReflectDecorationFlagBits) noexcept;
        const char *stringize(SpvReflectResourceType) noexcept;
        const char *stringize(SpvReflectFormat) noexcept;
        const char *stringize(SpvReflectVariableFlagBits) noexcept;
        const char *stringize(SpvReflectDescriptorType) noexcept;
        const char *stringize(SpvReflectShaderStageFlagBits) noexcept;
        const char *stringize(SpvReflectGenerator) noexcept;

        std::string stringifyImageCreateFlags(VkImageCreateFlags flags);
        std::string stringifyImageUsageFlags(VkImageUsageFlags);
        std::string stringifyBufferCreateFlags(VkBufferCreateFlags flags);
        std::string stringifyBufferUsageFlags(VkBufferUsageFlags);
    } // namespace helpers
} // namespace magma
