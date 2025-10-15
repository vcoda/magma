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
#include "mutableImage.h"

namespace magma
{
    /* Two-dimensional image. */

    class Image2D : public Image
    {
    public:
        explicit Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent3D& extent,
            uint32_t mipLevels,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit Image2D(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            const Mipmap& mipMap,
            const CopyLayout& bufferLayout = {0, 0, 0},
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit Image2D(lent_ptr<CommandBuffer> cmdBuffer,
            VkFormat format,
            const Mipmap& mipMap,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
        VkExtent2D getExtent2D() const noexcept { return {extent.width, extent.height}; }

    protected:
        Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageCreateFlags flags,
            VkImageUsageFlags usage,
            VkImageTiling tiling,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);
        Image2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t arrayLayers,
            uint32_t samples,
            VkImageCreateFlags flags,
            VkImageUsageFlags usage,
            VkImageTiling tiling);
    };

    /* Two-dimensional image with linear tiling. */

    class LinearTiledImage2D : public Image2D
    {
    public:
        explicit LinearTiledImage2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };

    /* Supports unfiltered loads, stores, and atomics in a shader.
       Do transition of image layout to VK_IMAGE_LAYOUT_GENERAL using image barrier. */

    class StorageImage2D : public Image2D
    {
    public:
        explicit StorageImage2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            uint32_t samples,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };

    /* Mutable image can be used to create an image view
       with a different format from the image. For multi-planar formats,
       image view can be created of a plane of the image. */

    class MutableImage2D : public MutableImage
    {
    public:
        explicit MutableImage2D(std::shared_ptr<Device> device,
            VkFormat format,
            const VkExtent2D& extent,
            uint32_t mipLevels,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };
} // namespace magma
