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
#include "image2D.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "../misc/format.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"

namespace magma
{
Image2D::Image2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent, uint32_t mipLevels,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        mipLevels,
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional,
        sharing,
        std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    std::shared_ptr<const SrcTransferBuffer> srcBuffer, const MipmapLayout& mipOffsets,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image(srcBuffer->getDevice(), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        MAGMA_COUNT(mipOffsets), // mipLevels
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional,
        sharing,
        std::move(allocator))
{
    const auto copyRegions = setupCopyRegions(mipOffsets, bufferLayout);
    copyTransfer(std::move(cmdBuffer), std::move(srcBuffer), copyRegions,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

Image2D::Image2D(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    const MipmapData& mipData, const MipmapLayout& mipSizes,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(cmdBuffer->getDevice(), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        MAGMA_COUNT(mipSizes), // mipLevels
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional,
        sharing,
        allocator)
{   // Calculate aligned size and mip offsets
    VkDeviceSize bufferSize = 0;
    const auto mipOffsets = setupMipOffsets(mipSizes, bufferSize);
    const auto copyRegions = setupCopyRegions(mipOffsets, {0, 0, 0});
    // Copy mip levels to host visible buffer
    auto srcBuffer = std::make_shared<SrcTransferBuffer>(device, bufferSize, nullptr, std::move(allocator), Buffer::Descriptor(), sharing);
    helpers::mapScoped<uint8_t>(srcBuffer,
        [&](uint8_t *data)
        {
            if (!copyFn)
                copyFn = core::copyMemory;
            for (uint32_t level = 0; level < mipLevels; ++level)
            {
                const VkDeviceSize bufferOffset = copyRegions[level].bufferOffset;
                void *mipLevel = data + bufferOffset;
                copyFn(mipLevel, mipData[level], static_cast<std::size_t>(mipSizes[level]));
            }
        });
    // Copy buffer to image
    cmdBuffer->begin();
    copyTransfer(cmdBuffer, srcBuffer, copyRegions,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

Image2D::Image2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    uint32_t mipLevels, uint32_t arrayLayers, uint32_t samples,
    VkImageCreateFlags flags, VkImageUsageFlags usage, VkImageTiling tiling,
    const Descriptor& optional, const Sharing& sharing, std::shared_ptr<Allocator> allocator):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        mipLevels,
        arrayLayers,
        samples,
        flags,
        usage,
        tiling,
        optional,
        sharing,
        std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<Device> device, VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image(std::move(device), handle, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1})
{}

LinearTiledImage2D::LinearTiledImage2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(std::move(device), format, extent,
        1, // mipLevels,
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_TILING_LINEAR,
        optional,
        sharing,
        std::move(allocator))
{}

StorageImage2D::StorageImage2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(std::move(device), format, extent,
        mipLevels,
        1, // arrayLayers
        samples,
        0, // flags
        VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional,
        sharing,
        std::move(allocator))
{}

MutableImage2D::MutableImage2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent, uint32_t mipLevels,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    MutableImage(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        mipLevels,
        1, // arrayLayers
        1, // samples
        0, // flags
        optional,
        sharing,
        std::move(allocator))
{}
} // namespace magma
