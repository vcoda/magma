/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "format.h"

namespace magma
{
std::size_t Format::size() const noexcept
{
    std::size_t size = 0;
    if (depth() || stencil() || depthStencil())
        size = depthStencilSize();
    else if (bc() || etc2() || eac() || astcLdr() || astcHdr() || pvrtc())
        size = blockSize();
    else // TODO: Add video format size
        size = texelSize();
    MAGMA_ASSERT(size);
    return size;
}

uint8_t Format::componentCount() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_USCALED:
    case VK_FORMAT_R8_SSCALED:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8_SRGB:
#ifdef VK_KHR_maintenance5
    case VK_FORMAT_A8_UNORM_KHR:
#endif // VK_KHR_maintenance5
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16_USCALED:
    case VK_FORMAT_R16_SSCALED:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_SFLOAT:
    case VK_FORMAT_R32_UINT:
    case VK_FORMAT_R32_SINT:
    case VK_FORMAT_R32_SFLOAT:
    case VK_FORMAT_R64_UINT:
    case VK_FORMAT_R64_SINT:
    case VK_FORMAT_R64_SFLOAT:
        return 1;
    case VK_FORMAT_R4G4_UNORM_PACK8:
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_USCALED:
    case VK_FORMAT_R8G8_SSCALED:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8_SRGB:
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R16G16_SNORM:
    case VK_FORMAT_R16G16_USCALED:
    case VK_FORMAT_R16G16_SSCALED:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16_SFLOAT:
    case VK_FORMAT_R32G32_UINT:
    case VK_FORMAT_R32G32_SINT:
    case VK_FORMAT_R32G32_SFLOAT:
    case VK_FORMAT_R64G64_UINT:
    case VK_FORMAT_R64G64_SINT:
    case VK_FORMAT_R64G64_SFLOAT:
#ifdef VK_NV_optical_flow
    case VK_FORMAT_R16G16_S10_5_NV:
#endif // VK_NV_optical_flow
        return 2;
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
    case VK_FORMAT_R8G8B8_UNORM:
    case VK_FORMAT_R8G8B8_SNORM:
    case VK_FORMAT_R8G8B8_USCALED:
    case VK_FORMAT_R8G8B8_SSCALED:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_R8G8B8_SRGB:
    case VK_FORMAT_B8G8R8_UNORM:
    case VK_FORMAT_B8G8R8_SNORM:
    case VK_FORMAT_B8G8R8_USCALED:
    case VK_FORMAT_B8G8R8_SSCALED:
    case VK_FORMAT_B8G8R8_UINT:
    case VK_FORMAT_B8G8R8_SINT:
    case VK_FORMAT_B8G8R8_SRGB:
    case VK_FORMAT_R16G16B16_UNORM:
    case VK_FORMAT_R16G16B16_SNORM:
    case VK_FORMAT_R16G16B16_USCALED:
    case VK_FORMAT_R16G16B16_SSCALED:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16_SFLOAT:
    case VK_FORMAT_R32G32B32_UINT:
    case VK_FORMAT_R32G32B32_SINT:
    case VK_FORMAT_R32G32B32_SFLOAT:
    case VK_FORMAT_R64G64B64_UINT:
    case VK_FORMAT_R64G64B64_SINT:
    case VK_FORMAT_R64G64B64_SFLOAT:
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
        return 3;
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
#ifdef VK_EXT_4444_formats
    case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT:
    case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT:
#endif // VK_EXT_4444_formats
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
        return 4;
    // TODO: add other relevant formats if necessary
    default:
        MAGMA_ASSERT(!depth() && !stencil() && !depthStencil());
        MAGMA_ASSERT(!blockCompressed());
        return 0;
    }
}

uint8_t Format::planeCount() const noexcept
{
    switch (format)
    {
#ifdef VK_EXT_ycbcr_2plane_444_formats
    case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:
    case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT:
        return 2;
#endif // VK_EXT_ycbcr_2plane_444_formats
#ifdef VK_KHR_sampler_ycbcr_conversion
    case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR:
        return 2;
    case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR:
    case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR:
        return 3;
#endif // VK_KHR_sampler_ycbcr_conversion
    default:
        MAGMA_ASSERT(ycbcr());
        return 0;
    }
}

