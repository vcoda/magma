namespace magma
{
inline Image::Mip::Mip() noexcept:
    extent{0, 0, 0},
    bufferOffset(0ull),
    texels(nullptr)
{}

inline Image::Mip::Mip(uint32_t width, uint32_t height, VkDeviceSize bufferOffset) noexcept:
    extent{width, height, 1},
    bufferOffset(bufferOffset),
    texels(nullptr)
{}

inline Image::Mip::Mip(const VkExtent2D& extent2D, VkDeviceSize bufferOffset) noexcept:
    extent{extent2D.width, extent2D.height, 1},
    bufferOffset(bufferOffset),
    texels(nullptr)
{}

inline Image::Mip::Mip(const VkExtent3D& extent3D, VkDeviceSize bufferOffset) noexcept:
    extent(extent3D),
    bufferOffset(bufferOffset),
    texels(nullptr)
{}

inline Image::Mip::Mip(uint32_t width, uint32_t height, VkDeviceSize size, const void *texels) noexcept:
    extent{width, height, 1},
    size(size),
    texels(texels)
{}

inline Image::Mip::Mip(const VkExtent2D& extent2D, VkDeviceSize size, const void *texels) noexcept:
    extent{extent2D.width, extent2D.height, 1},
    size(size),
    texels(texels)
{}

inline Image::Mip::Mip(const VkExtent3D& extent3D, VkDeviceSize size, const void *texels) noexcept:
    extent(extent3D),
    size(size),
    texels(texels)
{}

inline bool Image::hasUniformLayout() const noexcept
{
    for (uint32_t level = 1; level < mipLevels; ++level)
    {
        if (mipLayouts[level] != mipLayouts[0])
            return false;
    }
    return true;
}

inline uint32_t Image::getTexelCount() const noexcept
{
    uint32_t texelCount = 0;
    for (uint32_t level = 0; level < mipLevels; ++level)
        texelCount += getLevelTexelCount(level);
    return texelCount * arrayLayers;
}
} // namespace magma
