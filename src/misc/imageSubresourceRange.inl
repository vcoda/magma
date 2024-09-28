namespace magma
{
inline ImageSubresourceRange::ImageSubresourceRange(const Image *image,
    uint32_t baseMipLevel /* 0 */,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */) noexcept:
    VkImageSubresourceRange{
        image->getAspectMask(),
        baseMipLevel,
        levelCount,
        0, // baseArrayLayer
        VK_REMAINING_ARRAY_LAYERS // layerCount
    }
{}

inline ImageSubresourceRange::ImageSubresourceRange(const Image1DArray *imageArray,
    uint32_t baseMipLevel /* 0 */,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */) noexcept:
    VkImageSubresourceRange{
        imageArray->getAspectMask(),
        baseMipLevel,
        levelCount,
        baseArrayLayer,
        layerCount
    }
{}

inline ImageSubresourceRange::ImageSubresourceRange(const Image2DArray *imageArray,
    uint32_t baseMipLevel /* 0 */,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */) noexcept:
    VkImageSubresourceRange{
        imageArray->getAspectMask(),
        baseMipLevel,
        levelCount,
        baseArrayLayer,
        layerCount
    }
{}
} // namespace magma
