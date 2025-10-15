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
    /* An array of two-dimensional images. */

    class Image2DArray : public Image
    {
    public:
        explicit Image2DArray(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit Image2DArray(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            uint32_t arrayLayers,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const Mipmap& mipMap,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit Image2DArray(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            uint32_t arrayLayers,
            const Mipmap& mipMap,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
    };
} // namespace magma
