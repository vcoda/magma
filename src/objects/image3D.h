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
#include "image.h"

namespace magma
{
    /* Three-dimensional image. */

    class Image3D : public Image
    {
    public:
        explicit Image3D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit Image3D(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const std::vector<Mip>& mipMaps,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit Image3D(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const std::vector<MipData>& mipMaps,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMemFn = nullptr);

    protected:
        Image3D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent3D& extent,
            VkImageCreateFlags flags,
            VkImageUsageFlags usage,
            VkImageTiling tiling,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);
    };
} // namespace magma
