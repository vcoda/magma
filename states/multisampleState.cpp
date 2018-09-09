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
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
MultisampleState::MultisampleState(VkSampleCountFlagBits rasterizationSamples,
    bool sampleShading /* false */, bool alphaToCoverage /* false */, bool alphaToOne /* false */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->rasterizationSamples = rasterizationSamples;
    sampleShadingEnable = MAGMA_BOOLEAN(sampleShading);
    minSampleShading = 0;
    pSampleMask = nullptr;
    alphaToCoverageEnable = MAGMA_BOOLEAN(alphaToCoverage);
    alphaToOneEnable = MAGMA_BOOLEAN(alphaToOne);
}

MultisampleState::MultisampleState(uint32_t sampleCount,
    bool sampleShading /* false */, bool alphaToCoverage /* false */, bool alphaToOne /* false */) noexcept
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
    sampleShadingEnable = MAGMA_BOOLEAN(sampleShading);
    minSampleShading = 0;
    pSampleMask = nullptr;
    alphaToCoverageEnable = MAGMA_BOOLEAN(alphaToCoverage);
    alphaToOneEnable = MAGMA_BOOLEAN(alphaToOne);
}

namespace states
{
const MultisampleState noMultisample(VK_SAMPLE_COUNT_1_BIT); // Just another name for convenience

const MultisampleState multisample1(VK_SAMPLE_COUNT_1_BIT);
const MultisampleState multisample2(VK_SAMPLE_COUNT_2_BIT);
const MultisampleState multisample4(VK_SAMPLE_COUNT_4_BIT);
const MultisampleState multisample8(VK_SAMPLE_COUNT_8_BIT);
const MultisampleState multisample16(VK_SAMPLE_COUNT_16_BIT);
const MultisampleState multisample32(VK_SAMPLE_COUNT_32_BIT);
const MultisampleState multisample64(VK_SAMPLE_COUNT_64_BIT);

const MultisampleState multisample2SampleShading(VK_SAMPLE_COUNT_2_BIT, true);
const MultisampleState multisample4SampleShading(VK_SAMPLE_COUNT_4_BIT, true);
const MultisampleState multisample8SampleShading(VK_SAMPLE_COUNT_8_BIT, true);
const MultisampleState multisample16SampleShading(VK_SAMPLE_COUNT_16_BIT, true);
const MultisampleState multisample32SampleShading(VK_SAMPLE_COUNT_32_BIT, true);
const MultisampleState multisample64SampleShading(VK_SAMPLE_COUNT_64_BIT, true);

const MultisampleState multisample2AlphaToCoverage(VK_SAMPLE_COUNT_2_BIT, false, true);
const MultisampleState multisample4AlphaToCoverage(VK_SAMPLE_COUNT_4_BIT, false, true);
const MultisampleState multisample8AlphaToCoverage(VK_SAMPLE_COUNT_8_BIT, false, true);
const MultisampleState multisample16AlphaToCoverage(VK_SAMPLE_COUNT_16_BIT, false, true);
const MultisampleState multisample32AlphaToCoverage(VK_SAMPLE_COUNT_32_BIT, false, true);
const MultisampleState multisample64AlphaToCoverage(VK_SAMPLE_COUNT_64_BIT, false, true);

const MultisampleState multisample2AlphaToOne(VK_SAMPLE_COUNT_2_BIT, false, false, true);
const MultisampleState multisample4AlphaToOne(VK_SAMPLE_COUNT_4_BIT, false, false, true);
const MultisampleState multisample8AlphaToOne(VK_SAMPLE_COUNT_8_BIT, false, false, true);
const MultisampleState multisample16AlphaToOne(VK_SAMPLE_COUNT_16_BIT, false, false, true);
const MultisampleState multisample32AlphaToOne(VK_SAMPLE_COUNT_32_BIT, false, false, true);
const MultisampleState multisample64AlphaToOne(VK_SAMPLE_COUNT_64_BIT, false, false, true);
} // namespace states
} // namespace magma
