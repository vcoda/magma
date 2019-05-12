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
#include "../api/vulkan.h"

namespace magma
{
    /* Per-target blending state for each individual color attachment. */

    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        constexpr ColorBlendAttachmentState(bool blendEnable,
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
        constexpr ColorBlendState(const ColorBlendAttachmentState& attachment,
            bool logicOpEnable = false,
            VkLogicOp logicOp = VK_LOGIC_OP_CLEAR,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f});
        size_t hash() const noexcept;
        bool operator==(const ColorBlendState&) const noexcept;
    };

    struct MultiColorBlendState : VkPipelineColorBlendStateCreateInfo
    {
        MultiColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            const std::initializer_list<float>& blendConstants = {});
        MultiColorBlendState(const MultiColorBlendState&);
        MultiColorBlendState& operator=(const MultiColorBlendState&);
        ~MultiColorBlendState();
        size_t hash() const noexcept;
        bool operator==(const MultiColorBlendState&) const noexcept;
    };

    /* If logicOpEnable is VK_TRUE, then a logical operation selected by logicOp is applied
       between each color attachment and the fragment’s corresponding output value,
       and blending of all attachments is treated as if it were disabled. */

    struct ColorLogicOpState : ColorBlendState
    {
        constexpr ColorLogicOpState(const ColorBlendAttachmentState& attachment,
            VkLogicOp logicOp);
    };

    struct MultiColorLogicOpState : MultiColorBlendState
    {
        MultiColorLogicOpState(const std::vector<ColorBlendAttachmentState>& attachments,
            VkLogicOp logicOp);
    };
} // namespace magma

#include "colorBlendState.inl"

namespace magma
{
    namespace blendstates
    {
        constexpr VkColorComponentFlags colorWriteMaskR(VK_COLOR_COMPONENT_R_BIT);
        constexpr VkColorComponentFlags colorWriteMaskRG(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
        constexpr VkColorComponentFlags colorWriteMaskRGB(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
        constexpr VkColorComponentFlags colorWriteMaskRGBA(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

        constexpr ColorBlendAttachmentState writeR(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskR);
        constexpr ColorBlendAttachmentState writeRG(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState writeRGB(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState writeRGBA(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskRGBA);

        constexpr ColorBlendAttachmentState addR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskR);
        constexpr ColorBlendAttachmentState addRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState addRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState addRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskRGBA);

        constexpr ColorBlendAttachmentState subtractR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskR);
        constexpr ColorBlendAttachmentState subtractRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState subtractRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState subtractRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskRGBA);

        constexpr ColorBlendAttachmentState reverseSubtractR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskR);
        constexpr ColorBlendAttachmentState reverseSubtractRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState reverseSubtractRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState reverseSubtractRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskRGBA);

