namespace magma
{
inline Image::Mip::Mip() noexcept:
    extent{0, 0, 0},
    bufferOffset(0ull)
{}

inline Image::Mip::Mip(const VkExtent3D& extent, VkDeviceSize bufferOffset) noexcept:
    extent(extent),
    bufferOffset(bufferOffset)
{}

inline Image::MipData::MipData() noexcept:
    extent{0, 0, 0},
    size(0ull),
    texels(nullptr)
{}

inline Image::MipData::MipData(const VkExtent3D& extent, VkDeviceSize size, const void *texels) noexcept:
    extent(extent),
    size(size),
    texels(texels)
{}
} // namespace magma
