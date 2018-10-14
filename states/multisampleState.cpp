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
#include "multisampleState.h"
#include "../helpers/copy.h"
#include "../helpers/hash.h"
#include "../shared.h"

namespace magma
{
MultisampleState::MultisampleState(VkSampleCountFlagBits rasterizationSamples) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->rasterizationSamples = rasterizationSamples;
    sampleShadingEnable = VK_FALSE;
    minSampleShading = 0.f;
    pSampleMask = nullptr;
    alphaToCoverageEnable = VK_FALSE;
    alphaToOneEnable = VK_FALSE;
}

MultisampleState::MultisampleState(uint32_t sampleCount) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    switch (sampleCount)
    {
    case 1: rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: rasterizationSamples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: rasterizationSamples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: rasterizationSamples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: rasterizationSamples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: rasterizationSamples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: rasterizationSamples = VK_SAMPLE_COUNT_64_BIT; break;
    default:
        rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }
    sampleShadingEnable = VK_FALSE;
    minSampleShading = 0.f;
    pSampleMask = nullptr;
    alphaToCoverageEnable = VK_FALSE;
    alphaToOneEnable = VK_FALSE;
}

MultisampleState::MultisampleState(const MultisampleState& other)
{
    helpers::copy(this, &other);
    if (other.pSampleMask)
        pSampleMask = helpers::copyArray(other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1);
}

MultisampleState& MultisampleState::operator=(const MultisampleState& other)
{
    if (this != &other)
    {
        helpers::copy(this, &other);
        if (other.pSampleMask)
            pSampleMask = helpers::copyArray(other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1);
    }
    return *this;
}

MultisampleState::~MultisampleState()
{
    delete[] pSampleMask;
}

size_t MultisampleState::hash() const noexcept
{
    size_t hash = helpers::hashVariadic(
        flags,
        rasterizationSamples,
        sampleShadingEnable,
        minSampleShading,
        alphaToCoverageEnable,
        alphaToOneEnable);
    if (pSampleMask)
    {
        helpers::hashCombineArg(hash, pSampleMask[0]);
        if (rasterizationSamples > VK_SAMPLE_COUNT_32_BIT)
            helpers::hashCombineArg(hash, pSampleMask[1]);
    }
    return hash;
}

bool MultisampleState::operator==(const MultisampleState& other) const noexcept
{
    return (flags == other.flags) &&
        (rasterizationSamples == other.rasterizationSamples) &&
        (sampleShadingEnable == other.sampleShadingEnable) &&
        (minSampleShading == other.minSampleShading) &&
        (helpers::compareArrays(pSampleMask, other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1)) &&
        (alphaToCoverageEnable == other.alphaToCoverageEnable) &&
        (alphaToOneEnable == other.alphaToOneEnable);
}

MultisampleShadingState::MultisampleShadingState(const MultisampleState& state,
    float minSampleShading /* 1 */) noexcept:
    MultisampleState(state.rasterizationSamples)
{
    sampleShadingEnable = VK_TRUE;
    this->minSampleShading = minSampleShading;
}

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

namespace states
{
const MultisampleState multisample1(VK_SAMPLE_COUNT_1_BIT);
const MultisampleState multisample2(VK_SAMPLE_COUNT_2_BIT);
const MultisampleState multisample4(VK_SAMPLE_COUNT_4_BIT);
const MultisampleState multisample8(VK_SAMPLE_COUNT_8_BIT);
const MultisampleState multisample16(VK_SAMPLE_COUNT_16_BIT);
const MultisampleState multisample32(VK_SAMPLE_COUNT_32_BIT);
const MultisampleState multisample64(VK_SAMPLE_COUNT_64_BIT);
const MultisampleState noMultisample(multisample1); // Alias name for convenience

static constexpr uint64_t coverageMask = 0xFFFFFFFFFFFFFFFFULL;

const MultisampleCoverageState multisample2AlphaToCoverage(VK_SAMPLE_COUNT_2_BIT, coverageMask, true, false);
const MultisampleCoverageState multisample4AlphaToCoverage(VK_SAMPLE_COUNT_4_BIT, coverageMask, true, false);
const MultisampleCoverageState multisample8AlphaToCoverage(VK_SAMPLE_COUNT_8_BIT, coverageMask, true, false);
const MultisampleCoverageState multisample16AlphaToCoverage(VK_SAMPLE_COUNT_16_BIT, coverageMask, true, false);
const MultisampleCoverageState multisample32AlphaToCoverage(VK_SAMPLE_COUNT_32_BIT, coverageMask, true, false);
const MultisampleCoverageState multisample64AlphaToCoverage(VK_SAMPLE_COUNT_64_BIT, coverageMask, false);

const MultisampleCoverageState multisample2AlphaToOne(VK_SAMPLE_COUNT_2_BIT, coverageMask, false, true);
const MultisampleCoverageState multisample4AlphaToOne(VK_SAMPLE_COUNT_4_BIT, coverageMask, false, true);
const MultisampleCoverageState multisample8AlphaToOne(VK_SAMPLE_COUNT_8_BIT, coverageMask, false, true);
const MultisampleCoverageState multisample16AlphaToOne(VK_SAMPLE_COUNT_16_BIT, coverageMask, false, true);
const MultisampleCoverageState multisample32AlphaToOne(VK_SAMPLE_COUNT_32_BIT, coverageMask, false, true);
const MultisampleCoverageState multisample64AlphaToOne(VK_SAMPLE_COUNT_64_BIT, coverageMask, false, true);
} // namespace states
} // namespace magma
