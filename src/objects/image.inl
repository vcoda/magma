namespace magma
{
inline Image::Mip::Mip() noexcept:
    extent{0, 0, 0},
    bufferOffset(0ull),
    texels(nullptr)
{}

inline Image::Mip::Mip(const VkExtent3D& extent, VkDeviceSize bufferOffset) noexcept:
    extent(extent),
    bufferOffset(bufferOffset),
    texels(nullptr)
{}

inline Image::Mip::Mip(const VkExtent3D& extent, VkDeviceSize size, const void *texels) noexcept:
    extent(extent),
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
} // namespace magma
