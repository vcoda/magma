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

MAGMA_INLINE ImageSubresourceRange::ImageSubresourceRange(VkFormat format) noexcept
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
