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
#pragma once
#include "format.h"
#include "../objects/image.h"
#include "../objects/image1D.h"
#include "../objects/image1DArray.h"
#include "../objects/image2D.h"
#include "../objects/image2DArray.h"
#include "../objects/image3D.h"
#include "../objects/imageCube.h"
#include "../shared.h"

namespace magma
{
    /* Describes the image subresource range within image that is affected by image barrier. */

    struct ImageSubresourceRange : VkImageSubresourceRange
    {
        ImageSubresourceRange(std::shared_ptr<const Image> image) noexcept;
        ImageSubresourceRange(std::shared_ptr<const Image1D> image,
            uint32_t baseMipLevel,
            uint32_t levelCount = VK_REMAINING_MIP_LEVELS) noexcept;
        ImageSubresourceRange(std::shared_ptr<const Image1DArray> image,
            uint32_t baseMipLevel,
            uint32_t levelCount,
            uint32_t baseArrayLayer,
            uint32_t layerCount) noexcept;
        ImageSubresourceRange(std::shared_ptr<const Image2D> image,
            uint32_t baseMipLevel,
            uint32_t levelCount = VK_REMAINING_MIP_LEVELS) noexcept;
        ImageSubresourceRange(std::shared_ptr<const Image2DArray> image,
            uint32_t baseMipLevel,
            uint32_t levelCount,
            uint32_t baseArrayLayer,
            uint32_t layerCount) noexcept;
        ImageSubresourceRange(std::shared_ptr<const Image3D> image,
            uint32_t baseMipLevel,
            uint32_t levelCount = VK_REMAINING_MIP_LEVELS) noexcept;
        ImageSubresourceRange(std::shared_ptr<const ImageCube> image,
            uint32_t baseMipLevel,
            uint32_t levelCount = VK_REMAINING_MIP_LEVELS) noexcept;

    private:
        ImageSubresourceRange(VkFormat format) noexcept;
    };
} // namespace magma

#include "imageSubresourceRange.inl"
