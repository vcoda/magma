namespace magma
{
constexpr Format::Format() noexcept:
    format(VK_FORMAT_UNDEFINED)
{}

constexpr Format::Format(const VkFormat format) noexcept:
    format(format)
{}

constexpr Format::operator VkFormat() const noexcept
{
    return format;
}

constexpr bool Format::valid() const noexcept
{
    return format != VK_FORMAT_UNDEFINED;
}

constexpr bool Format::unorm() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R4G4_UNORM_PACK8:
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8B8_UNORM:
    case VK_FORMAT_B8G8R8_UNORM:
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_B8G8R8A8_UNORM:
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R16G16B16_UNORM:
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
    case VK_FORMAT_BC2_UNORM_BLOCK:
    case VK_FORMAT_BC3_UNORM_BLOCK:
    case VK_FORMAT_BC4_UNORM_BLOCK:
    case VK_FORMAT_BC5_UNORM_BLOCK:
    case VK_FORMAT_BC7_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
#ifdef VK_IMG_format_pvrtc
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
#endif // VK_IMG_format_pvrtc
#ifdef VK_KHR_sampler_ycbcr_conversion
    case VK_FORMAT_G8B8G8R8_422_UNORM_KHR:
    case VK_FORMAT_B8G8R8G8_422_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR:
    case VK_FORMAT_R10X6_UNORM_PACK16_KHR:
    case VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR:
    case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR:
    case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_R12X4_UNORM_PACK16_KHR:
    case VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR:
    case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR:
    case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G16B16G16R16_422_UNORM_KHR:
    case VK_FORMAT_B16G16R16G16_422_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR:
#endif // VK_KHR_sampler_ycbcr_conversion
#ifdef VK_EXT_ycbcr_2plane_444_formats
    case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT:
#endif // VK_EXT_ycbcr_2plane_444_formats
#ifdef VK_EXT_4444_formats
    case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT:
    case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT:
#endif // VK_EXT_4444_formats
        return true;
    default:
        return false;
    }
}

constexpr bool Format::snorm() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8B8_SNORM:
    case VK_FORMAT_B8G8R8_SNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_B8G8R8A8_SNORM:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16G16_SNORM:
    case VK_FORMAT_R16G16B16_SNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_BC4_SNORM_BLOCK:
    case VK_FORMAT_BC5_SNORM_BLOCK:
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::uscaled() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_USCALED:
    case VK_FORMAT_R8G8_USCALED:
    case VK_FORMAT_R8G8B8_USCALED:
    case VK_FORMAT_B8G8R8_USCALED:
    case VK_FORMAT_R8G8B8A8_USCALED:
    case VK_FORMAT_B8G8R8A8_USCALED:
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
    case VK_FORMAT_R16_USCALED:
    case VK_FORMAT_R16G16_USCALED:
    case VK_FORMAT_R16G16B16_USCALED:
    case VK_FORMAT_R16G16B16A16_USCALED:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::sscaled() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_SSCALED:
    case VK_FORMAT_R8G8_SSCALED:
    case VK_FORMAT_R8G8B8_SSCALED:
    case VK_FORMAT_B8G8R8_SSCALED:
    case VK_FORMAT_R8G8B8A8_SSCALED:
    case VK_FORMAT_B8G8R8A8_SSCALED:
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
    case VK_FORMAT_R16_SSCALED:
    case VK_FORMAT_R16G16_SSCALED:
    case VK_FORMAT_R16G16B16_SSCALED:
    case VK_FORMAT_R16G16B16A16_SSCALED:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::uint() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_B8G8R8_UINT:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_B8G8R8A8_UINT:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R64_UINT:
    case VK_FORMAT_R64G64_UINT:
    case VK_FORMAT_R64G64B64_UINT:
    case VK_FORMAT_R64G64B64A64_UINT:
    case VK_FORMAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::sint() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_B8G8R8_SINT:
    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_B8G8R8A8_SINT:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R64_SINT:
    case VK_FORMAT_R64G64_SINT:
    case VK_FORMAT_R64G64B64_SINT:
    case VK_FORMAT_R64G64B64A64_SINT:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::ufloat() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::sfloat() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R16_SFLOAT:
    case VK_FORMAT_R16G16_SFLOAT:
    case VK_FORMAT_R16G16B16_SFLOAT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32_SFLOAT:
    case VK_FORMAT_R32G32_SFLOAT:
    case VK_FORMAT_R32G32B32_SFLOAT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
    case VK_FORMAT_R64_SFLOAT:
    case VK_FORMAT_R64G64_SFLOAT:
    case VK_FORMAT_R64G64B64_SFLOAT:
    case VK_FORMAT_R64G64B64A64_SFLOAT:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
#ifdef VK_EXT_texture_compression_astc_hdr
    case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT:
#endif // VK_EXT_texture_compression_astc_hdr
        return true;
    default:
        return false;
    }
}

