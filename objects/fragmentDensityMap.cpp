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
#include "fragmentDensityMap.h"
#include "device.h"
#include "physicalDevice.h"
#include "srcTransferBuffer.h"
#include "commandBuffer.h"

namespace magma
{
#ifdef VK_EXT_fragment_density_map
FragmentDensityMap::FragmentDensityMap(std::shared_ptr<CommandBuffer> cmdBuffer,
    VkFormat format, const VkExtent2D& extent, uint32_t arrayLayers,
    VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFunction copyFn /* nullptr */):
    Image2D(cmdBuffer->getDevice(),
        checkFormatFeature(cmdBuffer->getDevice(), format),
        extent,
        1, // mipLevels
        arrayLayers,
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{
    MAGMA_ASSERT(data);
    auto srcBuffer = std::make_shared<SrcTransferBuffer>(device, size, data, std::move(allocator),
        Buffer::Descriptor(), Sharing(), std::move(copyFn));
    cmdBuffer->begin();
    {
        for (uint32_t arrayLayer = 0; arrayLayer < arrayLayers; ++arrayLayer)
        {
            constexpr CopyLayout bufferLayout = {0, 0, 0};
            constexpr VkOffset3D imageOffset = {0, 0, 0};
            copyMipLevel(cmdBuffer, 0, arrayLayer, srcBuffer, bufferLayout, imageOffset,
                VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT,
                VK_PIPELINE_STAGE_TRANSFER_BIT);
        }
    }
    cmdBuffer->end();
    commitAndWait(std::move(cmdBuffer));
}

FragmentDensityMap::FragmentDensityMap(std::shared_ptr<CommandBuffer> cmdBuffer,
    VkFormat format, const VkExtent2D& extent, uint32_t arrayLayers,
    std::shared_ptr<const SrcTransferBuffer> srcBuffer,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Descriptor& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(cmdBuffer->getDevice(),
        checkFormatFeature(cmdBuffer->getDevice(), format),
        extent,
        1, // mipLevels
        arrayLayers,
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{
    for (uint32_t arrayLayer = 0; arrayLayer < arrayLayers; ++arrayLayer)
    {
        constexpr VkOffset3D imageOffset = {0, 0, 0};
        copyMipLevel(cmdBuffer, 0, arrayLayer, srcBuffer, bufferLayout, imageOffset,
            VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT,
            VK_PIPELINE_STAGE_TRANSFER_BIT);
    }
}

VkFormat FragmentDensityMap::checkFormatFeature(std::shared_ptr<Device> device, VkFormat format)
{
    std::shared_ptr<PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    const VkFormatProperties properties = physicalDevice->getFormatProperties(format);
    bool fragmentShadingRateAttachment = (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT);
    if (!fragmentShadingRateAttachment)
        MAGMA_THROW("inappropriate format of fragment density map");
    return format;
}
#endif // VK_EXT_fragment_density_map
} // namespace magma
