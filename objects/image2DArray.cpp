/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "image2DArray.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "deviceMemory.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"

namespace magma
{
Image2DArray::Image2DArray(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    uint32_t mipLevels, uint32_t arrayLayers,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        mipLevels,
        arrayLayers,
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0, // flags
        sharing,
        std::move(allocator))
{}

Image2DArray::Image2DArray(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent, uint32_t arrayLayers,
    std::shared_ptr<const SrcTransferBuffer> buffer, const MipmapLayout& mipOffsets,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    bool flush /* true */):
    Image(std::move(cmdBuffer->getDevice()), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        MAGMA_COUNT(mipOffsets) / arrayLayers, // mipLevels
        arrayLayers,
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0, // flags
        sharing,
        std::move(allocator))
{
    MAGMA_ASSERT(MAGMA_COUNT(mipOffsets) % arrayLayers == 0);
    const auto copyRegions = setupCopyRegions(mipOffsets, bufferLayout);
    copyTransfer(std::move(cmdBuffer), std::move(buffer), copyRegions, flush);
}

Image2DArray::Image2DArray(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const VkExtent2D& extent,
    const ArrayMipmapData& mipData, const MipmapLayout& mipSizes,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(std::move(cmdBuffer->getDevice()), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
        MAGMA_COUNT(mipSizes), // mipLevels
        MAGMA_COUNT(mipData), // arrayLayers
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
    // Copy array layers to host visible buffer
    auto buffer = std::make_shared<SrcTransferBuffer>(this->device, bufferSize, nullptr, std::move(allocator), 0, sharing);
    helpers::mapScoped<uint8_t>(buffer, [&](uint8_t *data)
    {
        if (!copyFn)
            copyFn = core::copyMemory;
        for (uint32_t layer = 0; layer < arrayLayers; ++layer)
        {
            for (uint32_t level = 0; level < mipLevels; ++level)
            {
                const VkDeviceSize bufferOffset = copyRegions[layer * mipLevels + level].bufferOffset;
                void *mipLevel = data + bufferOffset;
                copyFn(mipLevel, mipData[layer][level], static_cast<std::size_t>(mipSizes[level]));
            }
        }
    });
    copyTransfer(std::move(cmdBuffer), std::move(buffer), copyRegions, true);
}
} // namespace magma
