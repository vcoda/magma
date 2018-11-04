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
#include <vector>
#include "../vulkan.h"

namespace magma
{
    /* Per-target blending state for each individual color attachment. */

    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        ColorBlendAttachmentState(bool blendEnable,
            VkBlendFactor srcBlendFactor,
            VkBlendFactor dstBlendFactor,
            VkBlendOp blendOp,
            VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT) noexcept;
        size_t hash() const noexcept;
        bool operator==(const ColorBlendAttachmentState&) const noexcept;
    };

    /* Blending combines the incoming source fragment's R, G, B, and A values
       with the destination R, G, B, and A values of each sample stored in the framebuffer
       at the fragment's location. Blending is performed for each pixel sample,
       rather than just once for each fragment. */

    struct ColorBlendState : VkPipelineColorBlendStateCreateInfo
    {
        ColorBlendState(const ColorBlendAttachmentState& attachment,
            const std::initializer_list<float>& blendConstants = {});
        ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            const std::initializer_list<float>& blendConstants = {});
        ColorBlendState(const ColorBlendState&);
        ColorBlendState& operator=(const ColorBlendState&);
        ~ColorBlendState();
        size_t hash() const noexcept;
        bool operator==(const ColorBlendState&) const noexcept;
    };

    /* If logicOpEnable is VK_TRUE, then a logical operation selected by logicOp is applied
       between each color attachment and the fragment’s corresponding output value,
       and blending of all attachments is treated as if it were disabled. */

    struct ColorLogicOpState : ColorBlendState
    {
        ColorLogicOpState(const ColorBlendAttachmentState& attachment,
            VkLogicOp logicOp);
        ColorLogicOpState(const std::vector<ColorBlendAttachmentState>& attachments,
            VkLogicOp logicOp);
    };

    namespace blendstates
    {
        extern const VkColorComponentFlags colorWriteMaskR;
        extern const VkColorComponentFlags colorWriteMaskRG;
        extern const VkColorComponentFlags colorWriteMaskRGB;
        extern const VkColorComponentFlags colorWriteMaskRGBA;

        extern const ColorBlendAttachmentState passR;
        extern const ColorBlendAttachmentState passRG;
        extern const ColorBlendAttachmentState passRGB;
        extern const ColorBlendAttachmentState passRGBA;

        extern const ColorBlendAttachmentState addR;
        extern const ColorBlendAttachmentState addRG;
        extern const ColorBlendAttachmentState addRGB;
        extern const ColorBlendAttachmentState addRGBA;

        extern const ColorBlendAttachmentState subtractR;
        extern const ColorBlendAttachmentState subtractRG;
        extern const ColorBlendAttachmentState subtractRGB;
        extern const ColorBlendAttachmentState subtractRGBA;

        extern const ColorBlendAttachmentState reverseSubtractR;
        extern const ColorBlendAttachmentState reverseSubtractRG;
        extern const ColorBlendAttachmentState reverseSubtractRGB;
        extern const ColorBlendAttachmentState reverseSubtractRGBA;

        extern const ColorBlendAttachmentState minR;
        extern const ColorBlendAttachmentState minRG;
        extern const ColorBlendAttachmentState minRGB;
        extern const ColorBlendAttachmentState minRGBA;

        extern const ColorBlendAttachmentState maxR;
        extern const ColorBlendAttachmentState maxRG;
        extern const ColorBlendAttachmentState maxRGB;
        extern const ColorBlendAttachmentState maxRGBA;

        extern const ColorBlendAttachmentState normalR;
        extern const ColorBlendAttachmentState normalRG;
        extern const ColorBlendAttachmentState normalRGB;
        extern const ColorBlendAttachmentState normalRGBA;
    } // namespace blendstates

    namespace renderstates
    {
        extern const ColorBlendState dontBlendWriteR;
        extern const ColorBlendState dontBlendWriteRG;
        extern const ColorBlendState dontBlendWriteRGB;
        extern const ColorBlendState dontBlendWriteRGBA;

        extern const ColorBlendState blendAddWriteR;
        extern const ColorBlendState blendAddWriteRG;
        extern const ColorBlendState blendAddWriteRGB;
        extern const ColorBlendState blendAddWriteRGBA;

        extern const ColorBlendState blendSubtractWriteR;
        extern const ColorBlendState blendSubtractWriteRG;
        extern const ColorBlendState blendSubractWriteRGB;
        extern const ColorBlendState blendSubractWriteRGBA;

        extern const ColorBlendState blendReverseSubtractWriteR;
        extern const ColorBlendState blendReverseSubtractWriteRG;
        extern const ColorBlendState blendReverseSubractWriteRGB;
        extern const ColorBlendState blendReverseSubractWriteRGBA;

        extern const ColorBlendState blendMinWriteR;
        extern const ColorBlendState blendMinWriteRG;
        extern const ColorBlendState blendMinWriteRGB;
        extern const ColorBlendState blendMinWriteRGBA;

        extern const ColorBlendState blendMaxWriteR;
        extern const ColorBlendState blendMaxWriteRG;
        extern const ColorBlendState blendMaxWriteRGB;
        extern const ColorBlendState blendMaxWriteRGBA;

        extern const ColorBlendState blendNormalWriteR;
        extern const ColorBlendState blendNormalWriteRG;
        extern const ColorBlendState blendNormalWriteRGB;
        extern const ColorBlendState blendNormalWriteRGBA;

        extern const ColorLogicOpState logicClearWriteR;
        extern const ColorLogicOpState logicAndWriteR;
        extern const ColorLogicOpState logicReverseAndWriteR;
        extern const ColorLogicOpState logicCopyWriteR;
        extern const ColorLogicOpState logicInvertedAndWriteR;
        extern const ColorLogicOpState logicNoOpWriteR;
        extern const ColorLogicOpState logicXorWriteR;
        extern const ColorLogicOpState logicOrWriteR;
        extern const ColorLogicOpState logicNorWriteR;
        extern const ColorLogicOpState logicEquivalentWriteR;
        extern const ColorLogicOpState logicInvertR;
        extern const ColorLogicOpState logicReverseOrWriteR;
        extern const ColorLogicOpState logicInvertedCopyWriteR;
        extern const ColorLogicOpState logicInvertedOrWriteR;
        extern const ColorLogicOpState logicNandWriteR;
        extern const ColorLogicOpState logicSetWriteR;

        extern const ColorLogicOpState logicClearWriteRG;
        extern const ColorLogicOpState logicAndWriteRG;
        extern const ColorLogicOpState logicReverseAndWriteRG;
        extern const ColorLogicOpState logicCopyWriteRG;
        extern const ColorLogicOpState logicInvertedAndWriteRG;
        extern const ColorLogicOpState logicNoOpWriteRG;
        extern const ColorLogicOpState logicXorWriteRG;
        extern const ColorLogicOpState logicOrWriteRG;
        extern const ColorLogicOpState logicNorWriteRG;
        extern const ColorLogicOpState logicEquivalentWriteRG;
        extern const ColorLogicOpState logicInvertR;
        extern const ColorLogicOpState logicReverseOrWriteRG;
        extern const ColorLogicOpState logicInvertedCopyWriteRG;
        extern const ColorLogicOpState logicInvertedOrWriteRG;
        extern const ColorLogicOpState logicNandWriteRG;
        extern const ColorLogicOpState logicSetWriteRG;

        extern const ColorLogicOpState logicClearWriteRGB;
        extern const ColorLogicOpState logicAndWriteRGB;
        extern const ColorLogicOpState logicReverseAndWriteRGB;
        extern const ColorLogicOpState logicCopyWriteRGB;
        extern const ColorLogicOpState logicInvertedAndWriteRGB;
        extern const ColorLogicOpState logicNoOpWriteRGB;
        extern const ColorLogicOpState logicXorWriteRGB;
        extern const ColorLogicOpState logicOrWriteRGB;
        extern const ColorLogicOpState logicNorWriteRGB;
        extern const ColorLogicOpState logicEquivalentWriteRGB;
        extern const ColorLogicOpState logicInvertR;
        extern const ColorLogicOpState logicReverseOrWriteRGB;
        extern const ColorLogicOpState logicInvertedCopyWriteRGB;
        extern const ColorLogicOpState logicInvertedOrWriteRGB;
        extern const ColorLogicOpState logicNandWriteRGB;
        extern const ColorLogicOpState logicSetWriteRGB;

        extern const ColorLogicOpState logicClearWriteRGBA;
        extern const ColorLogicOpState logicAndWriteRGBA;
        extern const ColorLogicOpState logicReverseAndWriteRGBA;
        extern const ColorLogicOpState logicCopyWriteRGBA;
        extern const ColorLogicOpState logicInvertedAndWriteRGBA;
        extern const ColorLogicOpState logicNoOpWriteRGBA;
        extern const ColorLogicOpState logicXorWriteRGBA;
        extern const ColorLogicOpState logicOrWriteRGBA;
        extern const ColorLogicOpState logicNorWriteRGBA;
        extern const ColorLogicOpState logicEquivalentWriteRGBA;
        extern const ColorLogicOpState logicInvertR;
        extern const ColorLogicOpState logicReverseOrWriteRGBA;
        extern const ColorLogicOpState logicInvertedCopyWriteRGBA;
        extern const ColorLogicOpState logicInvertedOrWriteRGBA;
        extern const ColorLogicOpState logicNandWriteRGBA;
        extern const ColorLogicOpState logicSetWriteRGBA;
    } // namespace renderstates
} // namespace magma
