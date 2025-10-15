/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "commandBuffer.h"

namespace magma
{
ImageCube::ImageCube(std::shared_ptr<Device> device, VkFormat format, uint32_t dimension, uint32_t mipLevels,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, VkExtent3D{dimension, dimension, 1}, mipLevels,
        6, // arrayLayers
        1, // samples
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

ImageCube::ImageCube(lent_ptr<CommandBuffer> cmdBuffer, VkFormat format,
    lent_ptr<const SrcTransferBuffer> srcBuffer, const std::vector<Mip>& mipMap,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    ImageCube(cmdBuffer->getDevice(), format, mipMap.front().extent.width, core::countof(mipMap) / 6,
        std::move(allocator), optional, sharing)
{
    MAGMA_ASSERT(mipMap.size() % 6 == 0);
    MAGMA_ASSERT(mipMap.front().extent.width == mipMap.front().extent.height);
    VkPipelineStageFlags dstStageMask = getSuitableDstStageMask(cmdBuffer->getQueueFamilyIndex());
    copyMipmap(std::move(cmdBuffer), std::move(srcBuffer), mipMap, bufferLayout,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, dstStageMask);
}

ImageCube::ImageCube(lent_ptr<CommandBuffer> cmdBuffer, VkFormat format, const std::vector<Mip>& mipMap,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMem /* nullptr */):
    ImageCube(cmdBuffer->getDevice(), format, mipMap.front().extent.width, core::countof(mipMap) / 6,
        allocator, optional, sharing)
{
    MAGMA_ASSERT(mipMap.size() % 6 == 0);
    MAGMA_ASSERT(mipMap.front().extent.width == mipMap.front().extent.height);
    VkPipelineStageFlags dstStageMask = getSuitableDstStageMask(cmdBuffer->getQueueFamilyIndex());
    copyMipmap(std::move(cmdBuffer), mipMap, std::move(allocator), std::move(copyMem),
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, dstStageMask);
}
} // namespace magma
