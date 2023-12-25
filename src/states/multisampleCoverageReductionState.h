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
#include "multisampleState.h"

#ifdef VK_NV_coverage_reduction_mode
namespace magma
{
    /* When using a framebuffer with mixed samples, a per-fragment coverage reduction operation
       is performed which generates color sample coverage from the pixel coverage.
       This extension defines the following modes to control how this reduction is performed:

        * Merge: When there are more samples in the pixel coverage than color samples, there is an
          implementation-dependent association of each pixel coverage sample to a color sample.
          In the merge mode, the color sample coverage is computed such that only if any associated sample
          in the pixel coverage is covered, the color sample is covered. This is the default mode.

        * Truncate: When there are more raster samples (N) than color samples(M), there is one to one
          association of the first M raster samples to the M color samples; other raster samples are ignored. */

    struct MultisampleCoverageReductionState : MultisampleState
    {
        constexpr MultisampleCoverageReductionState(const MultisampleState& state,
            VkCoverageReductionModeNV coverageReductionMode) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const MultisampleCoverageReductionState&) const noexcept;

        const VkPipelineCoverageReductionStateCreateInfoNV coverageReductionInfo;
    };
} // namespace magma

#include "multisampleCoverageReductionState.inl"

namespace magma
{
#define MAGMA_MULTISAMPLE_COVERAGE_REDUCTION_STATE_PERMUTATIONS(ReductionMode, coverageReductionMode)\
    constexpr MultisampleCoverageReductionState multisampleTwo##ReductionMode(multisampleTwo, coverageReductionMode);\
    constexpr MultisampleCoverageReductionState multisampleFour##ReductionMode(multisampleFour, coverageReductionMode);\
    constexpr MultisampleCoverageReductionState multisampleEight##ReductionMode(multisampleEight, coverageReductionMode);\
    constexpr MultisampleCoverageReductionState multisampleSixteen##ReductionMode(multisampleSixteen, coverageReductionMode);

    namespace renderstate
    {
        MAGMA_MULTISAMPLE_COVERAGE_REDUCTION_STATE_PERMUTATIONS(Merge, VK_COVERAGE_REDUCTION_MODE_MERGE_NV)
        MAGMA_MULTISAMPLE_COVERAGE_REDUCTION_STATE_PERMUTATIONS(Truncate, VK_COVERAGE_REDUCTION_MODE_TRUNCATE_NV)
    } // namespace renderstate
} // namespace magma
#endif // VK_NV_coverage_reduction_mode
