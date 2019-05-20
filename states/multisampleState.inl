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
#include "../internal/copy.h"
#include "../internal/hash.h"
#include "../internal/compareArrays.h"

namespace magma
{
constexpr MultisampleState::MultisampleState(VkSampleCountFlagBits rasterizationSamples) noexcept
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

constexpr MultisampleState::MultisampleState(uint32_t sampleCount) noexcept
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

constexpr size_t MultisampleState::hash() const noexcept
{
    size_t hash = internal::hashArgs(
        flags,
        rasterizationSamples,
        sampleShadingEnable,
        minSampleShading,
        alphaToCoverageEnable,
        alphaToOneEnable);
    if (pSampleMask)
    {
        std::hash<VkSampleMask> hasher;
        internal::hashCombine(hash, hasher(pSampleMask[0]));
        if (rasterizationSamples > VK_SAMPLE_COUNT_32_BIT)
            internal::hashCombine(hash, hasher(pSampleMask[1]));
    }
    return hash;
}

constexpr bool MultisampleState::operator==(const MultisampleState& other) const noexcept
{
    return (flags == other.flags) &&
        (rasterizationSamples == other.rasterizationSamples) &&
        (sampleShadingEnable == other.sampleShadingEnable) &&
        (minSampleShading == other.minSampleShading) &&
        (internal::compareArrays(pSampleMask, other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1)) &&
        (alphaToCoverageEnable == other.alphaToCoverageEnable) &&
        (alphaToOneEnable == other.alphaToOneEnable);
}

constexpr MultisampleShadingState::MultisampleShadingState(const MultisampleState& state,
    float minSampleShading /* 1 */) noexcept:
    MultisampleState(state.rasterizationSamples)
{
    sampleShadingEnable = VK_TRUE;
    this->minSampleShading = minSampleShading;
}
} // namespace magma
