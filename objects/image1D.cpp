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
#include "image1D.h"
#include "srcTransferBuffer.h"
#include "deviceMemory.h"
#include "../mem/copyMemory.h"
#include "../helpers/mapScoped.h"

namespace magma
{
Image1D::Image1D(std::shared_ptr<Device> device, VkFormat format, uint32_t width, uint32_t mipLevels,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_1D, format, VkExtent3D{width, 1, 1}, 
        mipLevels, 
        1, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
        0, // flags
        std::move(allocator))
{}

Image1D::Image1D(std::shared_ptr<Device> device, VkFormat format, uint32_t width, 
    std::shared_ptr<Buffer> buffer, VkDeviceSize bufferOffset, const ImageMipmapLayout& mipOffsets,
    std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_1D, format, VkExtent3D{width, 1, 1}, 
        MAGMA_COUNT(mipOffsets), // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
        0, 
        std::move(allocator))
{
    const auto copyRegions = buildCopyRegions(mipOffsets, bufferOffset);
    copyFromBuffer(buffer, copyRegions, cmdBuffer);
}

Image1D::Image1D(std::shared_ptr<Device> device, VkFormat format, uint32_t width,
    const ImageMipmapData& mipData, const ImageMipmapLayout& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer, std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_1D, format, VkExtent3D{width, 1, 1},
        MAGMA_COUNT(mipSizes), // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0,
        std::move(allocator))
{   // Calculate aligned size and mip offsets
    VkDeviceSize bufferSize = 0;
    const auto mipOffsets = buildMipOffsets(mipSizes, bufferSize);
    const auto copyRegions = buildCopyRegions(mipOffsets, 0);
    // Copy mip levels to host visible buffer
    std::shared_ptr<SrcTransferBuffer> buffer(std::make_shared<SrcTransferBuffer>(this->device, bufferSize, 0, allocator));
    helpers::mapScoped<uint8_t>(buffer, [&](uint8_t *data)
    {
        if (!copyFn)
            copyFn = copyMemory;
        for (uint32_t level = 0; level < mipLevels; ++level)
        {
            const VkDeviceSize bufferOffset = copyRegions[level].bufferOffset;
            void *mipLevel = data + bufferOffset;
            copyFn(mipLevel, mipData[level], static_cast<size_t>(mipSizes[level]));
        }
    });
    copyFromBuffer(buffer, copyRegions, cmdBuffer);
}
} // namespace magma
