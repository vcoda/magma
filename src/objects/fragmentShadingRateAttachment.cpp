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
#include "fragmentShadingRateAttachment.h"
#include "device.h"
#include "commandBuffer.h"
#include "srcTransferBuffer.h"
#include "../misc/finish.h"

namespace magma
{
#ifdef VK_KHR_fragment_shading_rate
FragmentShadingRateAttachment::FragmentShadingRateAttachment(lent_ptr<CommandBuffer> cmdBuffer,
    VkFormat format, const VkExtent2D& extent, uint32_t arrayLayers,
    VkDeviceSize size, const void *data,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */,
    CopyMemoryFn copyMem /* nullptr */):
    Image2D(cmdBuffer->getDevice(),
        checkFormatFeature(cmdBuffer->getDevice(), format, VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR),
        extent,
        1, // mipLevels
        arrayLayers,
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, allocator)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    MAGMA_ASSERT(data);
    if (cmdBuffer->reset())
    {
        if (cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT))
        {
            auto srcBuffer = std::make_unique<SrcTransferBuffer>(device, size, data, std::move(allocator),
                Buffer::Initializer(), Sharing(), std::move(copyMem));
            for (uint32_t arrayLayer = 0; arrayLayer < arrayLayers; ++arrayLayer)
            {
                constexpr CopyLayout bufferLayout = {0, 0, 0};
                constexpr VkOffset3D imageOffset = {0, 0, 0};
                copyMipWithTransition(cmdBuffer.get(), 0, arrayLayer, srcBuffer.get(), bufferLayout, imageOffset,
                    VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR,
                    VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR);
            }
            cmdBuffer->end();
            // Block until execution is complete
            finish(std::move(cmdBuffer));
        }
    }
}

FragmentShadingRateAttachment::FragmentShadingRateAttachment(lent_ptr<CommandBuffer> cmdBuffer,
    VkFormat format, const VkExtent2D& extent, uint32_t arrayLayers,
    lent_ptr<const SrcTransferBuffer> srcBuffer,
    const CopyLayout& bufferLayout /* {offset = 0, rowLength = 0, imageHeight = 0} */,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(cmdBuffer->getDevice(),
        checkFormatFeature(cmdBuffer->getDevice(), format, VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR),
        extent,
        1, // mipLevels
        arrayLayers,
        1, // samples
        0, // flags
        VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{
    for (uint32_t arrayLayer = 0; arrayLayer < arrayLayers; ++arrayLayer)
    {
        constexpr VkOffset3D imageOffset = {0, 0, 0};
        copyMipWithTransition(cmdBuffer.get(), 0, arrayLayer, srcBuffer.get(), bufferLayout, imageOffset,
            VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR,
            VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR);
    }
}
#endif // VK_KHR_fragment_shading_rate
} // namespace magma
