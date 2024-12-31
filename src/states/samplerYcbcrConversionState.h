/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

#ifdef VK_KHR_sampler_ycbcr_conversion
namespace magma
{
    /* YCbCr sampler conversion used for processing inputs from video decoders and cameras. */

    struct SamplerYcbcrConversionState : VkSamplerYcbcrConversionCreateInfoKHR
    {
        constexpr SamplerYcbcrConversionState(VkSamplerYcbcrModelConversionKHR ycbcrModel,
            VkSamplerYcbcrRangeKHR ycbcrRange,
            VkFilter chromaFilter) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const SamplerYcbcrConversionState&) const noexcept;
    };
} // namespace magma

#include "samplerYcbcrConversionState.inl"

namespace magma
{
#define MAGMA_SAMPLER_YCBCR_CONVERSION_STATE_PERMUTATIONS(name, ycbcrModel)\
    constexpr SamplerYcbcrConversionState name##FullNearest(ycbcrModel, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_NEAREST);\
    constexpr SamplerYcbcrConversionState name##NarrowNearest(ycbcrModel, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_NEAREST);\
    constexpr SamplerYcbcrConversionState name##FullLinear(ycbcrModel, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_LINEAR);\
    constexpr SamplerYcbcrConversionState name##NarrowLinear(ycbcrModel, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_LINEAR);

    namespace sampler
    {
        MAGMA_SAMPLER_YCBCR_CONVERSION_STATE_PERMUTATIONS(rgbIdentity, VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY_KHR)
        MAGMA_SAMPLER_YCBCR_CONVERSION_STATE_PERMUTATIONS(ycbcrIdendity, VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY_KHR)
        MAGMA_SAMPLER_YCBCR_CONVERSION_STATE_PERMUTATIONS(ycbcr709, VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709_KHR)
        MAGMA_SAMPLER_YCBCR_CONVERSION_STATE_PERMUTATIONS(ycbcr601, VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601_KHR)
        MAGMA_SAMPLER_YCBCR_CONVERSION_STATE_PERMUTATIONS(ycbcr2020, VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020_KHR)
    } // namespace sampler
} // namespace magma
#endif // VK_KHR_sampler_ycbcr_conversion
