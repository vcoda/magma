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
namespace magma
{
MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image> image) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    baseMipLevel = 0;
    levelCount = VK_REMAINING_MIP_LEVELS;
    baseArrayLayer = 0;
    layerCount = VK_REMAINING_ARRAY_LAYERS;
}

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image1D> image,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    baseArrayLayer = 0;
    layerCount = VK_REMAINING_ARRAY_LAYERS;
}

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image1DArray> image,
    uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    this->baseArrayLayer = baseArrayLayer;
    this->layerCount = layerCount;
}

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image2D> image,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    baseArrayLayer = 0;
    layerCount = VK_REMAINING_ARRAY_LAYERS;
}

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image2DArray> image,
    uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    this->baseArrayLayer = baseArrayLayer;
    this->layerCount = layerCount;
}

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image3D> image,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    baseArrayLayer = 0;
    layerCount = VK_REMAINING_ARRAY_LAYERS;
}

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const ImageCube> image,
    uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    baseArrayLayer = 0;
    layerCount = VK_REMAINING_ARRAY_LAYERS;
}

ImageSubresourceRange::ImageSubresourceRange(VkFormat format) noexcept
{
    const Format imageFormat(format);
    if (imageFormat.depthStencil())
        aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else if (imageFormat.depth())
        aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else
        aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
}
} // namespace magma
