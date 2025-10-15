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
    /* Cube map image is an image where each mipmap level consists
       of six 2D images which must be square. The 6 images represent
       the faces of a cube. The texture coordinate used to access
       a cube map is a 3D direction vector. */

    class ImageCube : public Image
    {
    public:
        explicit ImageCube(std::shared_ptr<Device> device,
            VkFormat format,
            uint32_t dimension,
            uint32_t mipLevels,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit ImageCube(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const Mipmap& mipMap,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit ImageCube(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const Mipmap& mipMap,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
    };
} // namespace magma
