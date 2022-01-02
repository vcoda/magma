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
#include "multisampleState.h"

namespace magma
{
MultisampleCoverageState::MultisampleCoverageState(const MultisampleState& state,
    uint64_t coverageMask,
    bool alphaToCoverage /* false */,
    bool alphaToOne /* false */):
    MultisampleState(state.rasterizationSamples)
{   // The array is sized to a length of [rasterizationSamples/32] words
    VkSampleMask *sampleMask = new VkSampleMask[rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1];
    sampleMask[0] = coverageMask & 0xFFFFFFFFULL;
    if (rasterizationSamples > VK_SAMPLE_COUNT_32_BIT)
        sampleMask[1] = (coverageMask >> VK_SAMPLE_COUNT_32_BIT) & 0xFFFFFFFFULL;
    pSampleMask = sampleMask;
    alphaToCoverageEnable = MAGMA_BOOLEAN(alphaToCoverage);
    alphaToOneEnable = MAGMA_BOOLEAN(alphaToOne);
}

MultisampleCoverageState::MultisampleCoverageState(const MultisampleCoverageState& other) noexcept:
    MultisampleState(other.rasterizationSamples)
{
    pSampleMask = core::copyArray(other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1);
    alphaToCoverageEnable = other.alphaToCoverageEnable;
    alphaToOneEnable = other.alphaToOneEnable;
}

MultisampleCoverageState& MultisampleCoverageState::operator=(const MultisampleCoverageState& other) noexcept
{
    if (this != &other)
    {
        delete[] pSampleMask;
        core::copy(this, &other);
        if (other.pSampleMask)
            pSampleMask = core::copyArray(other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1);
    }
    return *this;
}

MultisampleCoverageState::~MultisampleCoverageState()
{
    delete[] pSampleMask;
}

namespace renderstate
{
static constexpr uint64_t coverageMask = 0xFFFFFFFFFFFFFFFFULL;

const MultisampleCoverageState alphaToCoverageMultisample2(VK_SAMPLE_COUNT_2_BIT, coverageMask, true, false);
const MultisampleCoverageState alphaToCoverageMultisample4(VK_SAMPLE_COUNT_4_BIT, coverageMask, true, false);
const MultisampleCoverageState alphaToCoverageMultisample8(VK_SAMPLE_COUNT_8_BIT, coverageMask, true, false);
const MultisampleCoverageState alphaToCoverageMultisample16(VK_SAMPLE_COUNT_16_BIT, coverageMask, true, false);
const MultisampleCoverageState alphaToCoverageMultisample32(VK_SAMPLE_COUNT_32_BIT, coverageMask, true, false);
const MultisampleCoverageState alphaToCoverageMultisample64(VK_SAMPLE_COUNT_64_BIT, coverageMask, false);

const MultisampleCoverageState alphaToOneMultisample2(VK_SAMPLE_COUNT_2_BIT, coverageMask, false, true);
const MultisampleCoverageState alphaToOneMultisample4(VK_SAMPLE_COUNT_4_BIT, coverageMask, false, true);
const MultisampleCoverageState alphaToOneMultisample8(VK_SAMPLE_COUNT_8_BIT, coverageMask, false, true);
const MultisampleCoverageState alphaToOneMultisample16(VK_SAMPLE_COUNT_16_BIT, coverageMask, false, true);
const MultisampleCoverageState alphaToOneMultisample32(VK_SAMPLE_COUNT_32_BIT, coverageMask, false, true);
const MultisampleCoverageState alphaToOneMultisample64(VK_SAMPLE_COUNT_64_BIT, coverageMask, false, true);
} // namespace renderstate
} // namespace magma