bool Format::blockCompressed() const noexcept
{
    return bc() || etc2() || eac() || astcLdr() || astcHdr() || pvrtc();
}

std::pair<uint32_t, uint32_t> Format::blockFootprint() const noexcept
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
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        return std::make_pair(4, 4);
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
        return std::make_pair(4, 4);
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
        return std::make_pair(5, 4);
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
        return std::make_pair(5, 5);
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
        return std::make_pair(6, 5);
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
        return std::make_pair(6, 6);
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
        return std::make_pair(8, 5);
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
        return std::make_pair(8, 6);
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
        return std::make_pair(8, 8);
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
        return std::make_pair(10, 5);
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
        return std::make_pair(10, 6);
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
        return std::make_pair(10, 8);
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
        return std::make_pair(10, 10);
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
        return std::make_pair(12, 10);
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
        return std::make_pair(12, 12);
#ifdef VK_EXT_texture_compression_astc_hdr
    case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT:
        return std::make_pair(4, 4);
    case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT:
        return std::make_pair(5, 4);
    case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT:
        return std::make_pair(5, 5);
    case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT:
        return std::make_pair(6, 5);
    case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT:
        return std::make_pair(6, 6);
    case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT:
        return std::make_pair(8, 5);
    case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT:
        return std::make_pair(8, 6);
    case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT:
        return std::make_pair(8, 8);
    case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT:
        return std::make_pair(10, 5);
    case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT:
        return std::make_pair(10, 6);
    case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT:
        return std::make_pair(10, 8);
    case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT:
        return std::make_pair(10, 10);
    case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT:
        return std::make_pair(12, 10);
    case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT:
        return std::make_pair(12, 12);
#endif // VK_EXT_texture_compression_astc_hdr
#ifdef VK_IMG_format_pvrtc
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
        return std::make_pair(8, 4);
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return std::make_pair(4, 4);
#endif // VK_IMG_format_pvrtc
    default:
        MAGMA_ASSERT(false);
        return std::make_pair(1, 1);
    };
}

VkFormat Format::unormToSrgb() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_UNORM:
        return VK_FORMAT_R8_SRGB;
    case VK_FORMAT_R8G8_UNORM:
        return VK_FORMAT_R8G8_SRGB;
    case VK_FORMAT_R8G8B8_UNORM:
        return VK_FORMAT_R8G8B8_SRGB;
    case VK_FORMAT_B8G8R8_UNORM:
        return VK_FORMAT_B8G8R8_SRGB;
    case VK_FORMAT_R8G8B8A8_UNORM:
        return VK_FORMAT_R8G8B8A8_SRGB;
    case VK_FORMAT_B8G8R8A8_UNORM:
        return VK_FORMAT_B8G8R8A8_SRGB;
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
        return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
        return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
        return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
    case VK_FORMAT_BC2_UNORM_BLOCK:
        return VK_FORMAT_BC2_SRGB_BLOCK;
    case VK_FORMAT_BC3_UNORM_BLOCK:
        return VK_FORMAT_BC3_SRGB_BLOCK;
    case VK_FORMAT_BC7_UNORM_BLOCK:
        return VK_FORMAT_BC7_SRGB_BLOCK;
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
        return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
        return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
        return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
        return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
        return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
        return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
        return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
        return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
        return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
        return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
        return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
        return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
        return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
        return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
        return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
        return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
        return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
#ifdef VK_IMG_format_pvrtc
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
        return VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
        return VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
        return VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
        return VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;
#endif // VK_IMG_format_pvrtc
    default:
        MAGMA_ASSERT(unorm());
        return VK_FORMAT_UNDEFINED;
    }
}

