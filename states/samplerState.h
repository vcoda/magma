/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../api/vulkan.h"

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
        constexpr SamplerState(float maxAnisotropy,
            VkSamplerAddressMode addressMode) noexcept;
        constexpr size_t hash() const noexcept;
        constexpr bool operator==(const SamplerState&) const noexcept;

    private:
        VkFilter magFilter;
        VkFilter minFilter;
        VkSamplerMipmapMode mipmapMode;
        VkSamplerAddressMode addressMode;
        bool anisotropyEnable;
        float maxAnisotropy;
        friend class Sampler;
    };
}

#include "samplerState.inl"

namespace magma
{
    namespace samplers
    {
        constexpr SamplerState nearestMipmapNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState linearMipmapNearestRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState linearMipmapLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
        constexpr SamplerState anisotropicRepeat(16.f, VK_SAMPLER_ADDRESS_MODE_REPEAT);

        constexpr SamplerState nearestMipmapNearestMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState linearMipmapNearestMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState linearMipmapLinearMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
        constexpr SamplerState anisotropicMirroredRepeat(16.f, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);

        constexpr SamplerState nearestMipmapNearestClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState linearMipmapNearestClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState linearMipmapLinearClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        constexpr SamplerState anisotropicClampToEdge(16.f, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

        constexpr SamplerState nearestMipmapNearestClampToBorder(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState linearMipmapNearestClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState linearMipmapLinearClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
        constexpr SamplerState anisotropicClampToBorder(16.f, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);

        constexpr SamplerState nearestMipmapNearestMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState linearMipmapNearestMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState linearMipmapLinearMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
        constexpr SamplerState anisotropicMirrorClampToEdge(16.f, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
    } // namespace samplers
} // namespace magma
