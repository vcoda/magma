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

#define MAGMA_MINMAX_SAMPLER_STATE_PERMUTATIONS(AddressMode, Suffix, reductionMode)\
    constexpr MinmaxSamplerState magMinMipNearest##AddressMode##Suffix(magMinMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magLinearMinMipNearest##AddressMode##Suffix(magLinearMinMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magNearestMinLinearMipNearest##AddressMode##Suffix(magNearestMinLinearMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinNearestMipLinear##AddressMode##Suffix(magMinNearestMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipNearest##AddressMode##Suffix(magMinLinearMipNearest##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magNearestMinMipLinear##AddressMode##Suffix(magNearestMinMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magLinearMinNearestMipLinear##AddressMode##Suffix(magLinearMinNearestMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinMipLinear##AddressMode##Suffix(magMinMipLinear##AddressMode, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X1(magMinLinearMipAnisotropic##AddressMode##X1, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X2(magMinLinearMipAnisotropic##AddressMode##X2, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X3(magMinLinearMipAnisotropic##AddressMode##X3, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X4(magMinLinearMipAnisotropic##AddressMode##X4, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X5(magMinLinearMipAnisotropic##AddressMode##X5, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X6(magMinLinearMipAnisotropic##AddressMode##X6, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X7(magMinLinearMipAnisotropic##AddressMode##X7, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X8(magMinLinearMipAnisotropic##AddressMode##X8, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X9(magMinLinearMipAnisotropic##AddressMode##X9, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X10(magMinLinearMipAnisotropic##AddressMode##X10, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X11(magMinLinearMipAnisotropic##AddressMode##X11, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X12(magMinLinearMipAnisotropic##AddressMode##X12, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X13(magMinLinearMipAnisotropic##AddressMode##X13, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X14(magMinLinearMipAnisotropic##AddressMode##X14, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X15(magMinLinearMipAnisotropic##AddressMode##X15, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##X16(magMinLinearMipAnisotropic##AddressMode##X16, reductionMode);\
    constexpr MinmaxSamplerState magMinLinearMipAnisotropic##AddressMode##Suffix##Max(magMinLinearMipAnisotropic##AddressMode##Max, reductionMode);

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