VkFormat Format::srgbToUnorm() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R8_SRGB:
        return VK_FORMAT_R8_UNORM;
    case VK_FORMAT_R8G8_SRGB:
        return VK_FORMAT_R8G8_UNORM;
    case VK_FORMAT_R8G8B8_SRGB:
        return VK_FORMAT_R8G8B8_UNORM;
    case VK_FORMAT_B8G8R8_SRGB:
        return VK_FORMAT_B8G8R8_UNORM;
    case VK_FORMAT_R8G8B8A8_SRGB:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case VK_FORMAT_B8G8R8A8_SRGB:
        return VK_FORMAT_B8G8R8A8_UNORM;
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
        return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
        return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case VK_FORMAT_BC2_SRGB_BLOCK:
        return VK_FORMAT_BC2_UNORM_BLOCK;
    case VK_FORMAT_BC3_SRGB_BLOCK:
        return VK_FORMAT_BC3_UNORM_BLOCK;
    case VK_FORMAT_BC7_SRGB_BLOCK:
        return VK_FORMAT_BC7_UNORM_BLOCK;
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
        return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
        return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
        return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
        return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
        return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
        return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
        return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
        return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
        return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
        return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
        return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
        return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
        return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
        return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
        return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
        return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
#ifdef VK_IMG_format_pvrtc
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
        return VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
        return VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
        return VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
        return VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
#endif // VK_IMG_format_pvrtc
    default:
        MAGMA_ASSERT(srgb());
        return VK_FORMAT_UNDEFINED;
    }
}

