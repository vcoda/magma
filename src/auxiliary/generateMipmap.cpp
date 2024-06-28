/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/commandBuffer.h"
#include "../objects/image.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../misc/deviceFeatures.h"
#include "../misc/imageSubresourceRange.h"

namespace magma
{
namespace aux
{
bool generateMipmap(std::shared_ptr<Image> image, uint32_t baseMipLevel, VkFilter filter,
    std::shared_ptr<CommandBuffer> cmdBuffer) noexcept
{
    MAGMA_ASSERT(image);
    MAGMA_ASSERT(image->getUsage() & (VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT));
    MAGMA_ASSERT(cmdBuffer);
    MAGMA_ASSERT(cmdBuffer->getState() == CommandBuffer::State::Recording);
    if (!image || !(image->getUsage() & (VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)))
        return false;
    if (!cmdBuffer || cmdBuffer->getState() != CommandBuffer::State::Recording)
        return false;
    std::shared_ptr<DeviceFeatures> deviceFeatures = image->getDevice()->getFeatures();
    if (!deviceFeatures->supportsFormatFeatures(image->getFormat(), VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT).optimal)
        return false;
    // Store image layouts of mip chain
    const bool hadUniformLayout = image->hasUniformLayout();
    MAGMA_STACK_ARRAY(VkImageLayout, oldLayouts, image->getMipLevels() - baseMipLevel);
    for (uint32_t level = baseMipLevel; level < image->getMipLevels(); ++level)
        oldLayouts.put(image->getLayout(level));
    const VkImageAspectFlags aspectMask = image->getAspectMask();
    VkExtent3D prevMipExtent = image->calculateMipExtent(baseMipLevel);
    // Transition of base mip level to transfer src optimal layout
    const ImageSubresourceRange baseMipRange(image, baseMipLevel, 1);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        ImageMemoryBarrier(image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, baseMipRange));
    for (uint32_t level = baseMipLevel + 1; level < image->getMipLevels(); ++level)
    {
        const VkExtent3D nextMipExtent = image->calculateMipExtent(level);
        VkImageBlit blitRegion;
        blitRegion.srcSubresource.aspectMask = aspectMask;
        blitRegion.srcSubresource.mipLevel = level - 1;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcOffsets[0] = {0, 0, 0};
        blitRegion.srcOffsets[1].x = static_cast<int32_t>(prevMipExtent.width);
        blitRegion.srcOffsets[1].y = static_cast<int32_t>(prevMipExtent.height);
        blitRegion.srcOffsets[1].z = 1;
        blitRegion.dstSubresource.aspectMask = aspectMask;
        blitRegion.dstSubresource.mipLevel = level;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount = 1;
        blitRegion.dstOffsets[0] = {0, 0, 0};
        blitRegion.dstOffsets[1].x = static_cast<int32_t>(nextMipExtent.width);
        blitRegion.dstOffsets[1].y = static_cast<int32_t>(nextMipExtent.height);
        blitRegion.dstOffsets[1].z = 1;
        const ImageSubresourceRange nextMipRange(image, level, 1);
        // Transition of next mip level to transfer dest optimal layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, nextMipRange));
        // Downsample larger mip to smaller one
        cmdBuffer->blitImage(image, image, blitRegion, filter);
        // Transition of next mip level back to transfer source optimal layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, nextMipRange));
        prevMipExtent = nextMipExtent;
    }
    if (hadUniformLayout) // Restore image layout of mips remaining after <baseMipLevel>
        image->layoutTransitionMipLayer(oldLayouts[0], baseMipLevel, 0, cmdBuffer);
    else
    {   // Restore image layouts for each mip level
        ImageSubresourceRange mipRange(image, baseMipLevel, 1);
        for (uint32_t level = baseMipLevel; level < image->getMipLevels(); ++level)
        {
            const VkImageLayout oldLayout = oldLayouts[level - baseMipLevel];
            if (oldLayout == image->getLayout(level))
                continue;
            mipRange.baseMipLevel = level;
            cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
                ImageMemoryBarrier(image, oldLayout, mipRange));
        }
    }
    return true;
}
} // namespace aux
} // namespace magma
