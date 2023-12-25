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
std::unordered_map<PhysicalDevice*, float> Sampler::maxAnisotropies;

Sampler::Sampler(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_SAMPLER, std::move(device), std::move(allocator))
{}

Sampler::Sampler(std::shared_ptr<Device> device_, const SamplerState& state,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const BorderColor& borderColor /* opaqueBlackFloat */):
    Sampler(std::move(device_), std::move(allocator))
{
    VkSamplerCreateInfo samplerInfo = state;
    if (samplerInfo.anisotropyEnable)
    {   // If anisotropyEnable is VK_TRUE, maxAnisotropy must be between 1.0 and VkPhysicalDeviceLimits::maxSamplerAnisotropy, inclusive
        samplerInfo.maxAnisotropy = std::max(1.f, std::min(state.maxAnisotropy, getMaxAnisotropy()));
    }
    samplerInfo.borderColor = borderColor.getColor();
#ifdef VK_EXT_custom_border_color
    VkSamplerCustomBorderColorCreateInfoEXT samplerCustomBorderInfo;
    if (borderColor.custom())
    {
        samplerCustomBorderInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT;
        samplerCustomBorderInfo.pNext = samplerInfo.pNext;
        samplerCustomBorderInfo.customBorderColor = borderColor.getCustomColor();
        samplerCustomBorderInfo.format = borderColor.getFormat();
        samplerInfo.pNext = &samplerCustomBorderInfo;
    }
#endif // VK_EXT_custom_border_color
    const VkResult result = vkCreateSampler(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create sampler");
}

Sampler::~Sampler()
{
    vkDestroySampler(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

float Sampler::getMaxAnisotropy() const noexcept
{
    PhysicalDevice *physicalDevice = device->getPhysicalDevice().get();
    if (maxAnisotropies.find(physicalDevice) == maxAnisotropies.end())
    {
        const VkPhysicalDeviceProperties properties = physicalDevice->getProperties();
        const VkPhysicalDeviceLimits& limits = properties.limits;
        maxAnisotropies[physicalDevice] = limits.maxSamplerAnisotropy;
    }
    return maxAnisotropies[physicalDevice];
}

LodSampler::LodSampler(std::shared_ptr<Device> device_, const SamplerState& state,
    float mipLodBias, float minLod, float maxLod,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const BorderColor& borderColor /* opaqueBlackFloat */):
    Sampler(std::move(device_), std::move(allocator))
{
    VkSamplerCreateInfo samplerInfo;
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.pNext = state.pNext;
    samplerInfo.flags = state.flags;
    samplerInfo.magFilter = state.magFilter;
    samplerInfo.minFilter = state.minFilter;
    samplerInfo.mipmapMode = state.mipmapMode;
    samplerInfo.addressModeU = state.addressModeU;
    samplerInfo.addressModeV = state.addressModeV;
    samplerInfo.addressModeW = state.addressModeW;
    samplerInfo.mipLodBias = mipLodBias;
    samplerInfo.anisotropyEnable = state.anisotropyEnable && (maxLod - minLod > 1.f);
    samplerInfo.maxAnisotropy = state.maxAnisotropy;
    samplerInfo.compareEnable = state.compareEnable;
    samplerInfo.compareOp = state.compareOp;
    samplerInfo.minLod = minLod;
    samplerInfo.maxLod = maxLod;
    samplerInfo.borderColor = borderColor.getColor();
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    if (samplerInfo.anisotropyEnable)
    {   // If anisotropyEnable is VK_TRUE, maxAnisotropy must be between 1.0 and VkPhysicalDeviceLimits::maxSamplerAnisotropy, inclusive
        samplerInfo.maxAnisotropy = std::max(1.f, std::min(state.maxAnisotropy, getMaxAnisotropy()));
    }
#ifdef VK_EXT_custom_border_color
    VkSamplerCustomBorderColorCreateInfoEXT samplerCustomBorderInfo;
    if (borderColor.custom())
    {
        samplerCustomBorderInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT;
        samplerCustomBorderInfo.pNext = samplerInfo.pNext;
        samplerCustomBorderInfo.customBorderColor = borderColor.getCustomColor();
        samplerCustomBorderInfo.format = borderColor.getFormat();
        samplerInfo.pNext = &samplerCustomBorderInfo;
    }
#endif // VK_EXT_custom_border_color
    const VkResult result = vkCreateSampler(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create lod sampler");
}

UnnormalizedSampler::UnnormalizedSampler(std::shared_ptr<Device> device, bool linearFilter,
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
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_TRUE;
    const VkResult result = vkCreateSampler(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create unnormalized sampler");
}
} // namespace magma
