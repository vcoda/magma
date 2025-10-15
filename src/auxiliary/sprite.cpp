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
#include "sprite.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/srcTransferBuffer.h"
#include "../objects/commandBuffer.h"
#include "../misc/deviceFeatures.h"
#include "../misc/format.h"

namespace magma::aux
{
Sprite::Sprite(lent_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    lent_ptr<const SrcTransferBuffer> srcBuffer, VkDeviceSize offset,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    Image(cmdBuffer->getDevice(), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        1, // mipLevels
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, // Transfer only
        VK_IMAGE_TILING_OPTIMAL,
        Initializer(),
        sharing,
        std::move(allocator)),
    x(0), y(0),
    width(extent.width),
    height(extent.height),
    topLeft{0, 0, 0},
    bottomRight{static_cast<int32_t>(width), static_cast<int32_t>(height), 1}
{
    const std::unique_ptr<DeviceFeatures>& features = device->getPhysicalDevice()->features();
    if (!features->supportsFormatFeatures(format, VK_FORMAT_FEATURE_BLIT_SRC_BIT).optimal)
        MAGMA_ERROR("image format doesn't support source blit operations");
    const Format imageFormat(format);
    if (imageFormat.blockCompressed())
    {
        const std::pair<int, int> footprint = imageFormat.blockFootprint();
        bottomRight.x *= footprint.first;
        bottomRight.y *= footprint.second;
    }
    const CopyLayout bufferLayout = {offset, 0, 0};
    constexpr VkOffset3D imageOffset{0, 0, 0};
    copyMipWithTransition(std::move(cmdBuffer), 0, 0, std::move(srcBuffer), bufferLayout, imageOffset,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT);
}

Sprite::Sprite(lent_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent_,
    VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMem /* nullptr */):
    Image(cmdBuffer->getDevice(), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent_.width, extent_.height, 1},
        1, // mipLevels
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, // Transfer only
        VK_IMAGE_TILING_OPTIMAL,
        Initializer(),
        sharing,
        allocator),
    x(0), y(0),
    width(extent.width),
    height(extent.height),
    topLeft{0, 0, 0},
    bottomRight{static_cast<int32_t>(width), static_cast<int32_t>(height), 1}
{
    const std::unique_ptr<DeviceFeatures>& features = device->getPhysicalDevice()->features();
    if (!features->supportsFormatFeatures(format, VK_FORMAT_FEATURE_BLIT_SRC_BIT).optimal)
        MAGMA_ERROR("image format doesn't support source blit operations");
    const Format imageFormat(format);
    if (imageFormat.blockCompressed())
    {
        const std::pair<int, int> footprint = imageFormat.blockFootprint();
        bottomRight.x *= footprint.first;
        bottomRight.y *= footprint.second;
    }
    const Mip level = {extent, size, data};
    copyMipmap(std::move(cmdBuffer), {level}, std::move(allocator), std::move(copyMem),
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_PIPELINE_STAGE_TRANSFER_BIT);
}

void Sprite::blit(lent_ptr<CommandBuffer> cmdBuffer, lent_ptr<Image> dstImage,
    VkFilter filter /* VK_FILTER_NEAREST */) const noexcept
{
    const VkExtent3D dstExtent = dstImage->calculateMipExtent(0);
    if (inBounds(dstExtent))
    {
        VkImageBlit blitRegion;
        blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.srcSubresource.mipLevel = 0;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcOffsets[0] = topLeft;
        blitRegion.srcOffsets[1] = bottomRight;
        blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.dstSubresource.mipLevel = 0;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount = 1;
        blitRegion.dstOffsets[0] = {x, y, 0};
        blitRegion.dstOffsets[1].x = x + static_cast<int32_t>(width);
        blitRegion.dstOffsets[1].y = y + static_cast<int32_t>(height);
        blitRegion.dstOffsets[1].z = 1;
        if (!isScaled()) // We can clip properly only unscaled sprite
        {
            int32_t dstWidth = static_cast<int32_t>(dstExtent.width);
            int32_t dstHeight = static_cast<int32_t>(dstExtent.height);
            clip(blitRegion.srcOffsets, blitRegion.dstOffsets, dstWidth, dstHeight);
        }
        cmdBuffer->blitImage(this, std::move(dstImage), blitRegion, filter);
    }
}

bool Sprite::inBounds(const VkExtent3D& extent) const noexcept
{
    if (-x >= static_cast<int32_t>(width) ||
        -y >= static_cast<int32_t>(height))
        return false;
    if (x >= static_cast<int32_t>(extent.width) ||
        y >= static_cast<int32_t>(extent.height))
        return false;
    return true;
}

void Sprite::clip(VkOffset3D srcOffsets[2], VkOffset3D dstOffsets[2], int32_t dstWidth, int32_t dstHeight) const noexcept
{   // Left
    if (dstOffsets[0].x < 0)
    {
        dstOffsets[0].x = 0;
        srcOffsets[0].x += isFlippedHorizontally() ? x : -x;
    } // Top
    if (dstOffsets[0].y < 0)
    {
        dstOffsets[0].y = 0;
        srcOffsets[0].y += isFlippedVertically() ? y : -y;
    } // Right
    if (dstOffsets[1].x > dstWidth)
    {
        int32_t cx = dstOffsets[1].x - dstWidth;
        dstOffsets[1].x = dstWidth;
        srcOffsets[1].x += isFlippedHorizontally() ? cx : -cx;
    } // Bottom
    if (dstOffsets[1].y > dstHeight)
    {
        int32_t cy = dstOffsets[1].y - dstHeight;
        dstOffsets[1].y = dstHeight;
        srcOffsets[1].y += isFlippedVertically() ? cy : -cy;
    }
}
} // namespace magma::aux
