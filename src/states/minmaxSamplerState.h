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
#include "samplerState.h"

#ifdef VK_EXT_sampler_filter_minmax
namespace magma
{
    /* Minmax sampler allows to produce a filtered texel value
       by computing a component-wise minimum or maximum of the
       texels that would normally be averaged. */

    struct MinmaxSamplerState : SamplerState
    {
        constexpr MinmaxSamplerState(const SamplerState& state,
            VkSamplerReductionModeEXT reductionMode) noexcept;
        constexpr MinmaxSamplerState(const MinmaxSamplerState&) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const MinmaxSamplerState&) const noexcept;

        const VkSamplerReductionModeCreateInfoEXT reductionModeInfo;
    };
}

#include "minmaxSamplerState.inl"

#define MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(AddressMode, ReductionMode, reductionMode)\
    constexpr MinmaxSamplerState magMinMipNearest##AddressMode##ReductionMode(magMinMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magLinearMinMipNearest##AddressMode##ReductionMode(magLinearMinMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magNearestMinLinearMipNearest##AddressMode##ReductionMode(magNearestMinLinearMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinNearestMipLinear##AddressMode##ReductionMode(magMinNearestMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipNearest##AddressMode##ReductionMode(magMinLinearMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magNearestMinMipLinear##AddressMode##ReductionMode(magNearestMinMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magLinearMinNearestMipLinear##AddressMode##ReductionMode(magLinearMinNearestMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinMipLinear##AddressMode##ReductionMode(magMinMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##1x(magMinLinearMipAnisotropic##AddressMode##1x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##2x(magMinLinearMipAnisotropic##AddressMode##2x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##3x(magMinLinearMipAnisotropic##AddressMode##3x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##4x(magMinLinearMipAnisotropic##AddressMode##4x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##5x(magMinLinearMipAnisotropic##AddressMode##5x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##6x(magMinLinearMipAnisotropic##AddressMode##6x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##7x(magMinLinearMipAnisotropic##AddressMode##7x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##8x(magMinLinearMipAnisotropic##AddressMode##8x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##9x(magMinLinearMipAnisotropic##AddressMode##9x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##10x(magMinLinearMipAnisotropic##AddressMode##10x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##11x(magMinLinearMipAnisotropic##AddressMode##11x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##12x(magMinLinearMipAnisotropic##AddressMode##12x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##13x(magMinLinearMipAnisotropic##AddressMode##13x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##14x(magMinLinearMipAnisotropic##AddressMode##14x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##15x(magMinLinearMipAnisotropic##AddressMode##15x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##16x(magMinLinearMipAnisotropic##AddressMode##16x, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##ReductionMode##Max(magMinLinearMipAnisotropic##AddressMode##Max, reductionMode);

namespace magma::sampler
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
} // namespace magma::sampler
#endif // VK_EXT_sampler_filter_minmax
