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
#include "../vulkan.h"

namespace magma
{
    /* Multisample state of graphics pipeline. */

    struct MultisampleState : VkPipelineMultisampleStateCreateInfo
    {
        MultisampleState(VkSampleCountFlagBits rasterizationSamples,
            bool sampleShading = false,
            bool alphaToCoverage = false,
            bool alphaToOne = false) noexcept;
        MultisampleState(uint32_t sampleCount,
            bool sampleShading = false,
            bool alphaToCoverage = false,
            bool alphaToOne = false) noexcept;
    };

    namespace states
    {
        extern const MultisampleState noMultisample;

        extern const MultisampleState multisample1;
        extern const MultisampleState multisample2;
        extern const MultisampleState multisample4;
        extern const MultisampleState multisample8;
        extern const MultisampleState multisample16;
        extern const MultisampleState multisample32;
        extern const MultisampleState multisample64;

        extern const MultisampleState multisample2SampleShading;
        extern const MultisampleState multisample4SampleShading;
        extern const MultisampleState multisample8SampleShading;
        extern const MultisampleState multisample16SampleShading;
        extern const MultisampleState multisample32SampleShading;
        extern const MultisampleState multisample64SampleShading;

        extern const MultisampleState multisample2AlphaToCoverage;
        extern const MultisampleState multisample4AlphaToCoverage;
        extern const MultisampleState multisample8AlphaToCoverage;
        extern const MultisampleState multisample16AlphaToCoverage;
        extern const MultisampleState multisample32AlphaToCoverage;
        extern const MultisampleState multisample64AlphaToCoverage;

        extern const MultisampleState multisample2AlphaToOne;
        extern const MultisampleState multisample4AlphaToOne;
        extern const MultisampleState multisample8AlphaToOne;
        extern const MultisampleState multisample16AlphaToOne;
        extern const MultisampleState multisample32AlphaToOne;
        extern const MultisampleState multisample64AlphaToOne;
    } // namespace states
} // namespace magma
