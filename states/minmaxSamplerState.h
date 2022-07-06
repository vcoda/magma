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
    /* Minmax sampler allows to produce a filtered texel value by computing
       a component-wise minimum or maximum of the texels that would normally be averaged. */

    struct MinMaxSamplerState : SamplerState
    {
        constexpr MinMaxSamplerState(const SamplerState& state,
            VkSamplerReductionModeEXT reductionMode) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const MinMaxSamplerState&) const noexcept;

        const VkSamplerReductionModeCreateInfoEXT reduction;
    };
}

#include "minmaxSamplerState.inl"

namespace magma
{
    namespace sampler
    {
        constexpr MinMaxSamplerState magMinMipNearestRepeatWeightedAverage(magMinMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestRepeatWeightedAverage(magLinearMinMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestRepeatWeightedAverage(magNearestMinLinearMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearRepeatWeightedAverage(magMinNearestMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestRepeatWeightedAverage(magMinLinearMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearRepeatWeightedAverage(magNearestMinMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearRepeatWeightedAverage(magLinearMinNearestMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinMipLinearRepeatWeightedAverage(magMinMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicRepeatWeightedAverage(magMinLinearMipAnisotropicRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);

        constexpr MinMaxSamplerState magMinMipNearestMirroredRepeatWeightedAverage(magMinMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestMirroredRepeatWeightedAverage(magLinearMinMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestMirroredRepeatWeightedAverage(magNearestMinLinearMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearMirroredRepeatWeightedAverage(magMinNearestMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestMirroredRepeatWeightedAverage(magMinLinearMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearMirroredRepeatWeightedAverage(magNearestMinMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearMirroredRepeatWeightedAverage(magLinearMinNearestMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinMipLinearMirroredRepeatWeightedAverage(magMinMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicMirroredRepeatWeightedAverage(magMinLinearMipAnisotropicMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);

        constexpr MinMaxSamplerState magMinMipNearestClampToEdgeWeightedAverage(magMinMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestClampToEdgeWeightedAverage(magLinearMinMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestClampToEdgeWeightedAverage(magNearestMinLinearMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearClampToEdgeWeightedAverage(magMinNearestMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestClampToEdgeWeightedAverage(magMinLinearMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearClampToEdgeWeightedAverage(magNearestMinMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearClampToEdgeWeightedAverage(magLinearMinNearestMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinMipLinearClampToEdgeWeightedAverage(magMinMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicClampToEdgeWeightedAverage(magMinLinearMipAnisotropicClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);

        constexpr MinMaxSamplerState magMinMipNearestClampToBorderWeightedAverage(magMinMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestClampToBorderWeightedAverage(magLinearMinMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestClampToBorderWeightedAverage(magNearestMinLinearMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearClampToBorderWeightedAverage(magMinNearestMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestClampToBorderWeightedAverage(magMinLinearMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearClampToBorderWeightedAverage(magNearestMinMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearClampToBorderWeightedAverage(magLinearMinNearestMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinMipLinearClampToBorderWeightedAverage(magMinMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicClampToBorderWeightedAverage(magMinLinearMipAnisotropicClampToBorder, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);

        constexpr MinMaxSamplerState magMinMipNearestRepeatMin(magMinMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestRepeatMin(magLinearMinMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestRepeatMin(magNearestMinLinearMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearRepeatMin(magMinNearestMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestRepeatMin(magMinLinearMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearRepeatMin(magNearestMinMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearRepeatMin(magLinearMinNearestMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinMipLinearRepeatMin(magMinMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicRepeatMin(magMinLinearMipAnisotropicRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);

        constexpr MinMaxSamplerState magMinMipNearestMirroredRepeatMin(magMinMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestMirroredRepeatMin(magLinearMinMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestMirroredRepeatMin(magNearestMinLinearMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearMirroredRepeatMin(magMinNearestMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestMirroredRepeatMin(magMinLinearMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearMirroredRepeatMin(magNearestMinMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearMirroredRepeatMin(magLinearMinNearestMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinMipLinearMirroredRepeatMin(magMinMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicMirroredRepeatMin(magMinLinearMipAnisotropicMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);

        constexpr MinMaxSamplerState magMinMipNearestClampToEdgeMin(magMinMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestClampToEdgeMin(magLinearMinMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestClampToEdgeMin(magNearestMinLinearMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearClampToEdgeMin(magMinNearestMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestClampToEdgeMin(magMinLinearMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearClampToEdgeMin(magNearestMinMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearClampToEdgeMin(magLinearMinNearestMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinMipLinearClampToEdgeMin(magMinMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicClampToEdgeMin(magMinLinearMipAnisotropicClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);

        constexpr MinMaxSamplerState magMinMipNearestClampToBorderMin(magMinMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestClampToBorderMin(magLinearMinMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestClampToBorderMin(magNearestMinLinearMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearClampToBorderMin(magMinNearestMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestClampToBorderMin(magMinLinearMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearClampToBorderMin(magNearestMinMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearClampToBorderMin(magLinearMinNearestMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinMipLinearClampToBorderMin(magMinMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicClampToBorderMin(magMinLinearMipAnisotropicClampToBorder, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);

        constexpr MinMaxSamplerState magMinMipNearestRepeatMax(magMinMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestRepeatMax(magLinearMinMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestRepeatMax(magNearestMinLinearMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearRepeatMax(magMinNearestMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestRepeatMax(magMinLinearMipNearestRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearRepeatMax(magNearestMinMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearRepeatMax(magLinearMinNearestMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinMipLinearRepeatMax(magMinMipLinearRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicRepeatMax(magMinLinearMipAnisotropicRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);

        constexpr MinMaxSamplerState magMinMipNearestMirroredRepeatMax(magMinMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestMirroredRepeatMax(magLinearMinMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestMirroredRepeatMax(magNearestMinLinearMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearMirroredRepeatMax(magMinNearestMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestMirroredRepeatMax(magMinLinearMipNearestMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearMirroredRepeatMax(magNearestMinMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearMirroredRepeatMax(magLinearMinNearestMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinMipLinearMirroredRepeatMax(magMinMipLinearMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicMirroredRepeatMax(magMinLinearMipAnisotropicMirroredRepeat, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);

        constexpr MinMaxSamplerState magMinMipNearestClampToEdgeMax(magMinMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestClampToEdgeMax(magLinearMinMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestClampToEdgeMax(magNearestMinLinearMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearClampToEdgeMax(magMinNearestMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestClampToEdgeMax(magMinLinearMipNearestClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearClampToEdgeMax(magNearestMinMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearClampToEdgeMax(magLinearMinNearestMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinMipLinearClampToEdgeMax(magMinMipLinearClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicClampToEdgeMax(magMinLinearMipAnisotropicClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);

        constexpr MinMaxSamplerState magMinMipNearestClampToBorderMax(magMinMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestClampToBorderMax(magLinearMinMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestClampToBorderMax(magNearestMinLinearMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearClampToBorderMax(magMinNearestMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestClampToBorderMax(magMinLinearMipNearestClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearClampToBorderMax(magNearestMinMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearClampToBorderMax(magLinearMinNearestMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinMipLinearClampToBorderMax(magMinMipLinearClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicClampToBorderMax(magMinLinearMipAnisotropicClampToBorder, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);

#   ifdef VK_KHR_sampler_mirror_clamp_to_edge
        constexpr MinMaxSamplerState magMinMipNearestMirrorClampToEdgeWeightedAverage(magMinMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestMirrorClampToEdgeWeightedAverage(magLinearMinMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestMirrorClampToEdgeWeightedAverage(magNearestMinLinearMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearMirrorClampToEdgeWeightedAverage(magMinNearestMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestMirrorClampToEdgeWeightedAverage(magMinLinearMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearMirrorClampToEdgeWeightedAverage(magNearestMinMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearMirrorClampToEdgeWeightedAverage(magLinearMinNearestMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinMipLinearMirrorClampToEdgeWeightedAverage(magMinMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicMirrorClampToEdgeWeightedAverage(magMinLinearMipAnisotropicMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT);

        constexpr MinMaxSamplerState magMinMipNearestMirrorClampToEdgeMin(magMinMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestMirrorClampToEdgeMin(magLinearMinMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestMirrorClampToEdgeMin(magNearestMinLinearMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearMirrorClampToEdgeMin(magMinNearestMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestMirrorClampToEdgeMin(magMinLinearMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearMirrorClampToEdgeMin(magNearestMinMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearMirrorClampToEdgeMin(magLinearMinNearestMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinMipLinearMirrorClampToEdgeMin(magMinMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicMirrorClampToEdgeMin(magMinLinearMipAnisotropicMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MIN_EXT);

        constexpr MinMaxSamplerState magMinMipNearestMirrorClampToEdgeMax(magMinMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinMipNearestMirrorClampToEdgeMax(magLinearMinMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinLinearMipNearestMirrorClampToEdgeMax(magNearestMinLinearMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinNearestMipLinearMirrorClampToEdgeMax(magMinNearestMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipNearestMirrorClampToEdgeMax(magMinLinearMipNearestMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magNearestMinMipLinearMirrorClampToEdgeMax(magNearestMinMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magLinearMinNearestMipLinearMirrorClampToEdgeMax(magLinearMinNearestMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinMipLinearMirrorClampToEdgeMax(magMinMipLinearMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
        constexpr MinMaxSamplerState magMinLinearMipAnisotropicMirrorClampToEdgeMax(magMinLinearMipAnisotropicMirrorClampToEdge, VK_SAMPLER_REDUCTION_MODE_MAX_EXT);
#   endif // VK_KHR_sampler_mirror_clamp_to_edge
    } // namespace sampler
} // namespace magma
#endif // VK_EXT_sampler_filter_minmax