        constexpr ColorBlendAttachmentState minR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskR);
        constexpr ColorBlendAttachmentState minRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState minRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState minRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskRGBA);

        constexpr ColorBlendAttachmentState maxR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskR);
        constexpr ColorBlendAttachmentState maxRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState maxRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState maxRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskRGBA);

        constexpr ColorBlendAttachmentState normalR(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskR);
        constexpr ColorBlendAttachmentState normalRG(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskRG);
        constexpr ColorBlendAttachmentState normalRGB(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskRGB);
        constexpr ColorBlendAttachmentState normalRGBA(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskRGBA);
    } // namespace blendstates

    namespace renderstates
    {
        constexpr ColorBlendState dontBlendWriteR(blendstates::writeR);
        constexpr ColorBlendState dontBlendWriteRG(blendstates::writeRG);
        constexpr ColorBlendState dontBlendWriteRGB(blendstates::writeRGB);
        constexpr ColorBlendState dontBlendWriteRGBA(blendstates::writeRGBA);

        constexpr ColorBlendState blendAddWriteR(blendstates::addR);
        constexpr ColorBlendState blendAddWriteRG(blendstates::addRG);
        constexpr ColorBlendState blendAddWriteRGB(blendstates::addRGB);
        constexpr ColorBlendState blendAddWriteRGBA(blendstates::addRGBA);

        constexpr ColorBlendState blendSubtractWriteR(blendstates::subtractR);
        constexpr ColorBlendState blendSubtractWriteRG(blendstates::subtractRG);
        constexpr ColorBlendState blendSubtractWriteRGB(blendstates::subtractRGB);
        constexpr ColorBlendState blendSubtractWriteRGBA(blendstates::subtractRGBA);

        constexpr ColorBlendState blendReverseSubtractWriteR(blendstates::reverseSubtractR);
        constexpr ColorBlendState blendReverseSubtractWriteRG(blendstates::reverseSubtractRG);
        constexpr ColorBlendState blendReverseSubtractWriteRGB(blendstates::reverseSubtractRGB);
        constexpr ColorBlendState blendReverseSubtractWriteRGBA(blendstates::reverseSubtractRGBA);

        constexpr ColorBlendState blendMinWriteR(blendstates::minR);
        constexpr ColorBlendState blendMinWriteRG(blendstates::minRG);
        constexpr ColorBlendState blendMinWriteRGB(blendstates::minRGB);
        constexpr ColorBlendState blendMinWriteRGBA(blendstates::minRGBA);

        constexpr ColorBlendState blendMaxWriteR(blendstates::maxRG);
        constexpr ColorBlendState blendMaxWriteRG(blendstates::maxRG);
        constexpr ColorBlendState blendMaxWriteRGB(blendstates::maxRGB);
        constexpr ColorBlendState blendMaxWriteRGBA(blendstates::maxRGBA);

        constexpr ColorBlendState blendNormalWriteR(blendstates::normalR);
        constexpr ColorBlendState blendNormalWriteRG(blendstates::normalRG);
        constexpr ColorBlendState blendNormalWriteRGB(blendstates::normalRGB);
        constexpr ColorBlendState blendNormalWriteRGBA(blendstates::normalRGBA);

        constexpr ColorLogicOpState logicClearWriteR(blendstates::writeR, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndWriteR(blendstates::writeR, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicReverseAndWriteR(blendstates::writeR, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyWriteR(blendstates::writeR, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicInvertedAndWriteR(blendstates::writeR, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpWriteR(blendstates::writeR, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorWriteR(blendstates::writeR, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrWriteR(blendstates::writeR, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorWriteR(blendstates::writeR, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentWriteR(blendstates::writeR, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertR(blendstates::writeR, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicReverseOrWriteR(blendstates::writeR, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicInvertedCopyWriteR(blendstates::writeR, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicInvertedOrWriteR(blendstates::writeR, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandWriteR(blendstates::writeR, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetWriteR(blendstates::writeR, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearWriteRG(blendstates::writeRG, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndWriteRG(blendstates::writeRG, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicReverseAndWriteRG(blendstates::writeRG, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyWriteRG(blendstates::writeRG, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicInvertedAndWriteRG(blendstates::writeRG, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpWriteRG(blendstates::writeRG, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorWriteRG(blendstates::writeRG, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrWriteRG(blendstates::writeRG, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorWriteRG(blendstates::writeRG, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentWriteRG(blendstates::writeRG, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRG(blendstates::writeRG, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicReverseOrWriteRG(blendstates::writeRG, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicInvertedCopyWriteRG(blendstates::writeRG, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicInvertedOrWriteRG(blendstates::writeRG, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandWriteRG(blendstates::writeRG, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetWriteRG(blendstates::writeRG, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicReverseAndWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicInvertedAndWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRGB(blendstates::writeRGB, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicReverseOrWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicInvertedCopyWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicInvertedOrWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetWriteRGB(blendstates::writeRGB, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicReverseAndWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicInvertedAndWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRGBA(blendstates::writeRGBA, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicReverseOrWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicInvertedCopyWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicInvertedOrWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetWriteRGBA(blendstates::writeRGBA, VK_LOGIC_OP_SET);
    } // namespace renderstates
} // namespace magma
