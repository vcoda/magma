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
#include "imageCube.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"
#include "../helpers/mapScoped.h"
#include "../core/copyMemory.h"
#include "../core/foreach.h"

namespace magma
{
ImageCube::ImageCube(std::shared_ptr<Device> device, VkFormat format, uint32_t dimension, uint32_t mipLevels,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{dimension, dimension, 1}, mipLevels,
        6, // arrayLayers
        1, // samples
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

ImageCube::ImageCube(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format,
    std::shared_ptr<const SrcTransferBuffer> srcBuffer, const std::vector<Mip>& mipMaps,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    ImageCube(cmdBuffer->getDevice(), format, mipMaps.front().extent.width, MAGMA_COUNT(mipMaps) / 6,
        std::move(allocator), optional, sharing)
{
    MAGMA_ASSERT(mipMaps.size() % 6 == 0);
    MAGMA_ASSERT(mipMaps.front().extent.width == mipMaps.front().extent.height);
    copyMipMaps(std::move(cmdBuffer), std::move(srcBuffer), mipMaps, bufferLayout);
}

ImageCube::ImageCube(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, const std::vector<MipData>& mipMaps,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    ImageCube(cmdBuffer->getDevice(), format, mipMaps.front().extent.width, MAGMA_COUNT(mipMaps) / 6,
        allocator, optional, sharing)
{
    MAGMA_ASSERT(mipMaps.size() % 6 == 0);
    MAGMA_ASSERT(mipMaps.front().extent.width == mipMaps.front().extent.height);
    // Setup memory layout of mip maps in the buffer
    std::vector<Mip> mipChain;
    const VkDeviceSize bufferSize = setupMipMaps(mipChain, mipMaps);
    std::shared_ptr<SrcTransferBuffer> srcBuffer = std::make_shared<SrcTransferBuffer>(device, bufferSize, nullptr,
        std::move(allocator), Buffer::Descriptor(), sharing);
    helpers::mapScoped<uint8_t>(srcBuffer,
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
} // namespace magma
