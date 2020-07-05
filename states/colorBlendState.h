/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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

    struct ColorBlendState : VkPipelineColorBlendStateCreateInfo, core::NonCopyableConstexpr
    {
        constexpr ColorBlendState(const ColorBlendAttachmentState& attachment,
            bool logicOpEnable = false,
            VkLogicOp logicOp = VK_LOGIC_OP_CLEAR,
            const std::initializer_list<float>& blendConstants = {1.f, 1.f, 1.f, 1.f}) noexcept;
        std::size_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendState&) const noexcept;

    protected:
        constexpr ColorBlendState() noexcept;
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
    namespace blendstates
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
    } // namespace blendstates

    namespace renderstates
    {
        constexpr ColorBlendState dontWriteRgba(blendstates::writeNone);
        constexpr ColorBlendState dontBlendR(blendstates::writeR);
        constexpr ColorBlendState dontBlendRg(blendstates::writeRg);
        constexpr ColorBlendState dontBlendRgb(blendstates::writeRgb);
        constexpr ColorBlendState dontBlendRgba(blendstates::writeRgba);

        constexpr ColorBlendState blendAddR(blendstates::addR);
        constexpr ColorBlendState blendSubtractR(blendstates::subtractR);
        constexpr ColorBlendState blendReverseSubtractR(blendstates::reverseSubtractR);
        constexpr ColorBlendState blendMinR(blendstates::minR);
        constexpr ColorBlendState blendMaxR(blendstates::maxR);
        constexpr ColorBlendState blendMultiplyR(blendstates::multiplyR);
        constexpr ColorBlendState blendNormalR(blendstates::normalR);
        constexpr ColorBlendState blendNormalPremultipliedR(blendstates::normalPremultipliedR);
        constexpr ColorBlendState blendScreenR(blendstates::screenR);

        constexpr ColorBlendState blendAddRg(blendstates::addRg);
        constexpr ColorBlendState blendSubtractRg(blendstates::subtractRg);
        constexpr ColorBlendState blendReverseSubtractRg(blendstates::reverseSubtractRg);
        constexpr ColorBlendState blendMinRg(blendstates::minRg);
        constexpr ColorBlendState blendMaxRg(blendstates::maxRg);
        constexpr ColorBlendState blendMultiplyRg(blendstates::multiplyRg);
        constexpr ColorBlendState blendNormalRg(blendstates::normalRg);
        constexpr ColorBlendState blendNormalPremultipliedRg(blendstates::normalPremultipliedRg);
        constexpr ColorBlendState blendScreenRg(blendstates::screenRg);

        constexpr ColorBlendState blendAddRgb(blendstates::addRgb);
        constexpr ColorBlendState blendSubtractRgb(blendstates::subtractRgb);
        constexpr ColorBlendState blendReverseSubtractRgb(blendstates::reverseSubtractRgb);
        constexpr ColorBlendState blendMinRgb(blendstates::minRgb);
        constexpr ColorBlendState blendMaxRgb(blendstates::maxRgb);
        constexpr ColorBlendState blendMultiplyRgb(blendstates::multiplyRgb);
        constexpr ColorBlendState blendNormalRgb(blendstates::normalRgb);
        constexpr ColorBlendState blendNormalPremultipliedRgb(blendstates::normalPremultipliedRgb);
        constexpr ColorBlendState blendScreenRgb(blendstates::screenRgb);

        constexpr ColorBlendState blendAddRgba(blendstates::addRgba);
        constexpr ColorBlendState blendSubtractRgba(blendstates::subtractRgba);
        constexpr ColorBlendState blendReverseSubtractRgba(blendstates::reverseSubtractRgba);
        constexpr ColorBlendState blendMinRgba(blendstates::minRgba);
        constexpr ColorBlendState blendMaxRgba(blendstates::maxRgba);
        constexpr ColorBlendState blendMultiplyRgba(blendstates::multiplyRgba);
        constexpr ColorBlendState blendNormalRgba(blendstates::normalRgba);
        constexpr ColorBlendState blendNormalPremultipliedRgba(blendstates::normalPremultipliedRgba);
        constexpr ColorBlendState blendScreenRgba(blendstates::screenRgba);

        constexpr ColorLogicOpState logicClearR(blendstates::writeR, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndR(blendstates::writeR, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseR(blendstates::writeR, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyR(blendstates::writeR, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedR(blendstates::writeR, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpR(blendstates::writeR, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorR(blendstates::writeR, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrR(blendstates::writeR, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorR(blendstates::writeR, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentR(blendstates::writeR, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertR(blendstates::writeR, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseR(blendstates::writeR, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedR(blendstates::writeR, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedR(blendstates::writeR, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandR(blendstates::writeR, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetR(blendstates::writeR, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearRg(blendstates::writeRg, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndRg(blendstates::writeRg, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseRg(blendstates::writeRg, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyRg(blendstates::writeRg, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedRg(blendstates::writeRg, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpRg(blendstates::writeRg, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorRg(blendstates::writeRg, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrRg(blendstates::writeRg, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorRg(blendstates::writeRg, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentRg(blendstates::writeRg, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRg(blendstates::writeRg, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseRg(blendstates::writeRg, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedRg(blendstates::writeRg, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRg(blendstates::writeRg, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandRg(blendstates::writeRg, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetRg(blendstates::writeRg, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearRgb(blendstates::writeRgb, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndRgb(blendstates::writeRgb, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseRgb(blendstates::writeRgb, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyRgb(blendstates::writeRgb, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedRgb(blendstates::writeRgb, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpRgb(blendstates::writeRgb, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorRgb(blendstates::writeRgb, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrRgb(blendstates::writeRgb, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorRgb(blendstates::writeRgb, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentRgb(blendstates::writeRgb, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRgb(blendstates::writeRgb, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseRgb(blendstates::writeRgb, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedRgb(blendstates::writeRgb, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRgb(blendstates::writeRgb, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandRgb(blendstates::writeRgb, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetRgb(blendstates::writeRgb, VK_LOGIC_OP_SET);

        constexpr ColorLogicOpState logicClearRgba(blendstates::writeRgba, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicAndRgba(blendstates::writeRgba, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndReverseRgba(blendstates::writeRgba, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicCopyRgba(blendstates::writeRgba, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicAndInvertedRgba(blendstates::writeRgba, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicNoOpRgba(blendstates::writeRgba, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicXorRgba(blendstates::writeRgba, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicOrRgba(blendstates::writeRgba, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicNorRgba(blendstates::writeRgba, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicEquivalentRgba(blendstates::writeRgba, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicInvertRgba(blendstates::writeRgba, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicOrReverseRgba(blendstates::writeRgba, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicCopyInvertedRgba(blendstates::writeRgba, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRgba(blendstates::writeRgba, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicNandRgba(blendstates::writeRgba, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicSetRgba(blendstates::writeRgba, VK_LOGIC_OP_SET);
    } // namespace renderstates
} // namespace magma
