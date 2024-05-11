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
#include "../objects/objectType.h"

#define MAGMA_STREAM_INSERT_OPERATOR(Type) std::ostream& operator<<(std::ostream&, Type)

MAGMA_STREAM_INSERT_OPERATOR(VkResult);
MAGMA_STREAM_INSERT_OPERATOR(VkStructureType);
MAGMA_STREAM_INSERT_OPERATOR(VkSystemAllocationScope);
MAGMA_STREAM_INSERT_OPERATOR(VkInternalAllocationType);
MAGMA_STREAM_INSERT_OPERATOR(VkFormat);
MAGMA_STREAM_INSERT_OPERATOR(VkImageType);
MAGMA_STREAM_INSERT_OPERATOR(VkImageTiling);
MAGMA_STREAM_INSERT_OPERATOR(VkPhysicalDeviceType);
MAGMA_STREAM_INSERT_OPERATOR(VkQueryType);
MAGMA_STREAM_INSERT_OPERATOR(VkSharingMode);
MAGMA_STREAM_INSERT_OPERATOR(VkImageLayout);
MAGMA_STREAM_INSERT_OPERATOR(VkImageViewType);
MAGMA_STREAM_INSERT_OPERATOR(VkComponentSwizzle);
MAGMA_STREAM_INSERT_OPERATOR(VkVertexInputRate);
MAGMA_STREAM_INSERT_OPERATOR(VkPrimitiveTopology);
MAGMA_STREAM_INSERT_OPERATOR(VkPolygonMode);
MAGMA_STREAM_INSERT_OPERATOR(VkFrontFace);
MAGMA_STREAM_INSERT_OPERATOR(VkCompareOp);
MAGMA_STREAM_INSERT_OPERATOR(VkStencilOp);
MAGMA_STREAM_INSERT_OPERATOR(VkLogicOp);
MAGMA_STREAM_INSERT_OPERATOR(VkBlendFactor);
MAGMA_STREAM_INSERT_OPERATOR(VkBlendOp);
MAGMA_STREAM_INSERT_OPERATOR(VkDynamicState);
MAGMA_STREAM_INSERT_OPERATOR(VkFilter);
MAGMA_STREAM_INSERT_OPERATOR(VkSamplerMipmapMode);
MAGMA_STREAM_INSERT_OPERATOR(VkSamplerAddressMode);
MAGMA_STREAM_INSERT_OPERATOR(VkBorderColor);
MAGMA_STREAM_INSERT_OPERATOR(VkDescriptorType);
MAGMA_STREAM_INSERT_OPERATOR(VkAttachmentLoadOp);
MAGMA_STREAM_INSERT_OPERATOR(VkAttachmentStoreOp);
MAGMA_STREAM_INSERT_OPERATOR(VkPipelineBindPoint);
MAGMA_STREAM_INSERT_OPERATOR(VkCommandBufferLevel);
MAGMA_STREAM_INSERT_OPERATOR(VkIndexType);
MAGMA_STREAM_INSERT_OPERATOR(VkSubpassContents);
MAGMA_STREAM_INSERT_OPERATOR(VkObjectType);
MAGMA_STREAM_INSERT_OPERATOR(VkFormatFeatureFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkImageUsageFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkImageCreateFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkSampleCountFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkQueueFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkMemoryPropertyFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkMemoryHeapFlagBits);
#ifdef VK_VERSION_1_1
MAGMA_STREAM_INSERT_OPERATOR(VkDeviceQueueCreateFlagBits);
#endif
MAGMA_STREAM_INSERT_OPERATOR(VkPipelineStageFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkImageAspectFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkSparseImageFormatFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkSparseMemoryBindFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkFenceCreateFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkQueryPipelineStatisticFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkQueryResultFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkBufferCreateFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkBufferUsageFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkPipelineCreateFlagBits);
#ifdef VK_EXT_subgroup_size_control
MAGMA_STREAM_INSERT_OPERATOR(VkPipelineShaderStageCreateFlagBits);
#endif
MAGMA_STREAM_INSERT_OPERATOR(VkShaderStageFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkCullModeFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkColorComponentFlagBits);
#ifdef VK_KHR_push_descriptor
MAGMA_STREAM_INSERT_OPERATOR(VkDescriptorSetLayoutCreateFlagBits);
#endif
MAGMA_STREAM_INSERT_OPERATOR(VkDescriptorPoolCreateFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkAttachmentDescriptionFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkAccessFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkCommandPoolCreateFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkCommandPoolResetFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkCommandBufferUsageFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkQueryControlFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkCommandBufferResetFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(VkStencilFaceFlagBits);
#ifdef VK_KHR_performance_query
MAGMA_STREAM_INSERT_OPERATOR(VkPerformanceCounterUnitKHR);
MAGMA_STREAM_INSERT_OPERATOR(VkPerformanceCounterScopeKHR);
MAGMA_STREAM_INSERT_OPERATOR(VkPerformanceCounterStorageKHR);
MAGMA_STREAM_INSERT_OPERATOR(VkPerformanceCounterDescriptionFlagBitsKHR);
#endif // VK_KHR_performance_query
#ifdef VK_KHR_surface
MAGMA_STREAM_INSERT_OPERATOR(VkPresentModeKHR);
MAGMA_STREAM_INSERT_OPERATOR(VkColorSpaceKHR);
MAGMA_STREAM_INSERT_OPERATOR(VkSurfaceTransformFlagBitsKHR);
MAGMA_STREAM_INSERT_OPERATOR(VkCompositeAlphaFlagBitsKHR);
#endif // VK_KHR_surface
#ifdef VK_KHR_swapchain
MAGMA_STREAM_INSERT_OPERATOR(VkSwapchainCreateFlagBitsKHR);
#ifdef VK_KHR_device_group
MAGMA_STREAM_INSERT_OPERATOR(VkDeviceGroupPresentModeFlagBitsKHR);
#endif
#endif // VK_KHR_swapchain
#ifdef VK_EXT_fullscreen_exclusive
MAGMA_STREAM_INSERT_OPERATOR(VkFullScreenExclusiveEXT);
#endif
#ifdef VK_KHR_driver_properties
MAGMA_STREAM_INSERT_OPERATOR(VkDriverIdKHR);
#endif
#ifdef VK_NV_ray_tracing
MAGMA_STREAM_INSERT_OPERATOR(VkRayTracingShaderGroupTypeNV);
MAGMA_STREAM_INSERT_OPERATOR(VkGeometryTypeNV);
MAGMA_STREAM_INSERT_OPERATOR(VkAccelerationStructureTypeNV);
MAGMA_STREAM_INSERT_OPERATOR(VkCopyAccelerationStructureModeNV);
MAGMA_STREAM_INSERT_OPERATOR(VkAccelerationStructureMemoryRequirementsTypeNV);
MAGMA_STREAM_INSERT_OPERATOR(VkGeometryFlagBitsNV);
MAGMA_STREAM_INSERT_OPERATOR(VkGeometryInstanceFlagBitsNV);
MAGMA_STREAM_INSERT_OPERATOR(VkBuildAccelerationStructureFlagBitsNV);
#endif // VK_NV_ray_tracing

#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

MAGMA_STREAM_INSERT_OPERATOR(SpvReflectResult);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectTypeFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectDecorationFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectResourceType);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectFormat);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectVariableFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectDescriptorType);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectShaderStageFlagBits);
MAGMA_STREAM_INSERT_OPERATOR(SpvReflectGenerator);

namespace magma
{
    /* Helper type that wraps VkBool32 and
       outputs VK_TRUE/VK_FALSE instead of 0/1. */

    struct Bool
    {
        VkBool32 value;
        Bool(VkBool32 value): value(value) {}
        friend std::ostream& operator<<(std::ostream& out, Bool boolean)
        {
            return boolean.value ?
                (out << MAGMA_STRINGIZE(VK_TRUE)) :
                (out << MAGMA_STRINGIZE(VK_FALSE));
        }
    };
} // namespace magma
