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
#include "samplerYcbcrConversion.h"
#include "device.h"
#include "../states/samplerYcbcrConversionState.h"
#include "../allocator/allocator.h"
#include "../misc/extProcAddress.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_sampler_ycbcr_conversion
SamplerYcbcrConversion::SamplerYcbcrConversion(std::shared_ptr<Device> device,
    VkFormat format,
    const SamplerYcbcrConversionState& state,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    VkChromaLocationKHR xChromaOffset /* VK_CHROMA_LOCATION_MIDPOINT_KHR */,
    VkChromaLocationKHR yChromaOffset /* VK_CHROMA_LOCATION_MIDPOINT_KHR */,
    bool forceExplicitReconstruction /* false */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR, std::move(device), std::move(allocator))
{
    VkSamplerYcbcrConversionCreateInfoKHR samplerYcbcrConversionInfo;
    samplerYcbcrConversionInfo.sType = state.sType;
    samplerYcbcrConversionInfo.pNext = extendedInfo.chainNodes();
    samplerYcbcrConversionInfo.format = format;
    samplerYcbcrConversionInfo.ycbcrModel = state.ycbcrModel;
    samplerYcbcrConversionInfo.ycbcrRange = state.ycbcrRange;
    samplerYcbcrConversionInfo.components = swizzle;
    samplerYcbcrConversionInfo.xChromaOffset = xChromaOffset;
    samplerYcbcrConversionInfo.yChromaOffset = yChromaOffset;
    samplerYcbcrConversionInfo.chromaFilter = state.chromaFilter;
    samplerYcbcrConversionInfo.forceExplicitReconstruction = MAGMA_BOOLEAN(forceExplicitReconstruction);
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateSamplerYcbcrConversionKHR, VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME);
    const VkResult result = vkCreateSamplerYcbcrConversionKHR(MAGMA_HANDLE(device), &samplerYcbcrConversionInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Ycbcr sampler");
}

SamplerYcbcrConversion::~SamplerYcbcrConversion()
{
    MAGMA_DEVICE_EXTENSION(vkDestroySamplerYcbcrConversionKHR);
    vkDestroySamplerYcbcrConversionKHR(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
#endif // VK_KHR_sampler_ycbcr_conversion
} // namespace magma
