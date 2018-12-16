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
        SamplerState(VkFilter magFilter,
            VkFilter minFilter,
            VkSamplerMipmapMode mipmapMode,
            VkSamplerAddressMode addressMode) noexcept;
        SamplerState(float maxAnisotropy,
            VkSamplerAddressMode addressMode) noexcept;
        size_t hash() const noexcept;
        bool operator==(const SamplerState&) const noexcept;

    private:
        VkFilter magFilter;
        VkFilter minFilter;
        VkSamplerMipmapMode mipmapMode;
        VkSamplerAddressMode addressMode;
        bool anisotropyEnable;
        float maxAnisotropy;
        friend class Sampler;
    };

    namespace samplers
    {
        extern const SamplerState nearestMipmapNearestRepeat;
        extern const SamplerState linearMipmapNearestRepeat;
        extern const SamplerState linearMipmapLinearRepeat;
        extern const SamplerState anisotropicRepeat;

        extern const SamplerState nearestMipmapNearestMirroredRepeat;
        extern const SamplerState linearMipmapNearestMirroredRepeat;
        extern const SamplerState linearMipmapLinearMirroredRepeat;
        extern const SamplerState anisotropicMirroredRepeat;

        extern const SamplerState nearestMipmapNearestClampToEdge;
        extern const SamplerState linearMipmapNearestClampToEdge;
        extern const SamplerState linearMipmapLinearClampToEdge;
        extern const SamplerState anisotropicClampToEdge;

        extern const SamplerState nearestMipmapNearestClampToBorder;
        extern const SamplerState linearMipmapNearestClampToBorder;
        extern const SamplerState linearMipmapLinearClampToBorder;
        extern const SamplerState anisotropicClampToBorder;

        extern const SamplerState nearestMipmapNearestMirrorClampToEdge;
        extern const SamplerState linearMipmapNearestMirrorClampToEdge;
        extern const SamplerState linearMipmapLinearMirrorClampToEdge;
        extern const SamplerState anisotropicMirrorClampToEdge;
    } // namespace samplers
} // namespace magma
