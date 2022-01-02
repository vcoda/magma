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

namespace magma
{
    class Image;

    /* Image memory barriers only apply to memory accesses involving a specific image subresource range.
       That is, a memory dependency formed from an image memory barrier is scoped to access
       via the specified image subresource range. Image memory barriers can also be used to define
       image layout transitions or a queue family ownership transfer for the specified image subresource range. */

    class ImageMemoryBarrier : public VkImageMemoryBarrier
    {
    public:
        ImageMemoryBarrier(std::shared_ptr<Image> image,
            VkImageLayout newLayout);
        ImageMemoryBarrier(std::shared_ptr<Image> image,
            VkImageLayout newLayout,
            VkImageSubresourceRange subresourceRange);

    private:
        std::shared_ptr<Image> resource; // Store resource as its layout may be changed
        friend class CommandBuffer;
    };
} // namespace magma