constexpr bool Format::depth() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::stencil() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::depthStencil() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_D16_UNORM_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::alpha() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_R8G8B8A8_SNORM:
    case VK_FORMAT_R8G8B8A8_USCALED:
    case VK_FORMAT_R8G8B8A8_SSCALED:
    case VK_FORMAT_R8G8B8A8_UINT:
    case VK_FORMAT_R8G8B8A8_SINT:
    case VK_FORMAT_R8G8B8A8_SRGB:
    case VK_FORMAT_B8G8R8A8_UNORM:
    case VK_FORMAT_B8G8R8A8_SNORM:
    case VK_FORMAT_B8G8R8A8_USCALED:
    case VK_FORMAT_B8G8R8A8_SSCALED:
    case VK_FORMAT_B8G8R8A8_UINT:
    case VK_FORMAT_B8G8R8A8_SINT:
    case VK_FORMAT_B8G8R8A8_SRGB:
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_R16G16B16A16_USCALED:
    case VK_FORMAT_R16G16B16A16_SSCALED:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32B32A32_UINT:
    case VK_FORMAT_R32G32B32A32_SINT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
    case VK_FORMAT_R64G64B64A64_UINT:
    case VK_FORMAT_R64G64B64A64_SINT:
    case VK_FORMAT_R64G64B64A64_SFLOAT:
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
#ifdef VK_KHR_sampler_ycbcr_conversion
    case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR:
    case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR:
#endif // VK_KHR_sampler_ycbcr_conversion
#ifdef VK_EXT_4444_formats
    case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT:
    case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT:
#endif // VK_EXT_4444_formats
        return true;
    default:
        return false;
    }
}

constexpr bool Format::srgb() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_SRGB:
    case VK_FORMAT_R8G8_SRGB:
    case VK_FORMAT_R8G8B8_SRGB:
    case VK_FORMAT_B8G8R8_SRGB:
    case VK_FORMAT_R8G8B8A8_SRGB:
    case VK_FORMAT_B8G8R8A8_SRGB:
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
    case VK_FORMAT_BC2_SRGB_BLOCK:
    case VK_FORMAT_BC3_SRGB_BLOCK:
    case VK_FORMAT_BC7_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
#ifdef VK_IMG_format_pvrtc
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
#endif // VK_IMG_format_pvrtc
        return true;
    default:
        return false;
    }
}

constexpr bool Format::bc() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
    case VK_FORMAT_BC2_UNORM_BLOCK:
    case VK_FORMAT_BC2_SRGB_BLOCK:
    case VK_FORMAT_BC3_UNORM_BLOCK:
    case VK_FORMAT_BC3_SRGB_BLOCK:
    case VK_FORMAT_BC4_UNORM_BLOCK:
    case VK_FORMAT_BC4_SNORM_BLOCK:
    case VK_FORMAT_BC5_UNORM_BLOCK:
    case VK_FORMAT_BC5_SNORM_BLOCK:
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
    case VK_FORMAT_BC7_UNORM_BLOCK:
    case VK_FORMAT_BC7_SRGB_BLOCK:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::etc2() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::eac() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::astcLdr() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
        return true;
    default:
        return false;
    };
}

constexpr bool Format::astcHdr() const noexcept
{
#ifdef VK_EXT_texture_compression_astc_hdr
    switch (format)
    {
    case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT:
        return true;
    };
#endif // VK_EXT_texture_compression_astc_hdr
    return false;
}

constexpr bool Format::pvrtc() const noexcept
{
#ifdef VK_IMG_format_pvrtc
    switch (format)
    {
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return true;
    }
#endif // VK_IMG_format_pvrtc
    return false;
}

constexpr bool Format::ycbcr() const noexcept
{
#ifdef VK_KHR_sampler_ycbcr_conversion
    switch (format)
    {
    case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_B16G16R16G16_422_UNORM_KHR:
    case VK_FORMAT_B8G8R8G8_422_UNORM_KHR:
    case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G16B16G16R16_422_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR:
    case VK_FORMAT_G8B8G8R8_422_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR:
    case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR:
    case VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR:
    case VK_FORMAT_R10X6_UNORM_PACK16_KHR:
    case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR:
    case VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR:
    case VK_FORMAT_R12X4_UNORM_PACK16_KHR:
        return true;
    }
#endif // VK_KHR_sampler_ycbcr_conversion
#ifdef VK_EXT_ycbcr_2plane_444_formats
    switch (format)
    {
    case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT:
        return true;
    }
#endif // VK_EXT_ycbcr_2plane_444_formats
    return false;
}

constexpr bool Format::packed() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
    case VK_FORMAT_X8_D24_UNORM_PACK32:
#ifdef VK_KHR_sampler_ycbcr_conversion
    case VK_FORMAT_R10X6_UNORM_PACK16_KHR:
    case VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR:
    case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR:
    case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_R12X4_UNORM_PACK16_KHR:
    case VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR:
    case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR:
    case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:
#endif // VK_KHR_sampler_ycbcr_conversion
#ifdef VK_EXT_ycbcr_2plane_444_formats
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT:
    case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT:
#endif // VK_EXT_ycbcr_2plane_444_formats
        return true;
    default:
        return false;
    }
}

