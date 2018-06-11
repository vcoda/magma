/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../vulkan.h"
#include "../shared.h"

#ifdef MAGMA_DEBUG

namespace magma
{
    const char *stringize(VkSystemAllocationScope param);
    const char *stringize(VkInternalAllocationType param);
    const char *stringize(VkFormat param);
    const char *stringize(VkImageType param);  
    const char *stringize(VkImageTiling param);
    const char *stringize(VkPhysicalDeviceType param);
    const char *stringize(VkQueryType param);
    const char *stringize(VkSharingMode param);
    const char *stringize(VkImageLayout param);
    const char *stringize(VkImageViewType param);
    const char *stringize(VkComponentSwizzle param);
    const char *stringize(VkVertexInputRate param);
    const char *stringize(VkPrimitiveTopology param);
    const char *stringize(VkPolygonMode param);
    const char *stringize(VkFrontFace param);
    const char *stringize(VkCompareOp param);
    const char *stringize(VkStencilOp param);
    const char *stringize(VkLogicOp param);
    const char *stringize(VkBlendFactor param);
    const char *stringize(VkBlendOp param);
    const char *stringize(VkDynamicState param);
    const char *stringize(VkFilter param);
    const char *stringize(VkSamplerMipmapMode param);
    const char *stringize(VkSamplerAddressMode param);
    const char *stringize(VkBorderColor param);
    const char *stringize(VkDescriptorType param);
    const char *stringize(VkAttachmentLoadOp param);
    const char *stringize(VkAttachmentStoreOp param); 
    const char *stringize(VkPipelineBindPoint param);
    const char *stringize(VkCommandBufferLevel param);
    const char *stringize(VkIndexType param);
    const char *stringize(VkSubpassContents param);
    const char *stringize(VkObjectType param);
    const char *stringize(VkFormatFeatureFlagBits bit);
    const char *stringize(VkImageUsageFlagBits bit);
    const char *stringize(VkImageCreateFlagBits bit);
    const char *stringize(VkSampleCountFlagBits bit);
    const char *stringize(VkQueueFlagBits bit);
    const char *stringize(VkMemoryPropertyFlagBits bit);
    const char *stringize(VkMemoryHeapFlagBits bit);
    const char *stringize(VkDeviceQueueCreateFlagBits bit);
    const char *stringize(VkPipelineStageFlagBits bit);
    const char *stringize(VkImageAspectFlagBits bit);
    const char *stringize(VkSparseImageFormatFlagBits bit);
    const char *stringize(VkSparseMemoryBindFlagBits bit);
    const char *stringize(VkFenceCreateFlagBits bit);
    const char *stringize(VkQueryPipelineStatisticFlagBits bit);
    const char *stringize(VkQueryResultFlagBits bit);
    const char *stringize(VkBufferCreateFlagBits bit);
    const char *stringize(VkBufferUsageFlagBits bit);
    const char *stringize(VkPipelineCreateFlagBits bit);
    const char *stringize(VkShaderStageFlagBits flags);
    const char *stringize(VkCullModeFlagBits flags);
    const char *stringize(VkColorComponentFlagBits bit);
    const char *stringize(VkDescriptorSetLayoutCreateFlagBits bit);
    const char *stringize(VkDescriptorPoolCreateFlagBits bit);
    const char *stringize(VkAttachmentDescriptionFlagBits bit);
    const char *stringize(VkAccessFlagBits bit);
    const char *stringize(VkCommandPoolCreateFlagBits bit);
    const char *stringize(VkCommandPoolResetFlagBits bit);
    const char *stringize(VkCommandBufferUsageFlagBits bit);
    const char *stringize(VkQueryControlFlagBits bit);
    const char *stringize(VkCommandBufferResetFlagBits bit);
    const char *stringize(VkStencilFaceFlagBits flags);
    const char *stringize(VkColorSpaceKHR param);
    const char *stringize(VkPresentModeKHR param);
    const char *stringize(VkSurfaceTransformFlagBitsKHR bit);
    const char *stringize(VkCompositeAlphaFlagBitsKHR bit);
} // namespace magma

#endif // MAGMA_DEBUG
