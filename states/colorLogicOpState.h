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
#define MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(name, logicOp)\
    constexpr ColorLogicOpState name##R(blendstate::write##R, logicOp);\
    constexpr ColorLogicOpState name##G(blendstate::write##G, logicOp);\
    constexpr ColorLogicOpState name##B(blendstate::write##B, logicOp);\
    constexpr ColorLogicOpState name##A(blendstate::write##A, logicOp);\
    constexpr ColorLogicOpState name##Rg(blendstate::write##Rg, logicOp);\
    constexpr ColorLogicOpState name##Rb(blendstate::write##Rb, logicOp);\
    constexpr ColorLogicOpState name##Ra(blendstate::write##Ra, logicOp);\
    constexpr ColorLogicOpState name##Gb(blendstate::write##Gb, logicOp);\
    constexpr ColorLogicOpState name##Ga(blendstate::write##Ga, logicOp);\
    constexpr ColorLogicOpState name##Ba(blendstate::write##Ba, logicOp);\
    constexpr ColorLogicOpState name##Rgb(blendstate::write##Rgb, logicOp);\
    constexpr ColorLogicOpState name##Rga(blendstate::write##Rga, logicOp);\
    constexpr ColorLogicOpState name##Rba(blendstate::write##Rba, logicOp);\
    constexpr ColorLogicOpState name##Gba(blendstate::write##Gba, logicOp);\
    constexpr ColorLogicOpState name##Rgba(blendstate::write##Rgba, logicOp);

    namespace renderstate
    {
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicClear, VK_LOGIC_OP_CLEAR)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicAnd, VK_LOGIC_OP_AND)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicAndReverse, VK_LOGIC_OP_AND_REVERSE)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicCopy, VK_LOGIC_OP_COPY)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicAndInverted, VK_LOGIC_OP_AND_INVERTED)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicNoOp, VK_LOGIC_OP_NO_OP)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicXor, VK_LOGIC_OP_XOR)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicOr, VK_LOGIC_OP_OR)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicNor, VK_LOGIC_OP_NOR)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicEquivalent, VK_LOGIC_OP_EQUIVALENT)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicInvert, VK_LOGIC_OP_INVERT)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicOrReverse, VK_LOGIC_OP_OR_REVERSE)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicCopyInverted, VK_LOGIC_OP_COPY_INVERTED)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicOrInverted, VK_LOGIC_OP_OR_INVERTED)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicNand, VK_LOGIC_OP_NAND)
        MAGMA_DEFINE_COLOR_LOGIC_OP_STATE_PERMUTATIONS(logicSet, VK_LOGIC_OP_SET)
    } // namespace renderstate
} // namespace magma
