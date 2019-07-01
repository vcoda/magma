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
#include "../internal/hash.h"

namespace magma
{
constexpr SamplerState::SamplerState(VkFilter magFilter, VkFilter minFilter, 
    VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode) noexcept:
    magFilter(magFilter),
    minFilter(minFilter),
    mipmapMode(mipmapMode),
    addressMode(addressMode),
    anisotropyEnable(false),
    maxAnisotropy(0.f)
{}

constexpr size_t SamplerState::hash() const noexcept
{
    return internal::hashArgs(
        magFilter,
        minFilter,
        mipmapMode,
        addressMode,
        anisotropyEnable,
        maxAnisotropy);
}

constexpr bool SamplerState::operator==(const SamplerState& other) const noexcept
{
    return (magFilter == other.magFilter) &&
        (minFilter == other.minFilter) &&
        (mipmapMode == other.mipmapMode) &&
        (addressMode == other.addressMode) &&
        (anisotropyEnable == other.anisotropyEnable) &&
        (maxAnisotropy == other.maxAnisotropy);
}

constexpr AnisotropicSamplerState::AnisotropicSamplerState(VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode addressMode,
    float maxAnisotropy /* std::numeric_limits<float>::max() */ ):
    SamplerState(magFilter, minFilter, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode)
{
    anisotropyEnable = true;
    this->maxAnisotropy = maxAnisotropy; // Value will be clamped against hardware limit
}
} // namespace magma
