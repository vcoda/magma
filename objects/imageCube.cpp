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
#include "../mem/copyMemory.h"
#include "../helpers/mapScoped.h"

namespace magma
{
ImageCube::ImageCube(std::shared_ptr<const Device> device, VkFormat format,
    uint32_t dimension, uint32_t mipLevels, VkImageUsageFlags usage,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{dimension, dimension, 1}, mipLevels, 
        6, // arrayLayers  
        1, // samples
        usage, 
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, 
        allocator)
{}

ImageCube::ImageCube(std::shared_ptr<const Device> device, 
    VkFormat format,
    const std::vector<uint32_t>& mipDimensions,
    const std::vector<const void *> cubeMipData[6],
    const std::vector<VkDeviceSize>& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{mipDimensions[0], mipDimensions[0], 1},
        static_cast<uint32_t>(mipDimensions.size()), // mipLevels
        6, // arrayLayers 
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, 
        allocator)
{
    std::vector<VkExtent2D> mipExtents;
    for (const uint32_t size : mipDimensions)
        mipExtents.push_back(VkExtent2D{size, size});
    std::vector<VkBufferImageCopy> copyRegions;
    const VkDeviceSize size = getCopyRegions(mipExtents, mipSizes, copyRegions);
    // Copy array layers to host visible buffer
    std::shared_ptr<SrcTransferBuffer> srcBuffer(std::make_shared<SrcTransferBuffer>(device, size, 0, allocator));
    helpers::mapScoped<uint8_t>(srcBuffer, [&](uint8_t *data)
    {
        if (!copyFn)
            copyFn = copyMemory;
        for (uint32_t face = 0; face < 6; ++face)
        {
            for (uint32_t level = 0; level < mipLevels; ++level)
            {
                const VkDeviceSize bufferOffset = copyRegions[face * mipLevels + level].bufferOffset;
                void *mipLevel = data + bufferOffset;
                copyFn(mipLevel, cubeMipData[face][level], static_cast<size_t>(mipSizes[level]));
            }
        }
    });
    copyFromBuffer(srcBuffer, copyRegions, cmdBuffer);
}
} // namespace magma
