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
#include "image2D.h"
#include "transferBuffer.h"
#include "../helpers/alignedMemcpy.h"

namespace magma
{
Image2D::Image2D(std::shared_ptr<const Device> device, VkFormat format,
    const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples, VkImageUsageFlags usage):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1}, mipLevels, 1, samples, usage)
{}

Image2D::Image2D(std::shared_ptr<const Device> device, VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image(device, handle, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1})
{}

Image2D::Image2D(std::shared_ptr<const Device> device, 
    VkFormat format,
    const std::vector<VkExtent2D>& mipExtents,
    const std::vector<const void *>& mipData,
    const std::vector<VkDeviceSize>& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer):
    Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{mipExtents[0].width, mipExtents[0].height, 1},
        static_cast<uint32_t>(mipExtents.size()), // mipLevels
        1, // arrayLayers 
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
{
    std::vector<VkBufferImageCopy> copyRegions;
    VkDeviceSize size = getCopyRegions(mipExtents, mipSizes, copyRegions);
    // Copy mip levels to host visible buffer
    std::shared_ptr<SourceTransferBuffer> srcBuffer(new SourceTransferBuffer(device, size));
    if (uint8_t *data = reinterpret_cast<uint8_t *>(srcBuffer->getMemory()->map()))
    {
        for (uint32_t level = 0; level < mipLevels; ++level)
        {
            const VkDeviceSize bufferOffset = copyRegions[level].bufferOffset;
            void *mipLevel = data + bufferOffset;
            helpers::alignedMemcpy(mipLevel, mipData[level], static_cast<size_t>(mipSizes[level]));
        }
        srcBuffer->getMemory()->unmap();
    }
    copyFromBuffer(srcBuffer, copyRegions, cmdBuffer);
}

ColorAttachment2D::ColorAttachment2D(std::shared_ptr<const Device> device,
    VkFormat colorFormat,
    const VkExtent2D& extent,
    uint32_t mipLevels,
    uint32_t samples,
    bool sampled /* true */):
    Image2D(device, colorFormat, extent, mipLevels, samples,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0))
{}

DepthStencilAttachment2D::DepthStencilAttachment2D(std::shared_ptr<const Device> device,
    VkFormat depthStencilFormat,
    const VkExtent2D& extent,
    uint32_t mipLevels,
    uint32_t samples,
    bool sampled /* false */):
    Image2D(device, depthStencilFormat, extent, mipLevels, samples,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0))
{}

SwapchainColorAttachment2D::SwapchainColorAttachment2D(std::shared_ptr<const Device> device,
    VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image2D(device, handle, format, extent)
{}

SwapchainColorAttachment2D::~SwapchainColorAttachment2D()
{
    // vkDestroyImage() shouldn't have effect on it 
    // as it was not created via vkCreateImage()
    handle = VK_NULL_HANDLE;
}
} // namespace magma
