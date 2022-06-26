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
#include "sampler.h"
#include "device.h"
#include "physicalDevice.h"
#include "../states/samplerState.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Sampler::Sampler(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_SAMPLER, std::move(device), std::move(allocator))
{}

Sampler::Sampler(std::shared_ptr<Device> device, const SamplerState& state,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), state, border::opaqueBlackFloat, std::move(allocator))
{}

Sampler::Sampler(std::shared_ptr<Device> device, const SamplerState& state, const BorderColor& borderColor,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), std::move(allocator))
{
    VkSamplerCreateInfo samplerInfo = state;
    if (samplerInfo.anisotropyEnable)
    {   // If anisotropyEnable is VK_TRUE, maxAnisotropy must be between 1.0 and VkPhysicalDeviceLimits::maxSamplerAnisotropy, inclusive
        const VkPhysicalDeviceProperties properties = device->getPhysicalDevice()->getProperties();
        samplerInfo.maxAnisotropy = std::max(1.f, std::min(state.maxAnisotropy, properties.limits.maxSamplerAnisotropy));
    }
    samplerInfo.borderColor = borderColor.getColor();
    const VkResult result = vkCreateSampler(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create sampler");
}

Sampler::~Sampler()
{
    vkDestroySampler(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

LodSampler::LodSampler(std::shared_ptr<Device> device, const SamplerState& state,
    float mipLodBias, float minLod, float maxLod, const BorderColor& borderColor,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), std::move(allocator))
{
    VkSamplerCreateInfo samplerInfo = state;
    samplerInfo.mipLodBias = mipLodBias;
    if (samplerInfo.anisotropyEnable)
    {   // If anisotropyEnable is VK_TRUE, maxAnisotropy must be between 1.0 and VkPhysicalDeviceLimits::maxSamplerAnisotropy, inclusive
        const VkPhysicalDeviceProperties properties = device->getPhysicalDevice()->getProperties();
        samplerInfo.maxAnisotropy = std::max(1.f, std::min(state.maxAnisotropy, properties.limits.maxSamplerAnisotropy));
    }
    samplerInfo.minLod = minLod;
    samplerInfo.maxLod = maxLod;
    samplerInfo.borderColor = borderColor.getColor();
    const VkResult result = vkCreateSampler(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create lod sampler");
}

UnnormalizedSampler::UnnormalizedSampler(std::shared_ptr<Device> device, bool linearFilter, const BorderColor& borderColor,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Sampler(std::move(device), std::move(allocator))
{
    VkSamplerCreateInfo samplerInfo;
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.pNext = nullptr;
    samplerInfo.flags = 0;
    samplerInfo.magFilter = linearFilter ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    samplerInfo.minFilter = linearFilter ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.mipLodBias = 0.f;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.f;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.minLod = 0.f;
    samplerInfo.maxLod = 0.f;
    samplerInfo.borderColor = borderColor.getColor();
    samplerInfo.unnormalizedCoordinates = VK_TRUE;
    const VkResult result = vkCreateSampler(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create unnormalized sampler");
}
} // namespace magma
