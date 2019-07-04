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
#include <algorithm>
#include "sampler.h"
#include "device.h"
#include "physicalDevice.h"
#include "../states/samplerState.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../internal/shared.h"

namespace magma
{
Sampler::Sampler(std::shared_ptr<Device> device, const SamplerState& state,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_SAMPLER, std::move(device), std::move(allocator))
{
    const VkPhysicalDeviceProperties properties = this->device->getPhysicalDevice()->getProperties();
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
    info.mipLodBias = 0.f;
    info.anisotropyEnable = MAGMA_BOOLEAN(state.anisotropyEnable);
    if (info.anisotropyEnable)
        info.maxAnisotropy = std::min(state.maxAnisotropy, properties.limits.maxSamplerAnisotropy);
    else
        info.maxAnisotropy = 0.f;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.minLod = 0.f;
    info.maxLod = std::numeric_limits<float>::max();
    info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;
    const VkResult create = vkCreateSampler(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create sampler");
}

Sampler::Sampler(std::shared_ptr<Device> device, VkFilter magFilter, VkFilter minFilter,
    VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode,
    float mipLodBias /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(device, SamplerState(magFilter, minFilter, mipmapMode, addressMode), mipLodBias, allocator)
{}

Sampler::~Sampler()
{
    vkDestroySampler(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

LodSampler::LodSampler(std::shared_ptr<Device> device, const SamplerState& state, 
    float mipLodBias, float minLod, float maxLod, 
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), std::move(allocator))
{
    const VkPhysicalDeviceProperties properties = this->device->getPhysicalDevice()->getProperties();
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
    if (info.anisotropyEnable)
        info.maxAnisotropy = std::min(state.maxAnisotropy, properties.limits.maxSamplerAnisotropy);
    else
        info.maxAnisotropy = 0.f;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.minLod = minLod;
    info.maxLod = maxLod;
    info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;
    const VkResult create = vkCreateSampler(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create LOD sampler");
}

DepthSampler::DepthSampler(std::shared_ptr<Device> device, const DepthSamplerState& state,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), std::move(allocator))
{
    VkSamplerCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.magFilter = state.magFilter;
    info.minFilter = state.minFilter;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.mipLodBias = 0.f;
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 0.f;
    info.compareEnable = VK_TRUE;
    info.compareOp = state.compareOp;
    info.minLod = 0.f;
    info.maxLod = std::numeric_limits<float>::max();
    info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    info.unnormalizedCoordinates = VK_FALSE;
    const VkResult create = vkCreateSampler(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create depth sampler");
}

UnnormalizedSampler::UnnormalizedSampler(std::shared_ptr<Device> device, bool linearFilter,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), std::move(allocator))
{
    VkSamplerCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.magFilter = linearFilter ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    info.minFilter = linearFilter ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.mipLodBias = 0.f;
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 0.f;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_NEVER;
    info.minLod = 0.f;
    info.maxLod = 0.f;
    info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    info.unnormalizedCoordinates = VK_TRUE;
    const VkResult create = vkCreateSampler(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create unnormalized sampler");
}
} // namespace magma
