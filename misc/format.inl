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

constexpr bool Format::astc() const noexcept
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

constexpr bool Format::pvrtc() const noexcept
{
    switch (format)
    {
#ifdef VK_IMG_format_pvrtc
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return true;
#endif // VK_IMG_format_pvrtc
    default:
        return false;
    }
}

constexpr bool Format::blockCompressed() const noexcept
{
    return bc() || etc2() || eac() || astc() || pvrtc();
}

constexpr bool Format::floatingPoint() const noexcept
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
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
        return true;
    default:
        return false;
    }
}

constexpr bool Format::sRGB() const noexcept
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

constexpr bool Format::yCbCr() const noexcept
{
    switch (format)
    {
#ifdef VK_KHR_sampler_ycbcr_conversion
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
#endif // VK_KHR_sampler_ycbcr_conversion
#ifdef VK_EXT_ycbcr_2plane_444_formats
    case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT:
        return true;
#endif // VK_EXT_ycbcr_2plane_444_formats
    default:
        return false;
    }
}

inline std::size_t Format::size() const noexcept
{
    if (bc())
        return blockCompressedSize();
    if (pvrtc())
    {   // Specifies a four-component, PVRTC compressed format where each
        // 64-bit compressed texel block encodes an 8(4)x4 rectangle.
        return 64/8;
    }
    if (ycbcr())
        return videoCompressedSize();
    return nonCompressedSize();
}
} // namespace magma
