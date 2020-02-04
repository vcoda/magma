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
#include "image2DArray.h"
#include "srcTransferBuffer.h"
#include "deviceMemory.h"
#include "../helpers/mapScoped.h"
#include "../detail/copyMemory.h"

namespace magma
{
Image2DArray::Image2DArray(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    uint32_t mipLevels, uint32_t arrayLayers,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
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

Image2DArray::Image2DArray(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent, uint32_t arrayLayers,
    std::shared_ptr<Buffer> buffer, VkDeviceSize bufferOffset, const ImageMipmapLayout& mipOffsets,
    std::shared_ptr<CommandBuffer> cmdBuffer,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool flush /* true */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
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
    const auto copyRegions = buildCopyRegions(mipOffsets, bufferOffset);
    copyFromBuffer(buffer, copyRegions, cmdBuffer, flush);
}

Image2DArray::Image2DArray(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent,
    const ImageArrayMipmapData& mipData, const ImageMipmapLayout& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1},
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
    const auto mipOffsets = buildMipOffsets(mipSizes, bufferSize);
    const auto copyRegions = buildCopyRegions(mipOffsets, 0);
    // Copy array layers to host visible buffer
    auto buffer = std::make_shared<SrcTransferBuffer>(this->device, bufferSize, nullptr, 0, sharing, allocator);
    helpers::mapScoped<uint8_t>(buffer, [&](uint8_t *data)
    {
        if (!copyFn)
            copyFn = detail::copyMemory;
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
    copyFromBuffer(buffer, copyRegions, cmdBuffer, true);
}
} // namespace magma
