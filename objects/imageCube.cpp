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
#include "imageCube.h"
#include "srcTransferBuffer.h"
#include "deviceMemory.h"
#include "../helpers/mapScoped.h"
#include "../internal/copyMemory.h"

namespace magma
{
ImageCube::ImageCube(std::shared_ptr<Device> device, VkFormat format, uint32_t dimension,
    uint32_t mipLevels, std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{dimension, dimension, 1}, mipLevels,
        6, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        std::move(allocator))
{}

ImageCube::ImageCube(std::shared_ptr<Device> device, VkFormat format, uint32_t dimension, uint32_t mipLevels,
    std::shared_ptr<Buffer> buffer, VkDeviceSize bufferOffset, const ImageMipmapLayout& mipOffsets, 
    std::shared_ptr<CommandBuffer> cmdBuffer, 
    std::shared_ptr<IAllocator> allocator /* nullptr */, 
    bool flush /* true */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{dimension, dimension, 1},
        mipLevels,
        6, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        std::move(allocator))
{
    const auto copyRegions = buildCopyRegions(mipOffsets, bufferOffset);
    copyFromBuffer(buffer, copyRegions, cmdBuffer, flush);
}

ImageCube::ImageCube(std::shared_ptr<Device> device, VkFormat format, uint32_t dimension,
    const ImageMipmapData mipData[6], const ImageMipmapLayout& mipSizes, 
    std::shared_ptr<CommandBuffer> cmdBuffer, 
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{dimension, dimension, 1},
        MAGMA_COUNT(mipSizes), // mipLevels
        6, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        std::move(allocator))
{   // Calculate aligned size and mip offsets
    VkDeviceSize bufferSize = 0;
    const auto mipOffsets = buildMipOffsets(mipSizes, bufferSize);
    const auto copyRegions = buildCopyRegions(mipOffsets, 0);
    // Copy array layers to host visible buffer
    std::shared_ptr<SrcTransferBuffer> buffer(std::make_shared<SrcTransferBuffer>(this->device, bufferSize, 0, allocator));
    helpers::mapScoped<uint8_t>(buffer, [&](uint8_t *data)
    {
        if (!copyFn)
            copyFn = internal::copyMemory;
        for (uint32_t face = 0; face < arrayLayers; ++face)
        {
            for (uint32_t level = 0; level < mipLevels; ++level)
            {
                const VkDeviceSize bufferOffset = copyRegions[face * mipLevels + level].bufferOffset;
                void *mipLevel = data + bufferOffset;
                copyFn(mipLevel, mipData[face][level], static_cast<size_t>(mipSizes[level]));
            }
        }
    });
    copyFromBuffer(buffer, copyRegions, cmdBuffer, true);
}
} // namespace magma
