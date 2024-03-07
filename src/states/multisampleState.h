/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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

namespace magma
{
    /* Multisampling is a mechanism to antialias all Vulkan primitives:
       points, lines, and polygons. The technique is to sample all
       primitives multiple times at each pixel. */

    struct MultisampleState : VkPipelineMultisampleStateCreateInfo
    {
        constexpr MultisampleState(VkSampleCountFlagBits rasterizationSamples) noexcept;
        constexpr MultisampleState(uint32_t sampleCount) noexcept;
        constexpr MultisampleState(const MultisampleState&) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const MultisampleState&) const noexcept;
        friend std::ostream& operator<<(std::ostream&, const MultisampleState&);
    };

    /* Sample shading can be used to specify a minimum number of
       unique samples to process for each fragment. When the sample
       shading fraction is 1.0, a separate set of associated data
       are evaluated for each sample, and each set of values is
       evaluated at the sample location. */

    struct SampleShadingState : MultisampleState
    {
        constexpr SampleShadingState(uint32_t sampleCount,
            float minSampleShading = 1.f) noexcept;
        constexpr SampleShadingState(const MultisampleState& state,
            float minSampleShading = 1.f) noexcept;
    };

    /* Sample mask contains a bitmask of static coverage information
       that is ANDed with the coverage information generated during rasterization. */

    struct MultisampleCoverageState final : MultisampleState
    {
        explicit MultisampleCoverageState(const MultisampleState& state,
            uint64_t coverageMask,
            bool alphaToCoverage = false,
            bool alphaToOne = false) noexcept;
        MultisampleCoverageState(const MultisampleCoverageState&) noexcept;
        MultisampleCoverageState& operator=(const MultisampleCoverageState&) noexcept;
        ~MultisampleCoverageState();
    };
} // namespace magma

#include "multisampleState.inl"

namespace magma
{
    namespace renderstate
    {
        constexpr MultisampleState dontMultisample(VK_SAMPLE_COUNT_1_BIT);
        constexpr MultisampleState multisampleTwo(VK_SAMPLE_COUNT_2_BIT);
        constexpr MultisampleState multisampleFour(VK_SAMPLE_COUNT_4_BIT);
        constexpr MultisampleState multisampleEight(VK_SAMPLE_COUNT_8_BIT);
        constexpr MultisampleState multisampleSixteen(VK_SAMPLE_COUNT_16_BIT);
        constexpr MultisampleState multisampleThirtyTwo(VK_SAMPLE_COUNT_32_BIT);
        constexpr MultisampleState multisampleSixtyFour(VK_SAMPLE_COUNT_64_BIT);

        extern const MultisampleCoverageState alphaToCoverageMultisampleTwo;
        extern const MultisampleCoverageState alphaToCoverageMultisampleFour;
        extern const MultisampleCoverageState alphaToCoverageMultisampleEight;
        extern const MultisampleCoverageState alphaToCoverageMultisampleSixteen;
        extern const MultisampleCoverageState alphaToCoverageMultisampleThirtyTwo;
        extern const MultisampleCoverageState alphaToCoverageMultisampleSixtyFour;

        extern const MultisampleCoverageState alphaToOneMultisampleTwo;
        extern const MultisampleCoverageState alphaToOneMultisampleFour;
        extern const MultisampleCoverageState alphaToOneMultisampleEight;
        extern const MultisampleCoverageState alphaToOneMultisampleSixteen;
        extern const MultisampleCoverageState alphaToOneMultisampleThirtyTwo;
        extern const MultisampleCoverageState alphaToOneMultisampleSixtyFour;
    } // namespace renderstate
} // namespace magma
