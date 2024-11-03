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
#include "layoutTransition.h"
#include "../objects/image.h"
#include "../objects/commandBuffer.h"
#include "../misc/finish.h"

namespace magma::helpers
{
void layoutTransition(std::shared_ptr<Image> image, VkImageLayout newLayout,
    const std::unique_ptr<CommandBuffer>& cmdBuffer,
    VkPipelineStageFlags shaderStageMask /* VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT */,
    VkDependencyFlags dependencyFlags /* 0 */)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        image->layoutTransition(newLayout, cmdBuffer, shaderStageMask, dependencyFlags);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(cmdBuffer);
}

void batchLayoutTransition(const std::vector<ImageLayoutTransition>& imageLayouts,
    const std::unique_ptr<CommandBuffer>& cmdBuffer,
    VkPipelineStageFlags shaderStageMask /* VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT */,
    VkDependencyFlags dependencyFlags /* 0 */)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    std::vector<ImageMemoryBarrier> imageMemoryBarriers;
    VkPipelineStageFlags srcStageMask = 0;
    VkPipelineStageFlags dstStageMask = 0;
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        for (const ImageLayoutTransition& it: imageLayouts)
        {
            const VkImageLayout oldLayout = it.image->getLayout(it.baseMipLevel);
            switch (oldLayout)
            {
            case VK_IMAGE_LAYOUT_UNDEFINED:
                srcStageMask |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                break;
            case VK_IMAGE_LAYOUT_GENERAL:
                srcStageMask |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                srcStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
                srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                srcStageMask |= (VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT);
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                srcStageMask |= shaderStageMask;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                srcStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
                break;
            case VK_IMAGE_LAYOUT_PREINITIALIZED:
                srcStageMask |= VK_PIPELINE_STAGE_HOST_BIT;
                break;
        #ifdef VK_KHR_swapchain
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            #ifdef VK_KHR_shared_presentable_image
            case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
            #endif
                srcStageMask |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                break;
        #endif // VK_KHR_swapchain
        #ifdef VK_EXT_fragment_density_map
            case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
                srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
        #endif // VK_EXT_fragment_density_map
        #ifdef VK_KHR_fragment_shading_rate
            case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
                srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
        #endif // VK_KHR_fragment_shading_rate
            default:
                MAGMA_FAILURE("unknown old image layout");
            }
            const VkImageUsageFlags usage = it.image->getUsage();
            switch (it.newLayout)
            {
            case VK_IMAGE_LAYOUT_GENERAL:
                dstStageMask |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
                dstStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
                dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                break;
        #ifdef VK_KHR_maintenance2
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        #endif // VK_KHR_maintenance2
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
                MAGMA_ASSERT(usage & (VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT));
                // Taken from UE4.2x, file VulkanBarriers.cpp
                dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
                    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                    VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                MAGMA_ASSERT(usage & (VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT));
                dstStageMask |= shaderStageMask;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
                dstStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT);
                dstStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
                break;
        #ifdef VK_KHR_swapchain
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            #ifdef VK_KHR_shared_presentable_image
            case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
            #endif
                // When transitioning the image to VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR
                // or VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, there is no need to delay
                // subsequent processing, or perform any visibility operations
                // (as vkQueuePresentKHR performs automatic visibility operations).
                // To achieve this, the dstAccessMask member of the VkImageMemoryBarrier
                // should be set to 0, and the dstStageMask parameter should be set to
                // VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT.
                dstStageMask |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                break;
        #endif // VK_KHR_swapchain
        #ifdef VK_EXT_fragment_density_map
            case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT);
                dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
        #endif // VK_EXT_fragment_density_map
        #ifdef VK_KHR_fragment_shading_rate
            case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
                MAGMA_ASSERT(usage & VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR);
                dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                break;
        #endif // VK_KHR_fragment_shading_rate
            case VK_IMAGE_LAYOUT_UNDEFINED:
                MAGMA_FAILURE("undefined new image layout");
                break;
            default:
                MAGMA_FAILURE("unknown new image layout");
            }
            VkImageSubresourceRange subresourceRange = it.image->getSubresourceRange(it.baseMipLevel, it.baseArrayLayer);
            imageMemoryBarriers.emplace_back(it.image.get(), it.newLayout, subresourceRange);
        }
    }
    cmdBuffer->pipelineBarrier(srcStageMask, dstStageMask, imageMemoryBarriers, dependencyFlags);
    cmdBuffer->end();
    // Block until execution is complete
    finish(cmdBuffer);
}
} // namespace magma::helpers
