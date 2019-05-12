/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "../api/vulkan.h"

namespace magma
{
    /* Multisample state of graphics pipeline. */

    struct MultisampleState : VkPipelineMultisampleStateCreateInfo
    {
        constexpr MultisampleState(VkSampleCountFlagBits rasterizationSamples) noexcept;
        constexpr MultisampleState(uint32_t sampleCount) noexcept;
        size_t hash() const noexcept;
        bool operator==(const MultisampleState&) const noexcept;
    };

    /* Sample shading can be used to specify a minimum number of unique samples
       to process for each fragment. When the sample shading fraction is 1.0,
       a separate set of associated data are evaluated for each sample,
       and each set of values is evaluated at the sample location. */

    struct MultisampleShadingState : MultisampleState
    {
        constexpr MultisampleShadingState(const MultisampleState& state,
            float minSampleShading = 1.f) noexcept;
    };

    /* Sample mask contains a bitmask of static coverage information that is ANDed
       with the coverage information generated during rasterization. */

    struct MultisampleCoverageState : MultisampleState
    {
        MultisampleCoverageState(const MultisampleState& state,
            uint64_t coverageMask,
            bool alphaToCoverage = false,
            bool alphaToOne = false);
        MultisampleCoverageState(const MultisampleCoverageState&);
        MultisampleCoverageState& operator=(const MultisampleCoverageState&);
        ~MultisampleCoverageState();
    };
} // namespace magma

#include "multisampleState.inl"

namespace magma
{
    namespace renderstates
    {
        constexpr MultisampleState multisample1(VK_SAMPLE_COUNT_1_BIT);
        constexpr MultisampleState multisample2(VK_SAMPLE_COUNT_2_BIT);
        constexpr MultisampleState multisample4(VK_SAMPLE_COUNT_4_BIT);
        constexpr MultisampleState multisample8(VK_SAMPLE_COUNT_8_BIT);
        constexpr MultisampleState multisample16(VK_SAMPLE_COUNT_16_BIT);
        constexpr MultisampleState multisample32(VK_SAMPLE_COUNT_32_BIT);
        constexpr MultisampleState multisample64(VK_SAMPLE_COUNT_64_BIT);
        constexpr MultisampleState noMultisample(multisample1); // Alias name for convenience

        extern const MultisampleCoverageState multisample2AlphaToCoverage;
        extern const MultisampleCoverageState multisample4AlphaToCoverage;
        extern const MultisampleCoverageState multisample8AlphaToCoverage;
        extern const MultisampleCoverageState multisample16AlphaToCoverage;
        extern const MultisampleCoverageState multisample32AlphaToCoverage;
        extern const MultisampleCoverageState multisample64AlphaToCoverage;

        extern const MultisampleCoverageState multisample2AlphaToOne;
        extern const MultisampleCoverageState multisample4AlphaToOne;
        extern const MultisampleCoverageState multisample8AlphaToOne;
        extern const MultisampleCoverageState multisample16AlphaToOne;
        extern const MultisampleCoverageState multisample32AlphaToOne;
        extern const MultisampleCoverageState multisample64AlphaToOne;
    } // namespace renderstates
} // namespace magma
