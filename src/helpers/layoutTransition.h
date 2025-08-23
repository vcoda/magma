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
#pragma once

namespace magma
{
    class Image;
    class CommandBuffer;
}

namespace magma::helpers
{
    /* Helper function to change layout of the image. As example,
       for VK_DESCRIPTOR_TYPE_STORAGE_IMAGE allowed layout is
       VK_IMAGE_LAYOUT_GENERAL, which isn't set by default. */

    bool layoutTransition(lent_ptr<Image> image,
        VkImageLayout newLayout,
        lent_ptr<CommandBuffer> cmdBuffer,
        VkDependencyFlags dependencyFlags = 0);

    struct LayoutTransition
    {
        VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        uint32_t baseMipLevel = 0;
        uint32_t baseArrayLayer = 0;
    };

    bool batchLayoutTransition(const std::unordered_map<lent_ptr<Image>, LayoutTransition>& imageLayouts,
        lent_ptr<CommandBuffer> cmdBuffer,
        VkDependencyFlags dependencyFlags = 0);
} // namespace magma::helpers
