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
#include "sampler.h"
#include "device.h"
#include "../states/samplerState.h"
#include "../shared.h"

namespace magma
{
Sampler::Sampler(std::shared_ptr<const Device> device, const SamplerState& state, 
    float mipLodBias /* 0 */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, device)
{
    VkSamplerCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.magFilter = state.magFilter;
    info.minFilter = state.minFilter;
    info.mipmapMode = state.mipmapMode;
    info.addressModeU = state.addressMode;
    info.addressModeV = state.addressMode;
    info.addressModeW = state.addressMode;
    info.mipLodBias = mipLodBias;
    info.anisotropyEnable = MAGMA_BOOLEAN(state.anisotropyEnable);
    info.maxAnisotropy = state.maxAnisotropy;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.minLod = 0.f;
    info.maxLod = 1.f;
    info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;
    const VkResult create = vkCreateSampler(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create sampler");
}

Sampler::Sampler(std::shared_ptr<const Device> device, 
    VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode, 
    float mipLodBias /* 0 */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, device)
{
    VkSamplerCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.magFilter = magFilter;
    info.minFilter = minFilter;
    info.mipmapMode = mipmapMode;
    info.addressModeU = addressMode;
    info.addressModeV = addressMode;
    info.addressModeW = addressMode;
    info.mipLodBias = mipLodBias;
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 0.f;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.minLod = 0.f;
    info.maxLod = 1.f;
    info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;
    const VkResult create = vkCreateSampler(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create sampler");
}

Sampler::~Sampler()
{
    vkDestroySampler(*device, handle, nullptr);
}
} // namespace magma
