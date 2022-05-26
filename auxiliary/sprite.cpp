/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../objects/srcTransferBuffer.h"
#include "../objects/commandBuffer.h"
#include "../misc/format.h"
#include "../core/copyMemory.h"
#include "../helpers/mapScoped.h"

namespace magma
{
namespace aux
{
Sprite::Sprite(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    std::shared_ptr<const SrcTransferBuffer> buffer, VkDeviceSize offset,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    Image(cmdBuffer->getDevice(), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        1, // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, // Transfer only
        0, // flags
        sharing,
        std::move(allocator)),
    x(0), y(0),
    width(extent.width),
    height(extent.height),
    topLeft{0, 0},
    bottomRight{static_cast<int32_t>(width), static_cast<int32_t>(height)}
{
    const Format imageFormat(format);
    if (imageFormat.blockCompressed())
    {
        const std::pair<int, int> footprint = imageFormat.blockFootprint();
        bottomRight.x *= footprint.first;
        bottomRight.y *= footprint.second;
    }
    const CopyLayout bufferLayout = {offset, 0, 0};
    constexpr VkOffset3D imageOffset{0, 0, 0};
    copyMipLevel(std::move(cmdBuffer), 0, std::move(buffer), bufferLayout, imageOffset,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT,
        false); // User is responsible for submitting command buffer to the graphics queue!
}

Sprite::Sprite(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(cmdBuffer->getDevice(), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        1, // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, // Transfer only
        0, // flags
        sharing,
        std::move(allocator)),
    x(0), y(0),
    width(extent.width),
    height(extent.height),
    topLeft{0, 0},
    bottomRight{static_cast<int32_t>(width), static_cast<int32_t>(height)}
{
    const Format imageFormat(format);
    if (imageFormat.blockCompressed())
    {
        const std::pair<int, int> footprint = imageFormat.blockFootprint();
        bottomRight.x *= footprint.first;
        bottomRight.y *= footprint.second;
    }
    auto buffer = std::make_shared<SrcTransferBuffer>(device, size, nullptr, std::move(allocator), 0, sharing);
    helpers::mapScoped<uint8_t>(buffer,
        [size, data, &copyFn](uint8_t *buffer)
        {   // Copy bitmap data to host visible buffer
            if (!copyFn)
                copyFn = core::copyMemory;
            copyFn(buffer, data, size);
        });
    constexpr CopyLayout bufferLayout = {0, 0, 0};
    constexpr VkOffset3D imageOffset{0, 0, 0};
    copyMipLevel(std::move(cmdBuffer), 0, std::move(buffer), bufferLayout, imageOffset,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT,
        true); // Submit command buffer to the graphics queue as transfer buffer is going to be destroyed
}

void Sprite::blit(std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<Image> dstImage,
    VkFilter filter /* VK_FILTER_NEAREST */) const noexcept
{
    const VkExtent3D dstExtent = dstImage->getMipExtent(0);
    if (inBounds(dstExtent))
    {
        VkImageBlit blitRegion;
        blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.srcSubresource.mipLevel = 0;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcOffsets[0] = {topLeft.x, topLeft.y, 0};
        blitRegion.srcOffsets[1] = {bottomRight.x, bottomRight.y, 1};
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
        cmdBuffer->blitImage(shared_from_this(), dstImage, blitRegion, filter);
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
} // namespace aux
} // namespace magma
