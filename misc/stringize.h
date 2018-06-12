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
    typedef const char *MAGMA_STRING;

    MAGMA_STRING stringize(VkSystemAllocationScope param);
    MAGMA_STRING stringize(VkInternalAllocationType param);
    MAGMA_STRING stringize(VkFormat param);
    MAGMA_STRING stringize(VkImageType param);  
    MAGMA_STRING stringize(VkImageTiling param);
    MAGMA_STRING stringize(VkPhysicalDeviceType param);
    MAGMA_STRING stringize(VkQueryType param);
    MAGMA_STRING stringize(VkSharingMode param);
    MAGMA_STRING stringize(VkImageLayout param);
    MAGMA_STRING stringize(VkImageViewType param);
    MAGMA_STRING stringize(VkComponentSwizzle param);
    MAGMA_STRING stringize(VkVertexInputRate param);
    MAGMA_STRING stringize(VkPrimitiveTopology param);
    MAGMA_STRING stringize(VkPolygonMode param);
    MAGMA_STRING stringize(VkFrontFace param);
    MAGMA_STRING stringize(VkCompareOp param);
    MAGMA_STRING stringize(VkStencilOp param);
    MAGMA_STRING stringize(VkLogicOp param);
    MAGMA_STRING stringize(VkBlendFactor param);
    MAGMA_STRING stringize(VkBlendOp param);
    MAGMA_STRING stringize(VkDynamicState param);
    MAGMA_STRING stringize(VkFilter param);
    MAGMA_STRING stringize(VkSamplerMipmapMode param);
    MAGMA_STRING stringize(VkSamplerAddressMode param);
    MAGMA_STRING stringize(VkBorderColor param);
    MAGMA_STRING stringize(VkDescriptorType param);
    MAGMA_STRING stringize(VkAttachmentLoadOp param);
    MAGMA_STRING stringize(VkAttachmentStoreOp param); 
    MAGMA_STRING stringize(VkPipelineBindPoint param);
    MAGMA_STRING stringize(VkCommandBufferLevel param);
    MAGMA_STRING stringize(VkIndexType param);
    MAGMA_STRING stringize(VkSubpassContents param);
    MAGMA_STRING stringize(VkObjectType param);
    MAGMA_STRING stringize(VkFormatFeatureFlagBits bit);
    MAGMA_STRING stringize(VkImageUsageFlagBits bit);
    MAGMA_STRING stringize(VkImageCreateFlagBits bit);
    MAGMA_STRING stringize(VkSampleCountFlagBits bit);
    MAGMA_STRING stringize(VkQueueFlagBits bit);
    MAGMA_STRING stringize(VkMemoryPropertyFlagBits bit);
    MAGMA_STRING stringize(VkMemoryHeapFlagBits bit);
    MAGMA_STRING stringize(VkDeviceQueueCreateFlagBits bit);
    MAGMA_STRING stringize(VkPipelineStageFlagBits bit);
    MAGMA_STRING stringize(VkImageAspectFlagBits bit);
    MAGMA_STRING stringize(VkSparseImageFormatFlagBits bit);
    MAGMA_STRING stringize(VkSparseMemoryBindFlagBits bit);
    MAGMA_STRING stringize(VkFenceCreateFlagBits bit);
    MAGMA_STRING stringize(VkQueryPipelineStatisticFlagBits bit);
    MAGMA_STRING stringize(VkQueryResultFlagBits bit);
    MAGMA_STRING stringize(VkBufferCreateFlagBits bit);
    MAGMA_STRING stringize(VkBufferUsageFlagBits bit);
    MAGMA_STRING stringize(VkPipelineCreateFlagBits bit);
    MAGMA_STRING stringize(VkShaderStageFlagBits flags);
    MAGMA_STRING stringize(VkCullModeFlagBits flags);
    MAGMA_STRING stringize(VkColorComponentFlagBits bit);
    MAGMA_STRING stringize(VkDescriptorSetLayoutCreateFlagBits bit);
    MAGMA_STRING stringize(VkDescriptorPoolCreateFlagBits bit);
    MAGMA_STRING stringize(VkAttachmentDescriptionFlagBits bit);
    MAGMA_STRING stringize(VkAccessFlagBits bit);
    MAGMA_STRING stringize(VkCommandPoolCreateFlagBits bit);
    MAGMA_STRING stringize(VkCommandPoolResetFlagBits bit);
    MAGMA_STRING stringize(VkCommandBufferUsageFlagBits bit);
    MAGMA_STRING stringize(VkQueryControlFlagBits bit);
    MAGMA_STRING stringize(VkCommandBufferResetFlagBits bit);
    MAGMA_STRING stringize(VkStencilFaceFlagBits flags);
    MAGMA_STRING stringize(VkColorSpaceKHR param);
    MAGMA_STRING stringize(VkPresentModeKHR param);
    MAGMA_STRING stringize(VkSurfaceTransformFlagBitsKHR bit);
    MAGMA_STRING stringize(VkCompositeAlphaFlagBitsKHR bit);
} // namespace magma

#endif // MAGMA_DEBUG
