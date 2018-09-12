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
            VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                                   VK_COLOR_COMPONENT_G_BIT |
                                                   VK_COLOR_COMPONENT_B_BIT |
                                                   VK_COLOR_COMPONENT_A_BIT) noexcept;
        bool operator==(const ColorBlendAttachmentState&) const noexcept;
    };

    /* Blending combines the incoming source fragment's R, G, B, and A values
       with the destination R, G, B, and A values of each sample stored in the framebuffer
       at the fragment's location. Blending is performed for each pixel sample,
       rather than just once for each fragment. */

    class ColorBlendState final : public VkPipelineColorBlendStateCreateInfo
    {
    public:
        ColorBlendState(const ColorBlendAttachmentState& attachment,
            bool logicOpEnable = false,
            VkLogicOp logicOp = VK_LOGIC_OP_CLEAR);
        ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            bool logicOpEnable = false,
            VkLogicOp logicOp = VK_LOGIC_OP_CLEAR);
        ColorBlendState(const ColorBlendState&);
        ~ColorBlendState();
        ColorBlendState& operator=(const ColorBlendState&);
        bool operator==(const ColorBlendState&) const noexcept;
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

    namespace states
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

        extern const ColorBlendState logicClearWriteR;
        extern const ColorBlendState logicAndWriteR;
        extern const ColorBlendState logicReverseAndWriteR;
        extern const ColorBlendState logicCopyWriteR;
        extern const ColorBlendState logicInvertedAndWriteR;
        extern const ColorBlendState logicNoOpWriteR;
        extern const ColorBlendState logicXorWriteR;
        extern const ColorBlendState logicOrWriteR;
        extern const ColorBlendState logicNorWriteR;
        extern const ColorBlendState logicEquivalentWriteR;
        extern const ColorBlendState logicInvertR;
        extern const ColorBlendState logicReverseOrWriteR;
        extern const ColorBlendState logicInvertedCopyWriteR;
        extern const ColorBlendState logicInvertedOrWriteR;
        extern const ColorBlendState logicNandWriteR;
        extern const ColorBlendState logicSetWriteR;

        extern const ColorBlendState logicClearWriteRG;
        extern const ColorBlendState logicAndWriteRG;
        extern const ColorBlendState logicReverseAndWriteRG;
        extern const ColorBlendState logicCopyWriteRG;
        extern const ColorBlendState logicInvertedAndWriteRG;
        extern const ColorBlendState logicNoOpWriteRG;
        extern const ColorBlendState logicXorWriteRG;
        extern const ColorBlendState logicOrWriteRG;
        extern const ColorBlendState logicNorWriteRG;
        extern const ColorBlendState logicEquivalentWriteRG;
        extern const ColorBlendState logicInvertR;
        extern const ColorBlendState logicReverseOrWriteRG;
        extern const ColorBlendState logicInvertedCopyWriteRG;
        extern const ColorBlendState logicInvertedOrWriteRG;
        extern const ColorBlendState logicNandWriteRG;
        extern const ColorBlendState logicSetWriteRG;

        extern const ColorBlendState logicClearWriteRGB;
        extern const ColorBlendState logicAndWriteRGB;
        extern const ColorBlendState logicReverseAndWriteRGB;
        extern const ColorBlendState logicCopyWriteRGB;
        extern const ColorBlendState logicInvertedAndWriteRGB;
        extern const ColorBlendState logicNoOpWriteRGB;
        extern const ColorBlendState logicXorWriteRGB;
        extern const ColorBlendState logicOrWriteRGB;
        extern const ColorBlendState logicNorWriteRGB;
        extern const ColorBlendState logicEquivalentWriteRGB;
        extern const ColorBlendState logicInvertR;
        extern const ColorBlendState logicReverseOrWriteRGB;
        extern const ColorBlendState logicInvertedCopyWriteRGB;
        extern const ColorBlendState logicInvertedOrWriteRGB;
        extern const ColorBlendState logicNandWriteRGB;
        extern const ColorBlendState logicSetWriteRGB;

        extern const ColorBlendState logicClearWriteRGBA;
        extern const ColorBlendState logicAndWriteRGBA;
        extern const ColorBlendState logicReverseAndWriteRGBA;
        extern const ColorBlendState logicCopyWriteRGBA;
        extern const ColorBlendState logicInvertedAndWriteRGBA;
        extern const ColorBlendState logicNoOpWriteRGBA;
        extern const ColorBlendState logicXorWriteRGBA;
        extern const ColorBlendState logicOrWriteRGBA;
        extern const ColorBlendState logicNorWriteRGBA;
        extern const ColorBlendState logicEquivalentWriteRGBA;
        extern const ColorBlendState logicInvertR;
        extern const ColorBlendState logicReverseOrWriteRGBA;
        extern const ColorBlendState logicInvertedCopyWriteRGBA;
        extern const ColorBlendState logicInvertedOrWriteRGBA;
        extern const ColorBlendState logicNandWriteRGBA;
        extern const ColorBlendState logicSetWriteRGBA;
    } // namespace states
} // namespace magma
