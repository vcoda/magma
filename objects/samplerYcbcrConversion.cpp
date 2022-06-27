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
#include "../states/ycbcrSamplerState.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

#ifdef VK_KHR_sampler_ycbcr_conversion
namespace magma
{
SamplerYcbcrConversion::SamplerYcbcrConversion(std::shared_ptr<Device> device,
    VkFormat format,
    const YcbcrConversionSamplerState& state,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    VkChromaLocationKHR xChromaOffset /* VK_CHROMA_LOCATION_MIDPOINT_KHR */,
    VkChromaLocationKHR yChromaOffset /* VK_CHROMA_LOCATION_MIDPOINT_KHR */,
    bool forceExplicitReconstruction /* false */):
    NonDispatchable(VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR, std::move(device), std::move(allocator))
{
    VkSamplerYcbcrConversionCreateInfoKHR samplerInfo;
    samplerInfo.sType = state.sType;
    samplerInfo.format = format;
    samplerInfo.ycbcrModel = state.ycbcrModel;
    samplerInfo.ycbcrRange = state.ycbcrRange;
    samplerInfo.components = swizzle;
    samplerInfo.xChromaOffset = xChromaOffset;
    samplerInfo.yChromaOffset = yChromaOffset;
    samplerInfo.chromaFilter = state.chromaFilter;
    samplerInfo.forceExplicitReconstruction = MAGMA_BOOLEAN(forceExplicitReconstruction);
    const VkResult result = vkCreateSamplerYcbcrConversionKHR(MAGMA_HANDLE(device), &samplerInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Ycbcr sampler");
}

SamplerYcbcrConversion::~SamplerYcbcrConversion()
{
    vkDestroySamplerYcbcrConversionKHR(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
} // namespace magma
#endif // VK_KHR_sampler_ycbcr_conversion
