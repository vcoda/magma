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
#include "image.h"

namespace magma
{
    /* Array of one-dimensional images. */

    class Image1DArray : public Image
    {
    public:
        explicit Image1DArray(std::shared_ptr<Device> device,
            VkFormat format,
            uint32_t width,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        explicit Image1DArray(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            uint32_t width,
            uint32_t arrayLayers,
            std::shared_ptr<const SrcTransferBuffer> srcBuffer,
            const MipmapLayout& mipOffsets,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing());
        explicit Image1DArray(std::shared_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            uint32_t width,
            const ArrayMipmapData& mipData,
            const MipmapLayout& mipSizes,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Sharing& sharing = Sharing(),
            CopyMemoryFunction copyFn = nullptr);
    };
} // namespace magma
