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

    MAGMA_STRING stringize(VkSystemAllocationScope);
    MAGMA_STRING stringize(VkInternalAllocationType);
    MAGMA_STRING stringize(VkFormat);
    MAGMA_STRING stringize(VkImageType);  
    MAGMA_STRING stringize(VkImageTiling);
    MAGMA_STRING stringize(VkPhysicalDeviceType);
    MAGMA_STRING stringize(VkQueryType);
    MAGMA_STRING stringize(VkSharingMode);
    MAGMA_STRING stringize(VkImageLayout);
    MAGMA_STRING stringize(VkImageViewType);
    MAGMA_STRING stringize(VkComponentSwizzle);
    MAGMA_STRING stringize(VkVertexInputRate);
    MAGMA_STRING stringize(VkPrimitiveTopology);
    MAGMA_STRING stringize(VkPolygonMode);
    MAGMA_STRING stringize(VkFrontFace);
    MAGMA_STRING stringize(VkCompareOp);
    MAGMA_STRING stringize(VkStencilOp);
    MAGMA_STRING stringize(VkLogicOp);
    MAGMA_STRING stringize(VkBlendFactor);
    MAGMA_STRING stringize(VkBlendOp);
    MAGMA_STRING stringize(VkDynamicState);
    MAGMA_STRING stringize(VkFilter);
    MAGMA_STRING stringize(VkSamplerMipmapMode);
    MAGMA_STRING stringize(VkSamplerAddressMode);
    MAGMA_STRING stringize(VkBorderColor);
    MAGMA_STRING stringize(VkDescriptorType);
    MAGMA_STRING stringize(VkAttachmentLoadOp);
    MAGMA_STRING stringize(VkAttachmentStoreOp); 
    MAGMA_STRING stringize(VkPipelineBindPoint);
    MAGMA_STRING stringize(VkCommandBufferLevel);
    MAGMA_STRING stringize(VkIndexType);
    MAGMA_STRING stringize(VkSubpassContents);
    MAGMA_STRING stringize(VkObjectType);
    MAGMA_STRING stringize(VkFormatFeatureFlagBits);
    MAGMA_STRING stringize(VkImageUsageFlagBits);
    MAGMA_STRING stringize(VkImageCreateFlagBits);
    MAGMA_STRING stringize(VkSampleCountFlagBits);
    MAGMA_STRING stringize(VkQueueFlagBits);
    MAGMA_STRING stringize(VkMemoryPropertyFlagBits);
    MAGMA_STRING stringize(VkMemoryHeapFlagBits);
    MAGMA_STRING stringize(VkDeviceQueueCreateFlagBits);
    MAGMA_STRING stringize(VkPipelineStageFlagBits);
    MAGMA_STRING stringize(VkImageAspectFlagBits);
    MAGMA_STRING stringize(VkSparseImageFormatFlagBits);
    MAGMA_STRING stringize(VkSparseMemoryBindFlagBits);
    MAGMA_STRING stringize(VkFenceCreateFlagBits);
    MAGMA_STRING stringize(VkQueryPipelineStatisticFlagBits);
    MAGMA_STRING stringize(VkQueryResultFlagBits);
    MAGMA_STRING stringize(VkBufferCreateFlagBits);
    MAGMA_STRING stringize(VkBufferUsageFlagBits);
    MAGMA_STRING stringize(VkPipelineCreateFlagBits);
    MAGMA_STRING stringize(VkShaderStageFlagBits);
    MAGMA_STRING stringize(VkCullModeFlagBits);
    MAGMA_STRING stringize(VkColorComponentFlagBits);
    MAGMA_STRING stringize(VkDescriptorSetLayoutCreateFlagBits);
    MAGMA_STRING stringize(VkDescriptorPoolCreateFlagBits);
    MAGMA_STRING stringize(VkAttachmentDescriptionFlagBits);
    MAGMA_STRING stringize(VkAccessFlagBits);
    MAGMA_STRING stringize(VkCommandPoolCreateFlagBits);
    MAGMA_STRING stringize(VkCommandPoolResetFlagBits);
    MAGMA_STRING stringize(VkCommandBufferUsageFlagBits);
    MAGMA_STRING stringize(VkQueryControlFlagBits);
    MAGMA_STRING stringize(VkCommandBufferResetFlagBits);
    MAGMA_STRING stringize(VkStencilFaceFlagBits);
    MAGMA_STRING stringize(VkColorSpaceKHR);
    MAGMA_STRING stringize(VkPresentModeKHR);
    MAGMA_STRING stringize(VkSurfaceTransformFlagBitsKHR);
    MAGMA_STRING stringize(VkCompositeAlphaFlagBitsKHR);
} // namespace magma

#endif // MAGMA_DEBUG