constexpr bool Format::multiPlanar() const noexcept
{
#ifdef VK_KHR_sampler_ycbcr_conversion
    switch (format)
    {
    case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR:
        return true;
    }
#endif // VK_KHR_sampler_ycbcr_conversion
#ifdef VK_EXT_ycbcr_2plane_444_formats
    switch (format)
    {
    case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT:
        return true;
    }
#endif // VK_EXT_ycbcr_2plane_444_formats
    return false;
}

constexpr std::size_t Format::depthStencilSize() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_S8_UINT:
        return sizeof(uint8_t);
    case VK_FORMAT_D16_UNORM:
        return sizeof(uint16_t);
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return sizeof(uint16_t) + sizeof(uint8_t);
    case VK_FORMAT_X8_D24_UNORM_PACK32:
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return sizeof(uint32_t);
    case VK_FORMAT_D32_SFLOAT:
        return sizeof(float);
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        // Specifies a two-component format that has 32 signed
        // float bits in the depth component and 8 unsigned integer
        // bits in the stencil component. There are optionally
        // 24 bits that are unused.
        return sizeof(float) + sizeof(uint32_t);
    default:
        return 0;
    }
}

constexpr std::size_t Format::bcSize() const noexcept
{
    switch (format)
    {
    // https://en.wikipedia.org/wiki/S3_Texture_Compression
    // DXT1c
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
    // DXT1a
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        // Stores 16 input pixels in 64 bits of output,
        // consisting of two 16-bit RGB 5:6:5 color values
        // and a 4x4 two-bit lookup table.
        return 64/8;
    // DXT2, DXT3
    case VK_FORMAT_BC2_UNORM_BLOCK:
    case VK_FORMAT_BC2_SRGB_BLOCK:
        // Stores 16 input pixels in 128 bits of output,
        // consisting of 64 bits of alpha channel data
        // (4 bits for each pixel) followed by 64 bits of color data.
        return 128/8;
    // DXT4, DXT5
    case VK_FORMAT_BC3_UNORM_BLOCK:
    case VK_FORMAT_BC3_SRGB_BLOCK:
        // Stores 16 input pixels in 128 bits of output,
        // consisting of 64 bits of alpha channel data
        // (two 8 bit alpha values and a 4x4 3 bit lookup table)
        // followed by 64 bits of color data.
        return 128/8;
    // https://en.wikipedia.org/wiki/3Dc
    // ATI1
    case VK_FORMAT_BC4_UNORM_BLOCK:
    case VK_FORMAT_BC4_SNORM_BLOCK:
        // Assuming a 4x4 texture using the largest data format possible,
        // this compression technique reduces the memory required from
        // 16 bytes to 8 bytes.
        return 64/8;
    // ATI2
    case VK_FORMAT_BC5_UNORM_BLOCK:
    case VK_FORMAT_BC5_SNORM_BLOCK:
        // Total storage is 128 bits per 4x4 block once both
        // source components are factored in. In an uncompressed
        // scheme with similar 8-bit precision, the source data
        // is 32 8-bit values for the same area, occupying 256 bits.
        // The algorithm therefore produces a 2:1 compression ratio.
        return 128/8;
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc6h-format
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
        // BC6H uses a fixed block size of 16 bytes (128 bits)
        // and a fixed tile size of 4x4 texels.
        return 128/8;
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc7-format
    case VK_FORMAT_BC7_UNORM_BLOCK:
    case VK_FORMAT_BC7_SRGB_BLOCK:
        // BC7 uses a fixed block size of 16 bytes (128 bits)
        // and a fixed tile size of 4x4 texels.
        return 128/8;
    default:
        return 0;
    }
}

constexpr std::size_t Format::etc2Size() const noexcept
{
    switch (format)
    {
    // 43.1. Format Definition
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
        // ETC2 compressed format where each 64-bit compressed
        // texel block encodes a 4x4 rectangle.
        return 64/8;
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        // ETC2 compressed format where each 128-bit compressed
        // texel block encodes a 4x4 rectangle.
        return 128/8;
    default:
        return 0;
    }
}

constexpr std::size_t Format::eacSize() const noexcept
{
    switch (format)
    {
    // 43.1. Format Definition
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
        // ETC2 compressed format where each 64-bit compressed
        // texel block encodes a 4x4 rectangle.
        return 64/8;
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        // ETC2 compressed format where each 128-bit compressed
        // texel block encodes a 4x4 rectangle.
        return 128/8;
    default:
        return 0;
    }
}

constexpr std::size_t Format::astcSize() const noexcept
{
    switch (format)
    {
    // 43.1. Format Definition
    // ASTC compressed format where each 128-bit compressed
    // texel block encodes a NxN rectangle.
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
#ifdef VK_EXT_texture_compression_astc_hdr
    case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT:
    case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT:
#endif // VK_EXT_texture_compression_astc_hdr
        return 128/8;
    default:
        return 0;
    };
}

constexpr std::size_t Format::pvrtcSize() const noexcept
{
#ifdef VK_IMG_format_pvrtc
    switch (format)
    {
    // Specifies a four-component, PVRTC compressed format
    // where each 64-bit compressed texel block encodes an
    // 8(4)x4 rectangle.
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return 64/8;
    }
#endif // VK_IMG_format_pvrtc
    return 0;
}
} // namespace magma
