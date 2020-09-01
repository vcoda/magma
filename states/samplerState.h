/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
    /* Predefined sampler state of sampler object. */

    class SamplerState
    {
    public:
        constexpr SamplerState(VkFilter magFilter,
            VkFilter minFilter,
            VkSamplerMipmapMode mipmapMode,
            VkSamplerAddressMode addressMode) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const SamplerState&) const noexcept;

    protected:
        VkFilter magFilter;
        VkFilter minFilter;
        VkSamplerMipmapMode mipmapMode;
        VkSamplerAddressMode addressMode;
        bool anisotropyEnable;
        float maxAnisotropy;
        friend class Sampler;
        friend class LodSampler;
    };

    class AnisotropicSamplerState : public SamplerState
    {
    public:
        constexpr AnisotropicSamplerState(VkFilter magFilter,
            VkFilter minFilter,
            VkSamplerAddressMode addressMode,
            float maxAnisotropy = std::numeric_limits<float>::max()) noexcept;
    };

    /* Depth map sampler state. */

    class DepthSamplerState
    {
    public:
        constexpr DepthSamplerState(VkFilter magFilter,
            VkFilter minFilter,
            VkCompareOp compareOp) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const DepthSamplerState&) const noexcept;

    private:
        VkFilter magFilter;
        VkFilter minFilter;
        VkCompareOp compareOp;
        friend class DepthSampler;
    };
}

#include "samplerState.inl"

namespace magma
{
    namespace samplers
    {
        constexpr SamplerState magMinMipNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magLinearMinMipNearestRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magNearestMinLinearMipNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinNearestMipLinearRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinLinearMipNearestRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magNearestMinMipLinearRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magLinearMinNearesetMipLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinMipLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magMinMipNearestMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magLinearMinMipNearestMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magNearestMinLinearMipNearestMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinNearestMipLinearMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinLinearMipNearestMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magNearestMinMipLinearMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magLinearMinNearesetMipLinearMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinMipLinearMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);

        constexpr SamplerState magMinMipNearestClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magLinearMinMipNearestClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magNearestMinLinearMipNearestClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinNearestMipLinearClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinLinearMipNearestClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magNearestMinMipLinearClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magLinearMinNearesetMipLinearClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinMipLinearClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

        constexpr SamplerState magMinMipNearestClampToBorder(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magLinearMinMipNearestClampToBorder(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magNearestMinLinearMipNearestClampToBorder(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinNearestMipLinearClampToBorder(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinLinearMipNearestClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magNearestMinMipLinearClampToBorder(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magLinearMinNearesetMipLinearClampToBorder(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinMipLinearClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);

        constexpr SamplerState magMinMipNearestMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magLinearMinMipNearestMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magNearestMinLinearMipNearestMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magMinNearestMipLinearMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magMinLinearMipNearestMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magNearestMinMipLinearMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magLinearMinNearesetMipLinearMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magMinMipLinearMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr AnisotropicSamplerState magMinLinearMipAnisotropicMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);

#ifdef VK_EXT_filter_cubic
        constexpr SamplerState magCubicMinMipNearestRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magCubicMinLinearMipNearestRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magCubicMinMipLinearRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState magMinCubicMipLinearRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr AnisotropicSamplerState magMinCubicMipAnisotropicRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magCubicMinMipNearestMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magCubicMinLinearMipNearestMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magCubicMinMipLinearMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState magMinCubicMipLinearMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr AnisotropicSamplerState magMinCubicMipAnisotropicMirroredRepeat(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magCubicMinMipNearestClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magCubicMinLinearMipNearestClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magCubicMinMipLinearClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState magMinCubicMipLinearClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr AnisotropicSamplerState magMinCubicMipAnisotropicClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magCubicMinMipNearestClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magCubicMinLinearMipNearestClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magCubicMinMipLinearClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState magMinCubicMipLinearClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr AnisotropicSamplerState magMinCubicMipAnisotropicClampToBorder(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState magCubicMinMipNearestMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magCubicMinLinearMipNearestMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magCubicMinMipLinearMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState magMinCubicMipLinearMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr AnisotropicSamplerState magMinCubicMipAnisotropicMirrorClampToEdge(VK_FILTER_CUBIC_EXT, VK_FILTER_CUBIC_EXT, VK_SAMPLER_ADDRESS_MODE_REPEAT);
#endif // VK_EXT_filter_cubic

        constexpr DepthSamplerState magMinNearestCompareNever(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_NEVER);
        constexpr DepthSamplerState magMinNearestCompareLess(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_LESS);
        constexpr DepthSamplerState magMinNearestCompareEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_EQUAL);
        constexpr DepthSamplerState magMinNearestCompareLessOrEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_LESS_OR_EQUAL);
        constexpr DepthSamplerState magMinNearestCompareGreater(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_GREATER);
        constexpr DepthSamplerState magMinNearestCompareNotEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_NOT_EQUAL);
        constexpr DepthSamplerState magMinNearestCompareGreaterOrEqual(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_GREATER_OR_EQUAL);
        constexpr DepthSamplerState magMinNearestCompareAlways(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_COMPARE_OP_ALWAYS);

        constexpr DepthSamplerState magMinLinearCompareNever(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_NEVER);
        constexpr DepthSamplerState magMinLinearCompareLess(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_LESS);
        constexpr DepthSamplerState magMinLinearCompareEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_EQUAL);
        constexpr DepthSamplerState magMinLinearCompareLessOrEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_LESS_OR_EQUAL);
        constexpr DepthSamplerState magMinLinearCompareGreater(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_GREATER);
        constexpr DepthSamplerState magMinLinearCompareNotEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_NOT_EQUAL);
        constexpr DepthSamplerState magMinLinearCompareGreaterOrEqual(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_GREATER_OR_EQUAL);
        constexpr DepthSamplerState magMinLinearCompareAlways(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_COMPARE_OP_ALWAYS);
    } // namespace samplers
} // namespace magma
