/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../core/noncopyableConstexpr.h"
#include "colorWriteMask.h"

namespace magma
{
    /* Per-target blending state for each individual color attachment. */

    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        constexpr ColorBlendAttachmentState(VkColorComponentFlags colorWriteMask = colorwritemask::rgba) noexcept;
        constexpr ColorBlendAttachmentState(VkBlendFactor srcBlendFactor,
            VkBlendFactor dstBlendFactor,
            VkBlendOp blendOp,
            VkColorComponentFlags colorWriteMask = colorwritemask::rgba) noexcept;
        constexpr ColorBlendAttachmentState(VkBlendFactor srcColorBlendFactor,
            VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp,
            VkBlendFactor srcAlphaBlendFactor,
            VkBlendFactor dstAlphaBlendFactor,
            VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask = colorwritemask::rgba) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendAttachmentState&) const noexcept;
    };

    /* Blending combines the incoming source fragment's R, G, B, and A values
       with the destination R, G, B, and A values of each sample stored in the framebuffer
       at the fragment's location. Blending is performed for each pixel sample,
       rather than just once for each fragment. */

    struct ColorBlendState : VkPipelineColorBlendStateCreateInfo
    {
        constexpr ColorBlendState() noexcept;
        constexpr ColorBlendState(const ColorBlendAttachmentState& attachment,
            bool logicOpEnable = false,
            VkLogicOp logicOp = VK_LOGIC_OP_CLEAR,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f}) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendState&) const noexcept;
    };

    /* Multiple attachment color blend state takes care about array of blend attachment states and
       thereof is copyable, but not constexpr-constructible. */

    struct MultiColorBlendState final : ColorBlendState
    {
        explicit MultiColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f}) noexcept;
        MultiColorBlendState(const ColorBlendState&) noexcept;
        MultiColorBlendState(const MultiColorBlendState&) noexcept;
        MultiColorBlendState& operator=(const MultiColorBlendState&) noexcept;
        ~MultiColorBlendState();
        std::size_t hash() const noexcept;
        bool operator==(const MultiColorBlendState&) const noexcept;
    };

    /* If logicOpEnable is VK_TRUE, then a logical operation selected by logicOp is applied
       between each color attachment and the fragment’s corresponding output value,
       and blending of all attachments is treated as if it were disabled. */

    struct ColorLogicOpState : ColorBlendState
    {
        constexpr ColorLogicOpState(const ColorBlendAttachmentState& attachment,
            VkLogicOp logicOp) noexcept;
    };
} // namespace magma

#include "colorBlendState.inl"

namespace magma
{
    namespace blendstate
    {
        constexpr ColorBlendAttachmentState writeNone(colorwritemask::none);
        constexpr ColorBlendAttachmentState writeR(colorwritemask::r);
        constexpr ColorBlendAttachmentState writeG(colorwritemask::g);
        constexpr ColorBlendAttachmentState writeB(colorwritemask::b);
        constexpr ColorBlendAttachmentState writeA(colorwritemask::a);
        constexpr ColorBlendAttachmentState writeRg(colorwritemask::rg);
        constexpr ColorBlendAttachmentState writeRb(colorwritemask::rb);
        constexpr ColorBlendAttachmentState writeRa(colorwritemask::ra);
        constexpr ColorBlendAttachmentState writeGb(colorwritemask::gb);
        constexpr ColorBlendAttachmentState writeGa(colorwritemask::ga);
        constexpr ColorBlendAttachmentState writeBa(colorwritemask::ba);
        constexpr ColorBlendAttachmentState writeRgb(colorwritemask::rgb);
        constexpr ColorBlendAttachmentState writeRga(colorwritemask::rga);
        constexpr ColorBlendAttachmentState writeRba(colorwritemask::rba);
        constexpr ColorBlendAttachmentState writeGba(colorwritemask::gba);
        constexpr ColorBlendAttachmentState writeRgba(colorwritemask::rgba);

