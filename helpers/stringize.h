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

namespace magma
{
    namespace helpers
    {
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
        const char *stringize(VkDeviceQueueCreateFlagBits) noexcept;
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
        const char *stringize(VkDescriptorSetLayoutCreateFlagBits) noexcept;
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
    } // namespace helpers
} // namespace magma
