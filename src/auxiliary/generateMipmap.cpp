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
bool generateMipmap(std::shared_ptr<Image> image, uint32_t baseLevel, VkFilter filter,
    std::shared_ptr<CommandBuffer> cmdBuffer) noexcept
{
    MAGMA_ASSERT(image);
    MAGMA_ASSERT(cmdBuffer);
    if (!image || !cmdBuffer)
        return false;
    std::shared_ptr<DeviceFeatures> deviceFeatures = image->getDevice()->getFeatures();
    if (!deviceFeatures->supportsFormatFeatures(image->getFormat(), VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT).optimal)
        return false;
    VkExtent3D prevMipExtent = image->calculateMipExtent(baseLevel);
    for (uint32_t level = baseLevel + 1; level < image->getMipLevels(); ++level)
    {
        const VkExtent3D nextMipExtent = image->calculateMipExtent(level);
        VkImageBlit blitRegion;
        blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.srcSubresource.mipLevel = level - 1;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcOffsets[0] = {0, 0, 0};
        blitRegion.srcOffsets[1].x = static_cast<int32_t>(prevMipExtent.width);
        blitRegion.srcOffsets[1].y = static_cast<int32_t>(prevMipExtent.height);
        blitRegion.srcOffsets[1].z = 1;
        blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.dstSubresource.mipLevel = level;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount = 1;
        blitRegion.dstOffsets[0] = {0, 0, 0};
        blitRegion.dstOffsets[1].x = static_cast<int32_t>(nextMipExtent.width);
        blitRegion.dstOffsets[1].y = static_cast<int32_t>(nextMipExtent.height);
        blitRegion.dstOffsets[1].z = 1;
        const ImageSubresourceRange nextMipRange(image, level, 1);
        // Transition of next mip level to transfer dest optimal layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, nextMipRange));
        // Downsample larger mip to smaller one
        cmdBuffer->blitImage(image, image, blitRegion, filter);
        // Transition of next mip level back to transfer source optimal layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, nextMipRange));
        prevMipExtent = nextMipExtent;
    }
    // Blitted mip levels are transitioned to shader read only optimal layout
    const ImageSubresourceRange blitMipsRange(image, baseLevel + 1, image->getMipLevels() - baseLevel - 1);
    cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
        ImageMemoryBarrier(image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, blitMipsRange));
    return true;
}
} // namespace aux
} // namespace magma
