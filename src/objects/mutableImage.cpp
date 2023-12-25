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
#include "mutableImage.h"
#include "image.h"
#include "../misc/format.h"

namespace magma
{
MutableImage::MutableImage(std::shared_ptr<Device> device, VkImageType imageType, VkFormat format,
    const VkExtent3D& extent, uint32_t mipLevels, uint32_t arrayLayers, uint32_t samples,
    VkImageCreateFlags flags, const Descriptor& optional, const Sharing& sharing,
    std::shared_ptr<Allocator> allocator):
    Image(std::move(device), imageType, format, extent, mipLevels, arrayLayers, samples,
        VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT | flags |
            // Image with compressed format can be used to create an image view
            // with an uncompressed format where each texel in the image view
            // corresponds to a compressed texel block of the image.
            (Format(format).blockCompressed() ? VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT : 0),
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        optional, sharing, std::move(allocator))
{}

bool MutableImage::blockTexelViewCompatible() const noexcept
{
    return Format(format).blockCompressed();
}
} // namespace magma
