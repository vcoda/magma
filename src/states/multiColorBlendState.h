/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "colorBlendState.h"

namespace magma
{
    /* Multiple attachment color blend state takes care
       about array of blend attachment states and thereof
       is copyable, but not constexpr-constructible. */

    struct MultiColorBlendState final : ColorBlendState
    {
        explicit MultiColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            VkPipelineColorBlendStateCreateFlags flags = 0,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f}) noexcept;
        MultiColorBlendState(const ColorBlendState&) noexcept;
        MultiColorBlendState& operator=(const MultiColorBlendState&) noexcept;
        ~MultiColorBlendState();
        hash_t hash() const noexcept;
        bool operator==(const MultiColorBlendState&) const noexcept;
    };
} // namespace magma
