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
#include "imageAttachment.h"

namespace magma
{
ColorAttachment::ColorAttachment(std::shared_ptr<Device> device,
    VkFormat colorFormat, const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples,
    bool sampled,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool explicitResolve /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(std::move(device), colorFormat, extent, mipLevels,
        1, // arrayLayers,
        samples,
        0, // flags
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0) |
            (explicitResolve ? (samples > 1 ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_TRANSFER_DST_BIT) : 0),
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

DepthStencilAttachment::DepthStencilAttachment(std::shared_ptr<Device> device,
    VkFormat depthStencilFormat, const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples,
    bool sampled,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    bool explicitResolve /* false */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(std::move(device), depthStencilFormat, extent, mipLevels,
        1, // arrayLayers,
        samples,
        0, // flags
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0) |
            (explicitResolve ? (samples > 1 ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_TRANSFER_DST_BIT) : 0),
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

#ifdef VK_EXT_attachment_feedback_loop_layout
FeedbackColorAttachment::FeedbackColorAttachment(std::shared_ptr<Device> device,
    VkFormat colorFormat, const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(std::move(device), colorFormat, extent, mipLevels,
        1, // arrayLayers,
        samples,
        0, // flags
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

FeedbackDepthStencilAttachment::FeedbackDepthStencilAttachment(std::shared_ptr<Device> device,
    VkFormat depthStencilFormat, const VkExtent2D& extent, uint32_t mipLevels, uint32_t samples,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image2D(std::move(device), depthStencilFormat, extent, mipLevels,
        1, // arrayLayers,
        samples,
        0, // flags
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}
#endif // VK_EXT_attachment_feedback_loop_layout

#ifdef VK_KHR_maintenance1
VolumeAttachment::VolumeAttachment(std::shared_ptr<Device> device,
    VkFormat format, const VkExtent3D& extent, bool sampled,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Initializer& optional /* default */,
    const Sharing& sharing /* default */):
    Image3D(std::move(device), format, extent,
        VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0),
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}
#endif // VK_KHR_maintenance1
} // namespace magma
