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
#pragma once
#include "imageView.h"

namespace magma
{
    class MutableImage;

    /* Mutable image view allows to create a view with a different format from the image. */

    class MutableImageView : public ImageView
    {
    public:
        explicit MutableImageView(std::shared_ptr<MutableImage> image,
            VkFormat mutableFormat,
            const StructureChain& extendedInfo = StructureChain());
        explicit MutableImageView(std::shared_ptr<MutableImage> image,
            VkFormat mutableFormat,
            uint32_t baseMipLevel,
            uint32_t levelCount = VK_REMAINING_MIP_LEVELS,
            uint32_t baseArrayLayer = 0,
            uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS,
            const StructureChain& extendedInfo = StructureChain());
        VkFormat getMutableFormat() noexcept { return mutableFormat; }

    private:
        const VkFormat mutableFormat;
    };
} // namespace magma
