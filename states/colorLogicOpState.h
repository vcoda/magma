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
#include "colorBlendState.h"

namespace magma
{
    /* If logicOpEnable is VK_TRUE, then a logical operation selected by logicOp is applied
       between each color attachment and the fragment's corresponding output value,
       and blending of all attachments is treated as if it were disabled. */

    struct ColorLogicOpState : ColorBlendState
    {
        constexpr ColorLogicOpState(const ColorBlendAttachmentState& attachment, VkLogicOp logicOp) noexcept:
            ColorBlendState(attachment, true, logicOp) {}
    };
} // namespace magma

namespace magma
{
    namespace renderstate
    {
        constexpr ColorLogicOpState logicClearR(blendstate::writeR, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearG(blendstate::writeG, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearB(blendstate::writeB, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearA(blendstate::writeA, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRg(blendstate::writeRg, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRb(blendstate::writeRb, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRa(blendstate::writeRa, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearGb(blendstate::writeGb, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearGa(blendstate::writeGa, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearBa(blendstate::writeBa, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRgb(blendstate::writeRgb, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRga(blendstate::writeRga, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRba(blendstate::writeRba, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearGba(blendstate::writeGba, VK_LOGIC_OP_CLEAR);
        constexpr ColorLogicOpState logicClearRgba(blendstate::writeRgba, VK_LOGIC_OP_CLEAR);

        constexpr ColorLogicOpState logicAndR(blendstate::writeR, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndG(blendstate::writeG, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndB(blendstate::writeB, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndA(blendstate::writeA, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRg(blendstate::writeRg, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRb(blendstate::writeRb, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRa(blendstate::writeRa, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndGb(blendstate::writeGb, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndGa(blendstate::writeGa, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndBa(blendstate::writeBa, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRgb(blendstate::writeRgb, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRga(blendstate::writeRga, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRba(blendstate::writeRba, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndGba(blendstate::writeGba, VK_LOGIC_OP_AND);
        constexpr ColorLogicOpState logicAndRgba(blendstate::writeRgba, VK_LOGIC_OP_AND);

        constexpr ColorLogicOpState logicAndReverseR(blendstate::writeR, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseG(blendstate::writeG, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseB(blendstate::writeB, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseA(blendstate::writeA, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRg(blendstate::writeRg, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRb(blendstate::writeRb, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRa(blendstate::writeRa, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseGb(blendstate::writeGb, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseGa(blendstate::writeGa, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseBa(blendstate::writeBa, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRgb(blendstate::writeRgb, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRga(blendstate::writeRga, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRba(blendstate::writeRba, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseGba(blendstate::writeGba, VK_LOGIC_OP_AND_REVERSE);
        constexpr ColorLogicOpState logicAndReverseRgba(blendstate::writeRgba, VK_LOGIC_OP_AND_REVERSE);

        constexpr ColorLogicOpState logicCopyR(blendstate::writeR, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyG(blendstate::writeG, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyB(blendstate::writeB, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyA(blendstate::writeA, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRg(blendstate::writeRg, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRb(blendstate::writeRb, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRa(blendstate::writeRa, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyGb(blendstate::writeGb, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyGa(blendstate::writeGa, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyBa(blendstate::writeBa, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRgb(blendstate::writeRgb, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRga(blendstate::writeRga, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRba(blendstate::writeRba, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyGba(blendstate::writeGba, VK_LOGIC_OP_COPY);
        constexpr ColorLogicOpState logicCopyRgba(blendstate::writeRgba, VK_LOGIC_OP_COPY);

        constexpr ColorLogicOpState logicAndInvertedR(blendstate::writeR, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedG(blendstate::writeG, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedB(blendstate::writeB, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedA(blendstate::writeA, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRg(blendstate::writeRg, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRb(blendstate::writeRb, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRa(blendstate::writeRa, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedGb(blendstate::writeGb, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedGa(blendstate::writeGa, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedBa(blendstate::writeBa, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRgb(blendstate::writeRgb, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRga(blendstate::writeRga, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRba(blendstate::writeRba, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedGba(blendstate::writeGba, VK_LOGIC_OP_AND_INVERTED);
        constexpr ColorLogicOpState logicAndInvertedRgba(blendstate::writeRgba, VK_LOGIC_OP_AND_INVERTED);

        constexpr ColorLogicOpState logicNoOpR(blendstate::writeR, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpG(blendstate::writeG, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpB(blendstate::writeB, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpA(blendstate::writeA, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRg(blendstate::writeRg, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRb(blendstate::writeRb, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRa(blendstate::writeRa, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpGb(blendstate::writeGb, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpGa(blendstate::writeGa, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpBa(blendstate::writeBa, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRgb(blendstate::writeRgb, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRga(blendstate::writeRga, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRba(blendstate::writeRba, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpGba(blendstate::writeGba, VK_LOGIC_OP_NO_OP);
        constexpr ColorLogicOpState logicNoOpRgba(blendstate::writeRgba, VK_LOGIC_OP_NO_OP);

        constexpr ColorLogicOpState logicXorR(blendstate::writeR, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorG(blendstate::writeG, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorB(blendstate::writeB, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorA(blendstate::writeA, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRg(blendstate::writeRg, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRb(blendstate::writeRb, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRa(blendstate::writeRa, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorGb(blendstate::writeGb, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorGa(blendstate::writeGa, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorBa(blendstate::writeBa, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRgb(blendstate::writeRgb, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRga(blendstate::writeRga, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRba(blendstate::writeRba, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorGba(blendstate::writeGba, VK_LOGIC_OP_XOR);
        constexpr ColorLogicOpState logicXorRgba(blendstate::writeRgba, VK_LOGIC_OP_XOR);

        constexpr ColorLogicOpState logicOrR(blendstate::writeR, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrG(blendstate::writeG, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrB(blendstate::writeB, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrA(blendstate::writeA, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRg(blendstate::writeRg, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRb(blendstate::writeRb, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRa(blendstate::writeRa, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrGb(blendstate::writeGb, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrGa(blendstate::writeGa, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrBa(blendstate::writeBa, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRgb(blendstate::writeRgb, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRga(blendstate::writeRga, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRba(blendstate::writeRba, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrGba(blendstate::writeGba, VK_LOGIC_OP_OR);
        constexpr ColorLogicOpState logicOrRgba(blendstate::writeRgba, VK_LOGIC_OP_OR);

        constexpr ColorLogicOpState logicNorR(blendstate::writeR, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorG(blendstate::writeG, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorB(blendstate::writeB, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorA(blendstate::writeA, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRg(blendstate::writeRg, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRb(blendstate::writeRb, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRa(blendstate::writeRa, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorGb(blendstate::writeGb, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorGa(blendstate::writeGa, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorBa(blendstate::writeBa, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRgb(blendstate::writeRgb, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRga(blendstate::writeRga, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRba(blendstate::writeRba, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorGba(blendstate::writeGba, VK_LOGIC_OP_NOR);
        constexpr ColorLogicOpState logicNorRgba(blendstate::writeRgba, VK_LOGIC_OP_NOR);

        constexpr ColorLogicOpState logicEquivalentR(blendstate::writeR, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentG(blendstate::writeG, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentB(blendstate::writeB, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentA(blendstate::writeA, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRg(blendstate::writeRg, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRb(blendstate::writeRb, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRa(blendstate::writeRa, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentGb(blendstate::writeGb, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentGa(blendstate::writeGa, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentBa(blendstate::writeBa, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRgb(blendstate::writeRgb, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRga(blendstate::writeRga, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRba(blendstate::writeRba, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentGba(blendstate::writeGba, VK_LOGIC_OP_EQUIVALENT);
        constexpr ColorLogicOpState logicEquivalentRgba(blendstate::writeRgba, VK_LOGIC_OP_EQUIVALENT);

        constexpr ColorLogicOpState logicInvertR(blendstate::writeR, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertG(blendstate::writeG, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertB(blendstate::writeB, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertA(blendstate::writeA, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRg(blendstate::writeRg, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRb(blendstate::writeRb, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRa(blendstate::writeRa, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertGb(blendstate::writeGb, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertGa(blendstate::writeGa, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertBa(blendstate::writeBa, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRgb(blendstate::writeRgb, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRga(blendstate::writeRga, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRba(blendstate::writeRba, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertGba(blendstate::writeGba, VK_LOGIC_OP_INVERT);
        constexpr ColorLogicOpState logicInvertRgba(blendstate::writeRgba, VK_LOGIC_OP_INVERT);

        constexpr ColorLogicOpState logicOrReverseR(blendstate::writeR, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseG(blendstate::writeG, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseB(blendstate::writeB, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseA(blendstate::writeA, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRg(blendstate::writeRg, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRb(blendstate::writeRb, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRa(blendstate::writeRa, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseGb(blendstate::writeGb, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseGa(blendstate::writeGa, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseBa(blendstate::writeBa, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRgb(blendstate::writeRgb, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRga(blendstate::writeRga, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRba(blendstate::writeRba, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseGba(blendstate::writeGba, VK_LOGIC_OP_OR_REVERSE);
        constexpr ColorLogicOpState logicOrReverseRgba(blendstate::writeRgba, VK_LOGIC_OP_OR_REVERSE);

        constexpr ColorLogicOpState logicCopyInvertedR(blendstate::writeR, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedG(blendstate::writeG, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedB(blendstate::writeB, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedA(blendstate::writeA, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRg(blendstate::writeRg, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRb(blendstate::writeRb, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRa(blendstate::writeRa, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedGb(blendstate::writeGb, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedGa(blendstate::writeGa, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedBa(blendstate::writeBa, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRgb(blendstate::writeRgb, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRga(blendstate::writeRga, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRba(blendstate::writeRba, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedGba(blendstate::writeGba, VK_LOGIC_OP_COPY_INVERTED);
        constexpr ColorLogicOpState logicCopyInvertedRgba(blendstate::writeRgba, VK_LOGIC_OP_COPY_INVERTED);

        constexpr ColorLogicOpState logicOrInvertedR(blendstate::writeR, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedG(blendstate::writeG, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedB(blendstate::writeB, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedA(blendstate::writeA, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRg(blendstate::writeRg, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRb(blendstate::writeRb, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRa(blendstate::writeRa, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedGb(blendstate::writeGb, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedGa(blendstate::writeGa, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedBa(blendstate::writeBa, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRgb(blendstate::writeRgb, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRga(blendstate::writeRga, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRba(blendstate::writeRba, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedGba(blendstate::writeGba, VK_LOGIC_OP_OR_INVERTED);
        constexpr ColorLogicOpState logicOrInvertedRgba(blendstate::writeRgba, VK_LOGIC_OP_OR_INVERTED);

        constexpr ColorLogicOpState logicNandR(blendstate::writeR, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandG(blendstate::writeG, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandB(blendstate::writeB, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandA(blendstate::writeA, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRg(blendstate::writeRg, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRb(blendstate::writeRb, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRa(blendstate::writeRa, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandGb(blendstate::writeGb, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandGa(blendstate::writeGa, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandBa(blendstate::writeBa, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRgb(blendstate::writeRgb, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRga(blendstate::writeRga, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRba(blendstate::writeRba, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandGba(blendstate::writeGba, VK_LOGIC_OP_NAND);
        constexpr ColorLogicOpState logicNandRgba(blendstate::writeRgba, VK_LOGIC_OP_NAND);

        constexpr ColorLogicOpState logicSetR(blendstate::writeR, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetG(blendstate::writeG, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetB(blendstate::writeB, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetA(blendstate::writeA, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRg(blendstate::writeRg, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRb(blendstate::writeRb, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRa(blendstate::writeRa, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetGb(blendstate::writeGb, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetGa(blendstate::writeGa, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetBa(blendstate::writeBa, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRgb(blendstate::writeRgb, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRga(blendstate::writeRga, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRba(blendstate::writeRba, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetGba(blendstate::writeGba, VK_LOGIC_OP_SET);
        constexpr ColorLogicOpState logicSetRgba(blendstate::writeRgba, VK_LOGIC_OP_SET);
    } // namespace renderstate
} // namespace magma
