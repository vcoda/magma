/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "frameGrabber.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/commandBuffer.h"
#include "../objects/queue.h"
#include "../objects/fence.h"
#include "../objects/image2DAttachment.h"
#include "../barriers/imageMemoryBarrier.h"
#include "../helpers/mapScoped.h"

namespace magma
{
namespace aux
{
FrameGrabber::FrameGrabber(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    device(std::move(device)),
    queue(this->device->getQueue(VK_QUEUE_GRAPHICS_BIT, 0)),
    allocator(std::move(allocator))
{}

void FrameGrabber::captureFrame(std::shared_ptr<SwapchainColorAttachment2D> srcImage, std::shared_ptr<CommandBuffer> cmdBuffer)
{   // Allocate linear tiled image to copy pixels to
    dstImage = std::make_shared<LinearTiledImage2D>(device, VK_FORMAT_R8G8B8A8_UNORM,
        srcImage->getExtent(), Resource::Sharing(), allocator);
    const bool canBlitTiled = checkBlitSupport(srcImage->getFormat(), dstImage->getFormat());
    cmdBuffer->begin();
    {
        const VkImageLayout oldLayout = srcImage->getLayout();
        // Transition of destination image to transfer dest optimal layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL));
        // Transition of swapchain image to transfer source optimal layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL));
        if (canBlitTiled)
        {   // Use image blitting, format swizzle will be performed automatically
            VkOffset3D blitSize;
            blitSize.x = static_cast<int32_t>(srcImage->getExtent().width);
            blitSize.y = static_cast<int32_t>(srcImage->getExtent().height);
            blitSize.z = 1;
            VkImageBlit blitRegion;
            blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blitRegion.srcSubresource.mipLevel = 0;
            blitRegion.srcSubresource.baseArrayLayer = 0;
            blitRegion.srcSubresource.layerCount = 1;
            blitRegion.srcOffsets[0] = VkOffset3D{0, 0, 0};
            blitRegion.srcOffsets[1] = blitSize;
            blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blitRegion.dstSubresource.mipLevel = 0;
            blitRegion.dstSubresource.baseArrayLayer = 0;
            blitRegion.dstSubresource.layerCount = 1;
            blitRegion.dstOffsets[0] = VkOffset3D{0, 0, 0};
            blitRegion.dstOffsets[1] = blitSize;
            cmdBuffer->blitImage(srcImage, dstImage, blitRegion, VK_FILTER_NEAREST);
        }
        else
        {   // Use image copy, format swizzle will be performed later manually
            VkImageCopy copyRegion;
            copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.srcSubresource.mipLevel = 0;
            copyRegion.srcSubresource.baseArrayLayer = 0;
            copyRegion.srcSubresource.layerCount = 1;
            copyRegion.srcOffset = VkOffset3D{0, 0, 0};
            copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.dstSubresource.mipLevel = 0;
            copyRegion.dstSubresource.baseArrayLayer = 0;
            copyRegion.dstSubresource.layerCount = 1;
            copyRegion.dstOffset = VkOffset3D{0, 0, 0};
            copyRegion.extent = dstImage->getMipExtent(0);
            cmdBuffer->copyImage(srcImage, dstImage, copyRegion);
        }
        // Transition of destination image to general layout, which is the required layout for mapping
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(dstImage, VK_IMAGE_LAYOUT_GENERAL));
        // Transition of swapchain image back to old layout
        cmdBuffer->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            ImageMemoryBarrier(srcImage, oldLayout));
    }
    cmdBuffer->end();
    // Flush command buffer
    std::shared_ptr<Fence> fence = cmdBuffer->getFence();
    queue->submit(std::move(cmdBuffer), 0, nullptr, nullptr, fence);
    fence->wait();
    // Do we need to handle swizzling?
    swizzleBgra = false;
    if (!canBlitTiled)
    {
        const VkFormat format = srcImage->getFormat();
        if ((VK_FORMAT_B8G8R8A8_UNORM == format) || (VK_FORMAT_B8G8R8A8_SRGB == format))
            swizzleBgra = true;
    }
}

void FrameGrabber::readPixels(std::function<void(uint32_t col, uint32_t row, uint32_t rgba)> forEachPixel) const
{   // Get image row pitch
    const VkSubresourceLayout subresourceLayout = dstImage->getSubresourceLayout(0);
    const VkDeviceSize rowPitch = subresourceLayout.rowPitch;
    const VkExtent3D extent = dstImage->getMipExtent(0);
    helpers::mapScoped<uint8_t>(dstImage, subresourceLayout.offset,
        [this, extent, rowPitch, forEachPixel](const uint8_t *data)
        {
            for (uint32_t y = 0; y < extent.height; ++y)
            {
                const uint32_t *row = (const uint32_t *)data;
                for (uint32_t x = 0; x < extent.width; ++x)
                {
                    uint32_t abgr;
                    if (!swizzleBgra)
                        abgr = row[x];
                    else
                    {   // Swizzle BGRA to RGBA
                        const uint32_t argb = row[x];
                        const uint32_t r = (argb >> 16) & 0xFF;
                        const uint32_t g = (argb >> 8) & 0xFF;
                        const uint32_t b = argb & 0xFF;
                        abgr = (argb & 0xFF000000) | (b << 16) | (g << 8) | r;
                    }
                    forEachPixel(x, y, abgr);
                }
                data += rowPitch;
            }
        }
    );
}

void FrameGrabber::readPixels(std::function<void(uint32_t row, const std::vector<uint32_t>& rowPixels)> forEachRow) const
{   // Get image row pitch
    const VkSubresourceLayout subresourceLayout = dstImage->getSubresourceLayout(0);
    const VkDeviceSize rowPitch = subresourceLayout.rowPitch;
    const VkExtent3D extent = dstImage->getMipExtent(0);
    helpers::mapScoped<uint8_t>(dstImage, subresourceLayout.offset,
        [this, extent, rowPitch, forEachRow](const uint8_t *data)
        {
            std::vector<uint32_t> rowPixels(extent.width);
            for (uint32_t y = 0; y < extent.height; ++y)
            {
                const uint32_t *row = (const uint32_t *)data;
                if (!swizzleBgra)
                    memcpy(rowPixels.data(), row, sizeof(uint32_t) * extent.width);
                else for (uint32_t x = 0; x < extent.width; ++x)
                {   // Swizzle BGRA to RGBA
                    const uint32_t argb = row[x];
                    const uint32_t r = (argb >> 16) & 0xFF;
                    const uint32_t g = (argb >> 8) & 0xFF;
                    const uint32_t b = argb & 0xFF;
                    rowPixels[x] = (argb & 0xFF000000) | (b << 16) | (g << 8) | r;
                }
                forEachRow(y, rowPixels);
                data += rowPitch;
            }
        }
    );
}

VkExtent2D FrameGrabber::getImageExtent() const
{
    if (dstImage)
    {
        const VkExtent3D extent = dstImage->getMipExtent(0);
        return VkExtent2D{extent.width, extent.height};
    }
    return VkExtent2D{0, 0};
}

bool FrameGrabber::checkBlitSupport(VkFormat srcFormat, VkFormat dstFormat) const noexcept
{
    std::shared_ptr<PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    const VkFormatProperties srcProperties = physicalDevice->getFormatProperties(srcFormat);
    const VkFormatProperties dstProperties = physicalDevice->getFormatProperties(dstFormat);
    const bool optimalSrcBlit = (srcProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT);
    const bool linearDstBlit = (dstProperties.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
    return optimalSrcBlit && linearDstBlit;
}
} // namespace aux
} // namespace magma