std::size_t Format::texelSize() const noexcept
{
    switch (format)
    {
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
#ifdef VK_EXT_4444_formats
    case VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT:
    case VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT:
#endif // VK_EXT_4444_formats
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
#ifdef VK_KHR_maintenance5
    case VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR:
#endif // VK_KHR_maintenance5
        return sizeof(uint16_t);
    case VK_FORMAT_R4G4_UNORM_PACK8:
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R8_SNORM:
    case VK_FORMAT_R8_USCALED:
    case VK_FORMAT_R8_SSCALED:
    case VK_FORMAT_R8_UINT:
    case VK_FORMAT_R8_SINT:
    case VK_FORMAT_R8_SRGB:
#ifdef VK_KHR_maintenance5
    case VK_FORMAT_A8_UNORM_KHR:
#endif // VK_KHR_maintenance5
        return sizeof(uint8_t);
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R8G8_SNORM:
    case VK_FORMAT_R8G8_USCALED:
    case VK_FORMAT_R8G8_SSCALED:
    case VK_FORMAT_R8G8_UINT:
    case VK_FORMAT_R8G8_SINT:
    case VK_FORMAT_R8G8_SRGB:
        return 2 * sizeof(uint8_t);
    case VK_FORMAT_R8G8B8_UNORM:
    case VK_FORMAT_R8G8B8_SNORM:
    case VK_FORMAT_R8G8B8_USCALED:
    case VK_FORMAT_R8G8B8_SSCALED:
    case VK_FORMAT_R8G8B8_UINT:
    case VK_FORMAT_R8G8B8_SINT:
    case VK_FORMAT_R8G8B8_SRGB:
    case VK_FORMAT_B8G8R8_UNORM:
    case VK_FORMAT_B8G8R8_SNORM:
    case VK_FORMAT_B8G8R8_USCALED:
    case VK_FORMAT_B8G8R8_SSCALED:
    case VK_FORMAT_B8G8R8_UINT:
    case VK_FORMAT_B8G8R8_SINT:
    case VK_FORMAT_B8G8R8_SRGB:
        return 3 * sizeof(uint8_t);
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
        return 4 * sizeof(uint8_t);
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
        return sizeof(uint32_t);
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SNORM:
    case VK_FORMAT_R16_USCALED:
    case VK_FORMAT_R16_SSCALED:
    case VK_FORMAT_R16_UINT:
    case VK_FORMAT_R16_SINT:
    case VK_FORMAT_R16_SFLOAT:
        return sizeof(uint16_t);
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R16G16_SNORM:
    case VK_FORMAT_R16G16_USCALED:
    case VK_FORMAT_R16G16_SSCALED:
    case VK_FORMAT_R16G16_UINT:
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16_SFLOAT:
        return 2 * sizeof(uint16_t);
    case VK_FORMAT_R16G16B16_UNORM:
    case VK_FORMAT_R16G16B16_SNORM:
    case VK_FORMAT_R16G16B16_USCALED:
    case VK_FORMAT_R16G16B16_SSCALED:
    case VK_FORMAT_R16G16B16_UINT:
    case VK_FORMAT_R16G16B16_SINT:
    case VK_FORMAT_R16G16B16_SFLOAT:
        return 3 * sizeof(uint16_t);
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SNORM:
    case VK_FORMAT_R16G16B16A16_USCALED:
    case VK_FORMAT_R16G16B16A16_SSCALED:
    case VK_FORMAT_R16G16B16A16_UINT:
    case VK_FORMAT_R16G16B16A16_SINT:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return 4 * sizeof(uint16_t);
    // Map to 32-bit machine type
    case VK_FORMAT_R32_UINT:
        return sizeof(uint32_t);
    case VK_FORMAT_R32_SINT:
        return sizeof(int32_t);
    case VK_FORMAT_R32_SFLOAT:
        return sizeof(float);
    case VK_FORMAT_R32G32_UINT:
        return 2 * sizeof(uint32_t);
    case VK_FORMAT_R32G32_SINT:
        return 2 * sizeof(int32_t);
    case VK_FORMAT_R32G32_SFLOAT:
        return 2 * sizeof(float);
    case VK_FORMAT_R32G32B32_UINT:
        return 3 * sizeof(uint32_t);
    case VK_FORMAT_R32G32B32_SINT:
        return 3 * sizeof(int32_t);
    case VK_FORMAT_R32G32B32_SFLOAT:
        return 3 * sizeof(float);
    case VK_FORMAT_R32G32B32A32_UINT:
        return 4 * sizeof(uint32_t);
    case VK_FORMAT_R32G32B32A32_SINT:
        return 4 * sizeof(int32_t);
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return 4 * sizeof(float);
    // Map to 64-bit machine type
    case VK_FORMAT_R64_UINT:
        return sizeof(uint64_t);
    case VK_FORMAT_R64_SINT:
        return sizeof(int64_t);
    case VK_FORMAT_R64_SFLOAT:
        return sizeof(double);
    case VK_FORMAT_R64G64_UINT:
        return 2 * sizeof(uint64_t);
    case VK_FORMAT_R64G64_SINT:
        return 2 * sizeof(int64_t);
    case VK_FORMAT_R64G64_SFLOAT:
        return 2 * sizeof(double);
    case VK_FORMAT_R64G64B64_UINT:
        return 3 * sizeof(uint64_t);
    case VK_FORMAT_R64G64B64_SINT:
        return 3 * sizeof(int64_t);
    case VK_FORMAT_R64G64B64_SFLOAT:
        return 3 * sizeof(double);
    case VK_FORMAT_R64G64B64A64_UINT:
        return 4 * sizeof(uint64_t);
    case VK_FORMAT_R64G64B64A64_SINT:
        return 4 * sizeof(int64_t);
    case VK_FORMAT_R64G64B64A64_SFLOAT:
        return 4 * sizeof(double);
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
        return sizeof(float);
#ifdef VK_NV_optical_flow
    case VK_FORMAT_R16G16_S10_5_NV:
        return 2 * sizeof(uint16_t);
#endif // VK_NV_optical_flow
    default:
        return 0;
    }
}

std::size_t Format::blockSize() const noexcept
{
    if (bc())
        return bcSize();
    else if (etc2())
        return etc2Size();
    else if (eac())
        return eacSize();
    else if (astcLdr() || astcHdr())
        return astcSize();
    else if (pvrtc())
        return pvrtcSize();
    return 0;
}
} // namespace magma
