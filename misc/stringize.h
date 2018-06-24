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

    MAGMA_STRING stringize(VkSystemAllocationScope) noexcept;
    MAGMA_STRING stringize(VkInternalAllocationType) noexcept;
    MAGMA_STRING stringize(VkFormat) noexcept;
    MAGMA_STRING stringize(VkImageType) noexcept;  
    MAGMA_STRING stringize(VkImageTiling) noexcept;
    MAGMA_STRING stringize(VkPhysicalDeviceType) noexcept;
    MAGMA_STRING stringize(VkQueryType) noexcept;
    MAGMA_STRING stringize(VkSharingMode) noexcept;
    MAGMA_STRING stringize(VkImageLayout) noexcept;
    MAGMA_STRING stringize(VkImageViewType) noexcept;
    MAGMA_STRING stringize(VkComponentSwizzle) noexcept;
    MAGMA_STRING stringize(VkVertexInputRate) noexcept;
    MAGMA_STRING stringize(VkPrimitiveTopology) noexcept;
    MAGMA_STRING stringize(VkPolygonMode) noexcept;
    MAGMA_STRING stringize(VkFrontFace) noexcept;
    MAGMA_STRING stringize(VkCompareOp) noexcept;
    MAGMA_STRING stringize(VkStencilOp) noexcept;
    MAGMA_STRING stringize(VkLogicOp) noexcept;
    MAGMA_STRING stringize(VkBlendFactor) noexcept;
    MAGMA_STRING stringize(VkBlendOp) noexcept;
    MAGMA_STRING stringize(VkDynamicState) noexcept;
    MAGMA_STRING stringize(VkFilter) noexcept;
    MAGMA_STRING stringize(VkSamplerMipmapMode) noexcept;
    MAGMA_STRING stringize(VkSamplerAddressMode) noexcept;
    MAGMA_STRING stringize(VkBorderColor) noexcept;
    MAGMA_STRING stringize(VkDescriptorType) noexcept;
    MAGMA_STRING stringize(VkAttachmentLoadOp) noexcept;
    MAGMA_STRING stringize(VkAttachmentStoreOp) noexcept; 
    MAGMA_STRING stringize(VkPipelineBindPoint) noexcept;
    MAGMA_STRING stringize(VkCommandBufferLevel) noexcept;
    MAGMA_STRING stringize(VkIndexType) noexcept;
    MAGMA_STRING stringize(VkSubpassContents) noexcept;
    MAGMA_STRING stringize(VkObjectType) noexcept;
    MAGMA_STRING stringize(VkFormatFeatureFlagBits) noexcept;
    MAGMA_STRING stringize(VkImageUsageFlagBits) noexcept;
    MAGMA_STRING stringize(VkImageCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkSampleCountFlagBits) noexcept;
    MAGMA_STRING stringize(VkQueueFlagBits) noexcept;
    MAGMA_STRING stringize(VkMemoryPropertyFlagBits) noexcept;
    MAGMA_STRING stringize(VkMemoryHeapFlagBits) noexcept;
    MAGMA_STRING stringize(VkDeviceQueueCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkPipelineStageFlagBits) noexcept;
    MAGMA_STRING stringize(VkImageAspectFlagBits) noexcept;
    MAGMA_STRING stringize(VkSparseImageFormatFlagBits) noexcept;
    MAGMA_STRING stringize(VkSparseMemoryBindFlagBits) noexcept;
    MAGMA_STRING stringize(VkFenceCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkQueryPipelineStatisticFlagBits) noexcept;
    MAGMA_STRING stringize(VkQueryResultFlagBits) noexcept;
    MAGMA_STRING stringize(VkBufferCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkBufferUsageFlagBits) noexcept;
    MAGMA_STRING stringize(VkPipelineCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkShaderStageFlagBits) noexcept;
    MAGMA_STRING stringize(VkCullModeFlagBits) noexcept;
    MAGMA_STRING stringize(VkColorComponentFlagBits) noexcept;
    MAGMA_STRING stringize(VkDescriptorSetLayoutCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkDescriptorPoolCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkAttachmentDescriptionFlagBits) noexcept;
    MAGMA_STRING stringize(VkAccessFlagBits) noexcept;
    MAGMA_STRING stringize(VkCommandPoolCreateFlagBits) noexcept;
    MAGMA_STRING stringize(VkCommandPoolResetFlagBits) noexcept;
    MAGMA_STRING stringize(VkCommandBufferUsageFlagBits) noexcept;
    MAGMA_STRING stringize(VkQueryControlFlagBits) noexcept;
    MAGMA_STRING stringize(VkCommandBufferResetFlagBits) noexcept;
    MAGMA_STRING stringize(VkStencilFaceFlagBits) noexcept;
    MAGMA_STRING stringize(VkColorSpaceKHR) noexcept;
    MAGMA_STRING stringize(VkPresentModeKHR) noexcept;
    MAGMA_STRING stringize(VkSurfaceTransformFlagBitsKHR) noexcept;
    MAGMA_STRING stringize(VkCompositeAlphaFlagBitsKHR) noexcept;
} // namespace magma

#endif // MAGMA_DEBUG
