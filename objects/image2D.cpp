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
#include "srcTransferBuffer.h"
#include "deviceMemory.h"
#include "../mem/copyMemory.h"
#include "../helpers/mapScoped.h"

namespace magma
{
Image2D::Image2D(std::shared_ptr<Device> device, VkFormat format,
    const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples, VkImageUsageFlags usage,
    std::shared_ptr<IAllocator> allocator):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1}, mipLevels, 1, samples, usage, 0, std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<Device> device, VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image(std::move(device), handle, VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1})
{}

Image2D::Image2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent2D& extent, uint32_t mipLevels,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{extent.width, extent.height, 1}, mipLevels, 1, 1,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 0, std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<Device> device,
    VkFormat format,
    const std::vector<VkExtent2D>& mipExtents,
    const std::vector<const void *>& mipData,
    const std::vector<VkDeviceSize>& mipSizes,
    std::shared_ptr<CommandBuffer> cmdBuffer,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{mipExtents[0].width, mipExtents[0].height, 1},
        static_cast<uint32_t>(mipExtents.size()), // mipLevels
        1, // arrayLayers
        1, // samples
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        0,
        std::move(allocator))
{
    VkDeviceSize size;
    const std::vector<VkBufferImageCopy> copyRegions = getCopyRegions(mipExtents, mipSizes, &size);
    // Copy mip levels to host visible buffer
    std::shared_ptr<SrcTransferBuffer> srcBuffer(std::make_shared<SrcTransferBuffer>(this->device, size, 0, allocator));
    helpers::mapScoped<uint8_t>(srcBuffer, [&](uint8_t *data)
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
    copyFromBuffer(srcBuffer, copyRegions, cmdBuffer);
}

ColorAttachment2D::ColorAttachment2D(std::shared_ptr<Device> device,
    VkFormat colorFormat,
    const VkExtent2D& extent,
    uint32_t mipLevels,
    uint32_t samples,
    bool sampled /* true */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image2D(std::move(device), colorFormat, extent, mipLevels, samples,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0),
        std::move(allocator))
{}

DepthStencilAttachment2D::DepthStencilAttachment2D(std::shared_ptr<Device> device,
    VkFormat depthStencilFormat,
    const VkExtent2D& extent,
    uint32_t mipLevels,
    uint32_t samples,
    bool sampled /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image2D(std::move(device), depthStencilFormat, extent, mipLevels, samples,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0),
        std::move(allocator))
{}

SwapchainColorAttachment2D::SwapchainColorAttachment2D(std::shared_ptr<Device> device,
    VkImage handle, VkFormat format, const VkExtent2D& extent):
    Image2D(std::move(device), handle, format, extent)
{}

SwapchainColorAttachment2D::~SwapchainColorAttachment2D()
{
    // vkDestroyImage() shouldn't have effect on it
    // as it was not created via vkCreateImage()
    handle = VK_NULL_HANDLE;
}
} // namespace magma
