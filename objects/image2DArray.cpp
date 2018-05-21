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
#include "image2DArray.h"
#include "transferBuffer.h"
#include "deviceMemory.h"
#include "../mem/copyMemory.h"

namespace magma
{
Image2DArray::Image2DArray(std::shared_ptr<const Device> device, VkFormat format, 
    const VkExtent2D& extent, uint32_t mipLevels, uint32_t arrayLayers, VkImageUsageFlags usage,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1}, mipLevels, arrayLayers, 1, usage, 0, allocator)
{}

Image2DArray::Image2DArray(std::shared_ptr<const Device> device, 
    VkFormat format,
    const std::vector<VkExtent2D>& mipExtents,
    const std::vector<std::vector<const void *>>& layersMipData,
    const std::vector<VkDeviceSize>& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{mipExtents[0].width, mipExtents[0].height, 1},
        static_cast<uint32_t>(mipExtents.size()), // mipLevels
        static_cast<uint32_t>(layersMipData.size()), // arrayLayers 
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0, // flags
        allocator)
{
    std::vector<VkBufferImageCopy> copyRegions;
    VkDeviceSize size = getCopyRegions(mipExtents, mipSizes, copyRegions);
    // Copy array layers to host visible buffer
    std::shared_ptr<SourceTransferBuffer> srcBuffer(std::make_shared<SourceTransferBuffer>(device, size, 0, allocator));
    if (uint8_t *data = reinterpret_cast<uint8_t *>(srcBuffer->getMemory()->map()))
    {
        if (!copyFn)
            copyFn = copyMemory;
        for (uint32_t layer = 0; layer < arrayLayers; ++layer)
        {
            for (uint32_t level = 0; level < mipLevels; ++level)
            {
                const VkDeviceSize bufferOffset = copyRegions[layer * mipLevels + level].bufferOffset;
                void *mipLevel = data + bufferOffset;
                copyFn(mipLevel, layersMipData[layer][level], static_cast<size_t>(mipSizes[level]));
            }
        }
        srcBuffer->getMemory()->unmap();
    }
    copyFromBuffer(srcBuffer, copyRegions, cmdBuffer);
}
} // namespace magma
