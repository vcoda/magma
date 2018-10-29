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
#include "handle.h"

namespace magma
{
    class Image;

    /* Image objects are not directly accessed by pipeline shaders
       for reading or writing image data. Instead, image views representing
       contiguous ranges of the image subresources and containing additional
       metadata are used for that purpose. Views must be created on images
       of compatible types, and must represent a valid subset of image subresources. */

    class ImageView : public NonDispatchable<VkImageView>
    {
    public:
        explicit ImageView(std::shared_ptr<const Image> resource,
            uint32_t mipLevelCount = 0,
            // https://github.com/SaschaWillems/Vulkan/issues/160
            const VkComponentMapping& swizzle = {
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY},
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~ImageView();
        std::shared_ptr<const Image> getImage() const noexcept { return image; }

    private:
        std::shared_ptr<const Image> image;
    };
} // namespace magma