        constexpr ColorBlendAttachmentState addR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::r);
        constexpr ColorBlendAttachmentState addRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::rg);
        constexpr ColorBlendAttachmentState addRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState addRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState subtractR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorwritemask::r);
        constexpr ColorBlendAttachmentState subtractRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorwritemask::rg);
        constexpr ColorBlendAttachmentState subtractRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState subtractRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState reverseSubtractR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorwritemask::r);
        constexpr ColorBlendAttachmentState reverseSubtractRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorwritemask::rg);
        constexpr ColorBlendAttachmentState reverseSubtractRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState reverseSubtractRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState minR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorwritemask::r);
        constexpr ColorBlendAttachmentState minRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorwritemask::rg);
        constexpr ColorBlendAttachmentState minRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState minRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState maxR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorwritemask::r);
        constexpr ColorBlendAttachmentState maxRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorwritemask::rg);
        constexpr ColorBlendAttachmentState maxRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState maxRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState multiplyR(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorwritemask::r);
        constexpr ColorBlendAttachmentState multiplyRg(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorwritemask::rg);
        constexpr ColorBlendAttachmentState multiplyRgb(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState multiplyRgba(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState normalR(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::r);
        constexpr ColorBlendAttachmentState normalRg(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::rg);
        constexpr ColorBlendAttachmentState normalRgb(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState normalRgba(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::rgba);

        constexpr ColorBlendAttachmentState normalPremultipliedR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::r);
        constexpr ColorBlendAttachmentState normalPremultipliedRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::rg);
        constexpr ColorBlendAttachmentState normalPremultipliedRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState normalPremultipliedRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorwritemask::rgba);

        // 1 - (1 - a) (1 - b) = a * (1 - b) + b = a + (1 - a) * b
        constexpr ColorBlendAttachmentState screenR(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::r);
        constexpr ColorBlendAttachmentState screenRg(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::rg);
        constexpr ColorBlendAttachmentState screenRgb(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::rgb);
        constexpr ColorBlendAttachmentState screenRgba(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorwritemask::rgba);
    } // namespace blendstate

    namespace renderstate
    {
        constexpr ColorBlendState dontWriteRgba(blendstate::writeNone);
        constexpr ColorBlendState dontBlendR(blendstate::writeR);
        constexpr ColorBlendState dontBlendRg(blendstate::writeRg);
        constexpr ColorBlendState dontBlendRgb(blendstate::writeRgb);
        constexpr ColorBlendState dontBlendRgba(blendstate::writeRgba);

        constexpr ColorBlendState blendAddR(blendstate::addR);
        constexpr ColorBlendState blendSubtractR(blendstate::subtractR);
        constexpr ColorBlendState blendReverseSubtractR(blendstate::reverseSubtractR);
        constexpr ColorBlendState blendMinR(blendstate::minR);
        constexpr ColorBlendState blendMaxR(blendstate::maxR);
        constexpr ColorBlendState blendMultiplyR(blendstate::multiplyR);
        constexpr ColorBlendState blendNormalR(blendstate::normalR);
        constexpr ColorBlendState blendNormalPremultipliedR(blendstate::normalPremultipliedR);
        constexpr ColorBlendState blendScreenR(blendstate::screenR);

        constexpr ColorBlendState blendAddRg(blendstate::addRg);
        constexpr ColorBlendState blendSubtractRg(blendstate::subtractRg);
        constexpr ColorBlendState blendReverseSubtractRg(blendstate::reverseSubtractRg);
        constexpr ColorBlendState blendMinRg(blendstate::minRg);
        constexpr ColorBlendState blendMaxRg(blendstate::maxRg);
        constexpr ColorBlendState blendMultiplyRg(blendstate::multiplyRg);
        constexpr ColorBlendState blendNormalRg(blendstate::normalRg);
        constexpr ColorBlendState blendNormalPremultipliedRg(blendstate::normalPremultipliedRg);
        constexpr ColorBlendState blendScreenRg(blendstate::screenRg);

        constexpr ColorBlendState blendAddRgb(blendstate::addRgb);
        constexpr ColorBlendState blendSubtractRgb(blendstate::subtractRgb);
        constexpr ColorBlendState blendReverseSubtractRgb(blendstate::reverseSubtractRgb);
        constexpr ColorBlendState blendMinRgb(blendstate::minRgb);
        constexpr ColorBlendState blendMaxRgb(blendstate::maxRgb);
        constexpr ColorBlendState blendMultiplyRgb(blendstate::multiplyRgb);
        constexpr ColorBlendState blendNormalRgb(blendstate::normalRgb);
        constexpr ColorBlendState blendNormalPremultipliedRgb(blendstate::normalPremultipliedRgb);
        constexpr ColorBlendState blendScreenRgb(blendstate::screenRgb);

        constexpr ColorBlendState blendAddRgba(blendstate::addRgba);
        constexpr ColorBlendState blendSubtractRgba(blendstate::subtractRgba);
        constexpr ColorBlendState blendReverseSubtractRgba(blendstate::reverseSubtractRgba);
        constexpr ColorBlendState blendMinRgba(blendstate::minRgba);
        constexpr ColorBlendState blendMaxRgba(blendstate::maxRgba);
        constexpr ColorBlendState blendMultiplyRgba(blendstate::multiplyRgba);
        constexpr ColorBlendState blendNormalRgba(blendstate::normalRgba);
        constexpr ColorBlendState blendNormalPremultipliedRgba(blendstate::normalPremultipliedRgba);
        constexpr ColorBlendState blendScreenRgba(blendstate::screenRgba);

        constexpr ColorLogicOpState logicClearR(blendstate::writeR, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndR(blendstate::writeR, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseR(blendstate::writeR, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyR(blendstate::writeR, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedR(blendstate::writeR, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpR(blendstate::writeR, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorR(blendstate::writeR, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrR(blendstate::writeR, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorR(blendstate::writeR, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentR(blendstate::writeR, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertR(blendstate::writeR, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseR(blendstate::writeR, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedR(blendstate::writeR, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedR(blendstate::writeR, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandR(blendstate::writeR, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetR(blendstate::writeR, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearRg(blendstate::writeRg, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndRg(blendstate::writeRg, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseRg(blendstate::writeRg, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyRg(blendstate::writeRg, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedRg(blendstate::writeRg, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpRg(blendstate::writeRg, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorRg(blendstate::writeRg, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrRg(blendstate::writeRg, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorRg(blendstate::writeRg, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentRg(blendstate::writeRg, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRg(blendstate::writeRg, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseRg(blendstate::writeRg, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedRg(blendstate::writeRg, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRg(blendstate::writeRg, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandRg(blendstate::writeRg, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetRg(blendstate::writeRg, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearRgb(blendstate::writeRgb, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndRgb(blendstate::writeRgb, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseRgb(blendstate::writeRgb, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyRgb(blendstate::writeRgb, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedRgb(blendstate::writeRgb, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpRgb(blendstate::writeRgb, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorRgb(blendstate::writeRgb, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrRgb(blendstate::writeRgb, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorRgb(blendstate::writeRgb, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentRgb(blendstate::writeRgb, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRgb(blendstate::writeRgb, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseRgb(blendstate::writeRgb, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedRgb(blendstate::writeRgb, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRgb(blendstate::writeRgb, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandRgb(blendstate::writeRgb, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetRgb(blendstate::writeRgb, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearRgba(blendstate::writeRgba, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndRgba(blendstate::writeRgba, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseRgba(blendstate::writeRgba, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyRgba(blendstate::writeRgba, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedRgba(blendstate::writeRgba, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpRgba(blendstate::writeRgba, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorRgba(blendstate::writeRgba, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrRgba(blendstate::writeRgba, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorRgba(blendstate::writeRgba, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentRgba(blendstate::writeRgba, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRgba(blendstate::writeRgba, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseRgba(blendstate::writeRgba, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedRgba(blendstate::writeRgba, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRgba(blendstate::writeRgba, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandRgba(blendstate::writeRgba, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetRgba(blendstate::writeRgba, VK_LOGIC_OP_SET);
    } // namespace renderstate
} // namespace magma
