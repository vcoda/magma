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
#pragma once

#ifdef VK_KHR_sampler_ycbcr_conversion
namespace magma
{
    /* YCbCr sampler conversion used for processing inputs from video decoders and cameras. */

    struct YcbcrConversionSamplerState : VkSamplerYcbcrConversionCreateInfoKHR
    {
        constexpr YcbcrConversionSamplerState(VkSamplerYcbcrModelConversionKHR ycbcrModel,
            VkSamplerYcbcrRangeKHR ycbcrRange,
            VkFilter chromaFilter) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const YcbcrConversionSamplerState&) const noexcept;
    };
} // namespace magma

#include "ycbcrSamplerState.inl"

namespace magma
{
    namespace sampler
    {
        constexpr YcbcrConversionSamplerState rgbIdentityFullNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcrIdendityFullNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcr709FullNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcr601FullNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcr2020FullNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_NEAREST);

        constexpr YcbcrConversionSamplerState rgbIdentityNarrowNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcrIdendityNarrowNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcr709NarrowNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcr601NarrowNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_NEAREST);
        constexpr YcbcrConversionSamplerState ycbcr2020NarrowNearest(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_NEAREST);

        constexpr YcbcrConversionSamplerState rgbIdentityFullLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcrIdendityFullLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcr709FullLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcr601FullLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcr2020FullLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_FULL_KHR, VK_FILTER_LINEAR);

        constexpr YcbcrConversionSamplerState rgbIdentityNarrowLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_RGB_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcrIdendityNarrowLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_IDENTITY_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcr709NarrowLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_709_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcr601NarrowLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_601_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_LINEAR);
        constexpr YcbcrConversionSamplerState ycbcr2020NarrowLinear(VK_SAMPLER_YCBCR_MODEL_CONVERSION_YCBCR_2020_KHR, VK_SAMPLER_YCBCR_RANGE_ITU_NARROW_KHR, VK_FILTER_LINEAR);
    } // namespace sampler
} // namespace magma
#endif // VK_KHR_sampler_ycbcr_conversion
