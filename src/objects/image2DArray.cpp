/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "commandBuffer.h"

namespace magma
{
Image2DArray::Image2DArray(std::shared_ptr<Device> device, VkFormat format, const VkExtent3D& extent, uint32_t mipLevels, uint32_t arrayLayers,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image(std::move(device), VK_IMAGE_TYPE_2D, format, extent,
        mipLevels,
        arrayLayers,
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional,
        sharing,
        std::move(allocator))
{}

Image2DArray::Image2DArray(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, uint32_t arrayLayers,
    std::shared_ptr<const SrcTransferBuffer> srcBuffer, const std::vector<Mip>& mipMaps,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image2DArray(cmdBuffer->getDevice(), format, mipMaps.front().extent,
        MAGMA_COUNT(mipMaps) / arrayLayers, arrayLayers,
        std::move(allocator), optional, sharing)
{
    MAGMA_ASSERT(MAGMA_COUNT(mipMaps) % arrayLayers == 0);
    VkPipelineStageFlags dstStageMask = getSuitableDstStageMask(cmdBuffer);
    copyMipmap(std::move(cmdBuffer), std::move(srcBuffer), mipMaps, bufferLayout,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, dstStageMask);
}

Image2DArray::Image2DArray(std::shared_ptr<CommandBuffer> cmdBuffer, VkFormat format, uint32_t arrayLayers,
    const std::vector<MipData>& mipMaps,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image2DArray(cmdBuffer->getDevice(), format, mipMaps.front().extent,
        MAGMA_COUNT(mipMaps) / arrayLayers, arrayLayers,
        allocator, optional, sharing)
{
    MAGMA_ASSERT(MAGMA_COUNT(mipMaps) % arrayLayers == 0);
    VkPipelineStageFlags dstStageMask = getSuitableDstStageMask(cmdBuffer);
    copyMipmapStaged(std::move(cmdBuffer), mipMaps, std::move(allocator), std::move(copyFn),
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, dstStageMask);
}
} // namespace magma
