/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#pragma once

namespace magma
{
    /* Nearest filtering is a sampling of nearest texel.
       Bilinear filtering is an interpolation of four texels in two directions.
       Trilinear filtering also performs linear interpolation between mipmaps.
       Anisotropic filtering performs long and narrow projection of the pixel
       filter footprint into texture space. */

    enum class MagFilter : uint8_t
    {
        Nearest, Bilinear
    };

    enum class MipFilter : uint8_t
    {
        Nearest, Bilinear, Trilinear, Anisotropic, Partial
    };

    /* Sampler state defines texture filtration parameters
       during texture sampling in the fragment shader. */

    struct SamplerState : VkSamplerCreateInfo
    {
    public:
        constexpr SamplerState(VkFilter magFilter,
            VkFilter minFilter,
            VkSamplerMipmapMode mipmapMode,
            VkSamplerAddressMode addressMode) noexcept;
        constexpr SamplerState(const SamplerState&) noexcept;
        constexpr bool chained() const noexcept { return pNext != nullptr; }
        constexpr MagFilter getMagFilter() const noexcept;
        constexpr MipFilter getMipFilter() const noexcept;
        constexpr hash_t hash() const noexcept;
        hash_t chainedHash() const noexcept;
        constexpr bool operator==(const SamplerState&) const noexcept;
    };

    /* Anisotropic sampler state takes into account long and narrow
       (i.e. anisotropic) projection of the pixel filter footprint
       into texture space. */

    struct AnisotropicSamplerState : SamplerState
    {
        constexpr AnisotropicSamplerState(VkSamplerAddressMode addressMode,
            float maxAnisotropy = std::numeric_limits<float>::max()) noexcept;
    };
}

#include "samplerState.inl"

namespace magma
{
#define MAGMA_SAMPLER_STATE_PERMUTATIONS(Suffix, addressMode)\
    constexpr SamplerState magMinMipNearest##Suffix(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, addressMode);\
    constexpr SamplerState magLinearMinMipNearest##Suffix(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, addressMode);\
    constexpr SamplerState magNearestMinLinearMipNearest##Suffix(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, addressMode);\
    constexpr SamplerState magMinNearestMipLinear##Suffix(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode);\
    constexpr SamplerState magMinLinearMipNearest##Suffix(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, addressMode);\
    constexpr SamplerState magNearestMinMipLinear##Suffix(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode);\
    constexpr SamplerState magLinearMinNearestMipLinear##Suffix(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode);\
    constexpr SamplerState magMinMipLinear##Suffix(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode);\
    constexpr AnisotropicSamplerState magMinLinearMipAnisotropic##Suffix(addressMode);

#ifdef VK_EXT_filter_cubic
#define MAGMA_CUBIC_SAMPLER_STATE_PERMUTATIONS(Suffix, addressMode)\
    constexpr SamplerState magCubicMinMipNearest##Suffix(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, addressMode);\
    constexpr SamplerState magCubicMinLinearMipNearest##Suffix(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, addressMode);\
    constexpr SamplerState magCubicMinMipLinear##Suffix(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode);\
    constexpr SamplerState magMinCubicMipLinear##Suffix(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode);
#endif // VK_EXT_filter_cubic

    namespace sampler
    {
        MAGMA_SAMPLER_STATE_PERMUTATIONS(Repeat, VK_SAMPLER_ADDRESS_MODE_REPEAT)
        MAGMA_SAMPLER_STATE_PERMUTATIONS(MirroredRepeat, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT)
        MAGMA_SAMPLER_STATE_PERMUTATIONS(ClampToEdge, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
        MAGMA_SAMPLER_STATE_PERMUTATIONS(ClampToBorder, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
    #ifdef VK_KHR_sampler_mirror_clamp_to_edge
        MAGMA_SAMPLER_STATE_PERMUTATIONS(MirrorClampToEdge, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR)
    #endif

    #ifdef VK_EXT_filter_cubic
        MAGMA_CUBIC_SAMPLER_STATE_PERMUTATIONS(Repeat, VK_SAMPLER_ADDRESS_MODE_REPEAT)
        MAGMA_CUBIC_SAMPLER_STATE_PERMUTATIONS(MirroredRepeat, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT)
        MAGMA_CUBIC_SAMPLER_STATE_PERMUTATIONS(ClampToEdge, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
        MAGMA_CUBIC_SAMPLER_STATE_PERMUTATIONS(ClampToBorder, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
    #ifdef VK_KHR_sampler_mirror_clamp_to_edge
        MAGMA_CUBIC_SAMPLER_STATE_PERMUTATIONS(MirrorClampToEdge, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR)
    #endif
    #endif // VK_EXT_filter_cubic
    } // namespace sampler
} // namespace magma
