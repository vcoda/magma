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
#pragma once

namespace magma
{
    /* Nearest filtering is a sampling of nearest texel.
       Bilinear filtering is an interpolation of four texels in two directions.
       Trilinear filtering also performs linear interpolation between mipmaps.
       Anisotropic filtering performs long and narrow projection of the pixel
       filter footprint into texture space. */

    enum class MagFilter
    {
        Nearest, Bilinear
    };

    enum class MipFilter
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
        constexpr MagFilter getMagFilter() const noexcept;
        constexpr MipFilter getMipFilter() const noexcept;
        constexpr std::size_t hash() const noexcept;
        constexpr bool operator==(const SamplerState&) const noexcept;
    };

    /* Anisotropic sampler state takes into account long and narrow (i.e. anisotropic)
       projection of the pixel filter footprint into texture space. */

    struct AnisotropicSamplerState : SamplerState
    {
        constexpr AnisotropicSamplerState(VkSamplerAddressMode addressMode,
            float maxAnisotropy = std::numeric_limits<float>::max()) noexcept;
    };
}

#include "samplerState.inl"

namespace magma
{
    namespace sampler
    {
        constexpr SamplerState magMinMipNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magLinearMinMipNearestRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magNearestMinLinearMipNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinNearestMipLinearRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinLinearMipNearestRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magNearestMinMipLinearRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magLinearMinNearestMipLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinMipLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicRepeat(VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magMinMipNearestMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magLinearMinMipNearestMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magNearestMinLinearMipNearestMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinNearestMipLinearMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinLinearMipNearestMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magNearestMinMipLinearMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magLinearMinNearestMipLinearMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinMipLinearMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicMirroredRepeat(VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);

        constexpr SamplerState magMinMipNearestClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magLinearMinMipNearestClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magNearestMinLinearMipNearestClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinNearestMipLinearClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinLinearMipNearestClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magNearestMinMipLinearClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magLinearMinNearestMipLinearClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinMipLinearClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicClampToEdge(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

        constexpr SamplerState magMinMipNearestClampToBorder(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magLinearMinMipNearestClampToBorder(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magNearestMinLinearMipNearestClampToBorder(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinNearestMipLinearClampToBorder(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinLinearMipNearestClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magNearestMinMipLinearClampToBorder(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magLinearMinNearestMipLinearClampToBorder(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinMipLinearClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicClampToBorder(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);

#   ifdef VK_KHR_sampler_mirror_clamp_to_edge
        constexpr SamplerState magMinMipNearestMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magLinearMinMipNearestMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magNearestMinLinearMipNearestMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magMinNearestMipLinearMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magMinLinearMipNearestMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magNearestMinMipLinearMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magLinearMinNearestMipLinearMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magMinMipLinearMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicMirrorClampToEdge(VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
#   endif // VK_KHR_sampler_mirror_clamp_to_edge

#   ifdef VK_EXT_filter_cubic
        constexpr SamplerState magCubicMinMipNearestRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magCubicMinLinearMipNearestRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magCubicMinMipLinearRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinCubicMipLinearRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magCubicMinMipNearestMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magCubicMinLinearMipNearestMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magCubicMinMipLinearMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinCubicMipLinearMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);

        constexpr SamplerState magCubicMinMipNearestClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magCubicMinLinearMipNearestClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magCubicMinMipLinearClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinCubicMipLinearClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

        constexpr SamplerState magCubicMinMipNearestClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magCubicMinLinearMipNearestClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magCubicMinMipLinearClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinCubicMipLinearClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);

#   ifdef VK_KHR_sampler_mirror_clamp_to_edge
        constexpr SamplerState magCubicMinMipNearestMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magCubicMinLinearMipNearestMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magCubicMinMipLinearMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
        constexpr SamplerState magMinCubicMipLinearMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR);
#   endif // VK_KHR_sampler_mirror_clamp_to_edge
#   endif // VK_EXT_filter_cubic
    } // namespace sampler
} // namespace magma
