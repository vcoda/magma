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
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"
#include "../core/forEach.h"

namespace magma
{
Image2D::Image2D(std::shared_ptr<Device> device, VkFormat format, const VkExtent3D& extent, uint32_t mipLevels,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, extent,
        mipLevels,
        1, // arrayLayers
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

Image2D::Image2D(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format,
    std::shared_ptr<const SrcTransferBuffer> srcBuffer, const std::vector<Mip>& mipMaps,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(cmdBuffer->getDevice(), format, mipMaps.front().extent, MAGMA_COUNT(mipMaps),
        std::move(allocator), optional, sharing)
{
    copyMipMaps(std::move(cmdBuffer), std::move(srcBuffer), mipMaps, bufferLayout);
}

Image2D::Image2D(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const std::vector<MipData>& mipMaps,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image2D(cmdBuffer->getDevice(), format, mipMaps.front().extent, MAGMA_COUNT(mipMaps),
        allocator, optional, sharing)
{
    // Setup memory layout of mip maps in the buffer
    std::vector<Mip> mipChain;
    const VkDeviceSize bufferSize = setupMipMaps(mipChain, mipMaps);
    // Allocate temporary transfer buffer
    std::shared_ptr<SrcTransferBuffer> srcBuffer = std::make_shared<SrcTransferBuffer>(device,
        bufferSize, nullptr, std::move(allocator), Buffer::Descriptor(), sharing);
    util::mapScoped<uint8_t>(srcBuffer,
        [&](uint8_t *buffer)
        {
            if (!copyFn)
                copyFn = core::copyMemory;
            core::forConstEach(mipChain, mipMaps,
                [buffer, copyFn](auto dstMip, auto srcMip)
                {   // Copy mip texels to buffer
                    copyFn(buffer + dstMip->bufferOffset, srcMip->texels, (std::size_t)srcMip->size);
                });
        });
    // Copy buffer to image
    cmdBuffer->begin();
    copyMipMaps(cmdBuffer, srcBuffer, mipChain, CopyLayout{0, 0, 0});
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
        optional, sharing, std::move(allocator))
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
        optional, sharing, std::move(allocator))
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
        optional, sharing, std::move(allocator))
{}
} // namespace magma
