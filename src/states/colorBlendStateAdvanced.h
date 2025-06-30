/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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

#ifdef VK_EXT_blend_operation_advanced
namespace magma
{
    /* This extension adds a number of "advanced" blending
       operations that can be used to perform new color
       blending operations, many of which are more complex than
       the standard blend modes provided by unextended Vulkan.
       https://www.khronos.org/registry/OpenGL/extensions/NV/NV_blend_equation_advanced.txt */

    struct AdvancedColorBlendState : ColorBlendState
    {
        explicit AdvancedColorBlendState(const ColorBlendAttachmentState& attachment,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap) noexcept;
        explicit AdvancedColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap) noexcept;
        AdvancedColorBlendState(const AdvancedColorBlendState&) noexcept;
        AdvancedColorBlendState(AdvancedColorBlendState&&) noexcept;
        hash_t hash() const noexcept;
        bool operator==(const AdvancedColorBlendState&) const noexcept;

        const VkPipelineColorBlendAdvancedStateCreateInfoEXT colorBlendAdvancedStateInfo;
        std::vector<ColorBlendAttachmentState> attachments;
    };
} // namespace magma

#include "colorBlendStateAdvanced.inl"
#endif // VK_EXT_blend_operation_advanced
