/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "deviceMemory.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"

namespace magma
{
Image2D::Image2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent, uint32_t mipLevels,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        mipLevels,
        1, // arrayLayers
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0, // flags
        sharing,
        std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    std::shared_ptr<const SrcTransferBuffer> buffer, const MipmapLayout& mipOffsets,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool flush /* true */):
    Image(std::move(cmdBuffer->getDevice()), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        MAGMA_COUNT(mipOffsets), // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0, // flags
        sharing,
        std::move(allocator))
{
    const auto copyRegions = setupCopyRegions(mipOffsets, bufferLayout);
    copyTransfer(std::move(cmdBuffer), std::move(buffer), copyRegions, flush);
}

Image2D::Image2D(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    const MipmapData& mipData, const MipmapLayout& mipSizes,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(std::move(cmdBuffer->getDevice()), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        MAGMA_COUNT(mipSizes), // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0, // flags
        sharing,
        std::move(allocator))
{   // Calculate aligned size and mip offsets
    VkDeviceSize bufferSize = 0;
    const auto mipOffsets = setupMipOffsets(mipSizes, bufferSize);
    const auto copyRegions = setupCopyRegions(mipOffsets, {0, 0, 0});
    // Copy mip levels to host visible buffer
    auto buffer = std::make_shared<SrcTransferBuffer>(this->device, bufferSize, nullptr, 0, sharing, allocator);
    helpers::mapScoped<uint8_t>(buffer, [&](uint8_t *data)
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
    copyTransfer(std::move(cmdBuffer), std::move(buffer), copyRegions, true);
}

Image2D::Image2D(std::shared_ptr<Device> device,
    VkFormat format, const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples,
    VkImageTiling tiling, VkImageUsageFlags usage, const Sharing& sharing,
    std::shared_ptr<IAllocator> allocator):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        mipLevels,
        1, // arrayLayers
        samples,
        tiling,
        usage,
        0, // flags
        sharing,
        std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<Device> device, VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image(std::move(device), handle, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1})
{}

LinearTiledImage2D::LinearTiledImage2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image2D(std::move(device), format, extent,
        1, // mipLevels,
        1, // samples
        VK_IMAGE_TILING_LINEAR,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        sharing,
        std::move(allocator))
{}
} // namespace magma
