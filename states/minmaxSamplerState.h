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
#include "samplerState.h"

#ifdef VK_EXT_sampler_filter_minmax
namespace magma
{
    /* Minmax sampler allows to produce a filtered texel value
       by computing a component-wise minimum or maximum of the
       texels that would normally be averaged. */

    struct MinMaxSamplerState : SamplerState
    {
        constexpr MinMaxSamplerState(const SamplerState& state,
            VkSamplerReductionModeEXT reductionMode) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const MinMaxSamplerState&) const noexcept;

        const VkSamplerReductionModeCreateInfoEXT reductionModeInfo;
    };
}

#include "minmaxSamplerState.inl"

namespace magma
{
#define MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(AddressMode, Suffix, reductionMode)\
    constexpr MinMaxSamplerState magMinMipNearest##AddressMode##Suffix(magMinMipNearest##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magLinearMinMipNearest##AddressMode##Suffix(magLinearMinMipNearest##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magNearestMinLinearMipNearest##AddressMode##Suffix(magNearestMinLinearMipNearest##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magMinNearestMipLinear##AddressMode##Suffix(magMinNearestMipLinear##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magMinLinearMipNearest##AddressMode##Suffix(magMinLinearMipNearest##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magNearestMinMipLinear##AddressMode##Suffix(magNearestMinMipLinear##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magLinearMinNearestMipLinear##AddressMode##Suffix(magLinearMinNearestMipLinear##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magMinMipLinear##AddressMode##Suffix(magMinMipLinear##AddressMode, reductionMode);\
    constexpr MinMaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix(magMinLinearMipAnisotropic##AddressMode, reductionMode);

    namespace sampler
    {
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(Repeat, WeightedAverage, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(MirroredRepeat, WeightedAverage, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(ClampToEdge, WeightedAverage, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(ClampToBorder, WeightedAverage, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
    #ifdef VK_KHR_sampler_mirror_clamp_to_edge
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(MirrorClampToEdge, WeightedAverage, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
    #endif

        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(Repeat, Min, VK_SAMPLER_REDUCTION_MODE_MIN_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(MirroredRepeat, Min, VK_SAMPLER_REDUCTION_MODE_MIN_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(ClampToEdge, Min, VK_SAMPLER_REDUCTION_MODE_MIN_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(ClampToBorder, Min, VK_SAMPLER_REDUCTION_MODE_MIN_EXT)
    #ifdef VK_KHR_sampler_mirror_clamp_to_edge
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(MirrorClampToEdge, Min, VK_SAMPLER_REDUCTION_MODE_MIN_EXT)
    #endif

        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(Repeat, Max, VK_SAMPLER_REDUCTION_MODE_MAX_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(MirroredRepeat, Max, VK_SAMPLER_REDUCTION_MODE_MAX_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(ClampToEdge, Max, VK_SAMPLER_REDUCTION_MODE_MAX_EXT)
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(ClampToBorder, Max, VK_SAMPLER_REDUCTION_MODE_MAX_EXT)
    #ifdef VK_KHR_sampler_mirror_clamp_to_edge
        MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(MirrorClampToEdge, Max, VK_SAMPLER_REDUCTION_MODE_MAX_EXT)
    #endif
    } // namespace sampler
} // namespace magma
#endif // VK_EXT_sampler_filter_minmax
