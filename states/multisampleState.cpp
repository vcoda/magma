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
#include <limits>
#include "multisampleState.h"
#include "../helpers/copy.h"
#include "../helpers/hash.h"
#include "../shared.h"

namespace magma
{
MultisampleState::MultisampleState(VkSampleCountFlagBits rasterizationSamples,
    bool alphaToCoverage /* false */,
    bool alphaToOne /* false */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->rasterizationSamples = rasterizationSamples;
    sampleShadingEnable = VK_FALSE;
    minSampleShading = 0.f;
    pSampleMask = nullptr;
    alphaToCoverageEnable = MAGMA_BOOLEAN(alphaToCoverage);
    alphaToOneEnable = MAGMA_BOOLEAN(alphaToOne);
}

MultisampleState::MultisampleState(uint32_t sampleCount,
    bool alphaToCoverage /* false */,
    bool alphaToOne /* false */) noexcept
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
    alphaToCoverageEnable = MAGMA_BOOLEAN(alphaToCoverage);
    alphaToOneEnable = MAGMA_BOOLEAN(alphaToOne);
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
    MultisampleState(state.rasterizationSamples, state.alphaToCoverageEnable ? true : false, state.alphaToOneEnable ? true : false)
{
    sampleShadingEnable = VK_TRUE;
    this->minSampleShading = minSampleShading;
}

MultisampleCoverageState::MultisampleCoverageState(const MultisampleState& state, uint64_t coverageMask):
    MultisampleState(state.rasterizationSamples, state.alphaToCoverageEnable ? true : false, state.alphaToOneEnable ? true : false)
{
    // The array is sized to a length of [rasterizationSamples/32] words
    VkSampleMask *sampleMask = new VkSampleMask[rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1];
    sampleMask[0] = coverageMask & std::numeric_limits<uint32_t>::max();
    if (rasterizationSamples > VK_SAMPLE_COUNT_32_BIT)
        sampleMask[1] = (coverageMask >> VK_SAMPLE_COUNT_32_BIT) & std::numeric_limits<uint32_t>::max();
    pSampleMask = sampleMask;
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

const MultisampleState multisample2AlphaToCoverage(VK_SAMPLE_COUNT_2_BIT, true, false);
const MultisampleState multisample4AlphaToCoverage(VK_SAMPLE_COUNT_4_BIT, true, false);
const MultisampleState multisample8AlphaToCoverage(VK_SAMPLE_COUNT_8_BIT, true, false);
const MultisampleState multisample16AlphaToCoverage(VK_SAMPLE_COUNT_16_BIT, true, false);
const MultisampleState multisample32AlphaToCoverage(VK_SAMPLE_COUNT_32_BIT, true, false);
const MultisampleState multisample64AlphaToCoverage(VK_SAMPLE_COUNT_64_BIT, true, false);

const MultisampleState multisample2AlphaToOne(VK_SAMPLE_COUNT_2_BIT, false, true);
const MultisampleState multisample4AlphaToOne(VK_SAMPLE_COUNT_4_BIT, false, true);
const MultisampleState multisample8AlphaToOne(VK_SAMPLE_COUNT_8_BIT, false, true);
const MultisampleState multisample16AlphaToOne(VK_SAMPLE_COUNT_16_BIT, false, true);
const MultisampleState multisample32AlphaToOne(VK_SAMPLE_COUNT_32_BIT, false, true);
const MultisampleState multisample64AlphaToOne(VK_SAMPLE_COUNT_64_BIT, false, true);

const MultisampleState noMultisample(multisample1); // Alias name for convenience
} // namespace states
} // namespace magma
