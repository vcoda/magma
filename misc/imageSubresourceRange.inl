namespace magma
{
inline ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image> image,
    uint32_t baseMipLevel /* 0 */,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    this->baseArrayLayer = 0;
    this->layerCount = VK_REMAINING_ARRAY_LAYERS;
}

inline ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image1DArray> image,
    uint32_t baseMipLevel /* 0 */,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    this->baseArrayLayer = baseArrayLayer;
    this->layerCount = layerCount;
}

inline ImageSubresourceRange::ImageSubresourceRange(std::shared_ptr<const Image2DArray> image,
    uint32_t baseMipLevel /* 0 */,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */) noexcept:
    ImageSubresourceRange(image->getFormat())
{
    this->baseMipLevel = baseMipLevel;
    this->levelCount = levelCount;
    this->baseArrayLayer = baseArrayLayer;
    this->layerCount = layerCount;
}

inline ImageSubresourceRange::ImageSubresourceRange(VkFormat format) noexcept
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
