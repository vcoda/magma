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
#include "samplerState.h"
#include "../helpers/hash.h"

namespace magma
{
SamplerState::SamplerState(VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode) noexcept:
    magFilter(magFilter),
    minFilter(minFilter),
    mipmapMode(mipmapMode),
    addressMode(addressMode),
    anisotropyEnable(false),
    maxAnisotropy(0.f)
{}

SamplerState::SamplerState(float maxAnisotropy, VkSamplerAddressMode addressMode) noexcept:
    magFilter(VK_FILTER_LINEAR),
    minFilter(VK_FILTER_LINEAR),
    mipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR),
    addressMode(addressMode),
    anisotropyEnable(true),
    maxAnisotropy(maxAnisotropy)
{}

size_t SamplerState::hash() const noexcept
{
    return helpers::hashVariadic(
        magFilter,
        minFilter,
        mipmapMode,
        addressMode,
        anisotropyEnable,
        maxAnisotropy);
}

bool SamplerState::operator==(const SamplerState& other) const noexcept
{
    return (magFilter == other.magFilter) &&
        (minFilter == other.minFilter) &&
        (mipmapMode == other.mipmapMode) &&
        (addressMode == other.addressMode) &&
        (anisotropyEnable == other.anisotropyEnable) &&
        (maxAnisotropy == other.maxAnisotropy);
}

namespace samplers
{
const SamplerState nearestMipmapNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
const SamplerState linearMipmapNearestRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
const SamplerState linearMipmapLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
const SamplerState anisotropicRepeat(16.f, VK_SAMPLER_ADDRESS_MODE_REPEAT);

const SamplerState nearestMipmapNearestMirroredRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
const SamplerState linearMipmapNearestMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
const SamplerState linearMipmapLinearMirroredRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);
const SamplerState anisotropicMirroredRepeat(16.f, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT);

const SamplerState nearestMipmapNearestClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
const SamplerState linearMipmapNearestClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
const SamplerState linearMipmapLinearClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
const SamplerState anisotropicClampToEdge(16.f, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

const SamplerState nearestMipmapNearestClampToBorder(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
const SamplerState linearMipmapNearestClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
const SamplerState linearMipmapLinearClampToBorder(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
const SamplerState anisotropicClampToBorder(16.f, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);

const SamplerState nearestMipmapNearestMirrorClampToEdge(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
const SamplerState linearMipmapNearestMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
const SamplerState linearMipmapLinearMirrorClampToEdge(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
const SamplerState anisotropicMirrorClampToEdge(16.f, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE);
} // namespace samplers
} // namespace magma
