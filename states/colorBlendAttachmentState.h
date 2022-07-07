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
#include "colorMask.h"

namespace magma
{
    /* Per-target blending state for each individual color attachment. */

    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        constexpr ColorBlendAttachmentState(VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr ColorBlendAttachmentState(VkBlendFactor srcBlendFactor,
            VkBlendFactor dstBlendFactor,
            VkBlendOp blendOp,
            VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr ColorBlendAttachmentState(VkBlendFactor srcColorBlendFactor,
            VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp,
            VkBlendFactor srcAlphaBlendFactor,
            VkBlendFactor dstAlphaBlendFactor,
            VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendAttachmentState&) const noexcept;
    };
} // namespace magma

#include "colorBlendAttachmentState.inl"

namespace magma
{
    namespace blendstate
    {
        constexpr ColorBlendAttachmentState writeNone(colormask::none);

        constexpr ColorBlendAttachmentState writeR(colormask::r);
        constexpr ColorBlendAttachmentState writeG(colormask::g);
        constexpr ColorBlendAttachmentState writeB(colormask::b);
        constexpr ColorBlendAttachmentState writeA(colormask::a);
        constexpr ColorBlendAttachmentState writeRg(colormask::rg);
        constexpr ColorBlendAttachmentState writeRb(colormask::rb);
        constexpr ColorBlendAttachmentState writeRa(colormask::ra);
        constexpr ColorBlendAttachmentState writeGb(colormask::gb);
        constexpr ColorBlendAttachmentState writeGa(colormask::ga);
        constexpr ColorBlendAttachmentState writeBa(colormask::ba);
        constexpr ColorBlendAttachmentState writeRgb(colormask::rgb);
        constexpr ColorBlendAttachmentState writeRga(colormask::rga);
        constexpr ColorBlendAttachmentState writeRba(colormask::rba);
        constexpr ColorBlendAttachmentState writeGba(colormask::gba);
        constexpr ColorBlendAttachmentState writeRgba(colormask::rgba);

        constexpr ColorBlendAttachmentState addR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::r);
        constexpr ColorBlendAttachmentState addG(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::g);
        constexpr ColorBlendAttachmentState addB(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::b);
        constexpr ColorBlendAttachmentState addA(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::a);
        constexpr ColorBlendAttachmentState addRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rg);
        constexpr ColorBlendAttachmentState addRb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rb);
        constexpr ColorBlendAttachmentState addRa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::ra);
        constexpr ColorBlendAttachmentState addGb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::gb);
        constexpr ColorBlendAttachmentState addGa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::ga);
        constexpr ColorBlendAttachmentState addBa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::ba);
        constexpr ColorBlendAttachmentState addRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rgb);
        constexpr ColorBlendAttachmentState addRga(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rga);
        constexpr ColorBlendAttachmentState addRba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rba);
        constexpr ColorBlendAttachmentState addGba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::gba);
        constexpr ColorBlendAttachmentState addRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rgba);

        constexpr ColorBlendAttachmentState subtractR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::r);
        constexpr ColorBlendAttachmentState subtractG(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::g);
        constexpr ColorBlendAttachmentState subtractB(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::b);
        constexpr ColorBlendAttachmentState subtractA(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::a);
        constexpr ColorBlendAttachmentState subtractRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::rg);
        constexpr ColorBlendAttachmentState subtractRb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::rb);
        constexpr ColorBlendAttachmentState subtractRa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::ra);
        constexpr ColorBlendAttachmentState subtractGb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::gb);
        constexpr ColorBlendAttachmentState subtractGa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::ga);
        constexpr ColorBlendAttachmentState subtractBa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::ba);
        constexpr ColorBlendAttachmentState subtractRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::rgb);
        constexpr ColorBlendAttachmentState subtractRga(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::rga);
        constexpr ColorBlendAttachmentState subtractRba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::rba);
        constexpr ColorBlendAttachmentState subtractGba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::gba);
        constexpr ColorBlendAttachmentState subtractRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colormask::rgba);

        constexpr ColorBlendAttachmentState reverseSubtractR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::r);
        constexpr ColorBlendAttachmentState reverseSubtractG(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::g);
        constexpr ColorBlendAttachmentState reverseSubtractB(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::b);
        constexpr ColorBlendAttachmentState reverseSubtractA(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::a);
        constexpr ColorBlendAttachmentState reverseSubtractRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::rg);
        constexpr ColorBlendAttachmentState reverseSubtractRb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::rb);
        constexpr ColorBlendAttachmentState reverseSubtractRa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::ra);
        constexpr ColorBlendAttachmentState reverseSubtractGb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::gb);
        constexpr ColorBlendAttachmentState reverseSubtractGa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::ga);
        constexpr ColorBlendAttachmentState reverseSubtractBa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::ba);
        constexpr ColorBlendAttachmentState reverseSubtractRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::rgb);
        constexpr ColorBlendAttachmentState reverseSubtractRga(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::rga);
        constexpr ColorBlendAttachmentState reverseSubtractRba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::rba);
        constexpr ColorBlendAttachmentState reverseSubtractGba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::gba);
        constexpr ColorBlendAttachmentState reverseSubtractRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colormask::rgba);

        constexpr ColorBlendAttachmentState minR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::r);
        constexpr ColorBlendAttachmentState minG(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::g);
        constexpr ColorBlendAttachmentState minB(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::b);
        constexpr ColorBlendAttachmentState minA(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::a);
        constexpr ColorBlendAttachmentState minRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::rg);
        constexpr ColorBlendAttachmentState minRb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::rb);
        constexpr ColorBlendAttachmentState minRa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::ra);
        constexpr ColorBlendAttachmentState minGb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::gb);
        constexpr ColorBlendAttachmentState minGa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::ga);
        constexpr ColorBlendAttachmentState minBa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::ba);
        constexpr ColorBlendAttachmentState minRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::rgb);
        constexpr ColorBlendAttachmentState minRga(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::rga);
        constexpr ColorBlendAttachmentState minRba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::rba);
        constexpr ColorBlendAttachmentState minGba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::gba);
        constexpr ColorBlendAttachmentState minRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colormask::rgba);

        constexpr ColorBlendAttachmentState maxR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::r);
        constexpr ColorBlendAttachmentState maxG(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::g);
        constexpr ColorBlendAttachmentState maxB(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::b);
        constexpr ColorBlendAttachmentState maxA(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::a);
        constexpr ColorBlendAttachmentState maxRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::rg);
        constexpr ColorBlendAttachmentState maxRb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::rb);
        constexpr ColorBlendAttachmentState maxRa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::ra);
        constexpr ColorBlendAttachmentState maxGb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::gb);
        constexpr ColorBlendAttachmentState maxGa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::ga);
        constexpr ColorBlendAttachmentState maxBa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::ba);
        constexpr ColorBlendAttachmentState maxRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::rgb);
        constexpr ColorBlendAttachmentState maxRga(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::rga);
        constexpr ColorBlendAttachmentState maxRba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::rba);
        constexpr ColorBlendAttachmentState maxGba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::gba);
        constexpr ColorBlendAttachmentState maxRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colormask::rgba);

        constexpr ColorBlendAttachmentState multiplyR(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::r);
        constexpr ColorBlendAttachmentState multiplyG(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::g);
        constexpr ColorBlendAttachmentState multiplyB(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::b);
        constexpr ColorBlendAttachmentState multiplyA(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::a);
        constexpr ColorBlendAttachmentState multiplyRg(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::rg);
        constexpr ColorBlendAttachmentState multiplyRb(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::rb);
        constexpr ColorBlendAttachmentState multiplyRa(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::ra);
        constexpr ColorBlendAttachmentState multiplyGb(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::gb);
        constexpr ColorBlendAttachmentState multiplyGa(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::ga);
        constexpr ColorBlendAttachmentState multiplyBa(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::ba);
        constexpr ColorBlendAttachmentState multiplyRgb(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::rgb);
        constexpr ColorBlendAttachmentState multiplyRga(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::rga);
        constexpr ColorBlendAttachmentState multiplyRba(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::rba);
        constexpr ColorBlendAttachmentState multiplyGba(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::gba);
        constexpr ColorBlendAttachmentState multiplyRgba(VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colormask::rgba);

        constexpr ColorBlendAttachmentState normalR(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::r);
        constexpr ColorBlendAttachmentState normalG(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::g);
        constexpr ColorBlendAttachmentState normalB(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::b);
        constexpr ColorBlendAttachmentState normalA(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::a);
        constexpr ColorBlendAttachmentState normalRg(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rg);
        constexpr ColorBlendAttachmentState normalRb(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rb);
        constexpr ColorBlendAttachmentState normalRa(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::ra);
        constexpr ColorBlendAttachmentState normalGb(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::gb);
        constexpr ColorBlendAttachmentState normalGa(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::ga);
        constexpr ColorBlendAttachmentState normalBa(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::ba);
        constexpr ColorBlendAttachmentState normalRgb(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rgb);
        constexpr ColorBlendAttachmentState normalRga(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rga);
        constexpr ColorBlendAttachmentState normalRba(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rba);
        constexpr ColorBlendAttachmentState normalGba(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::gba);
        constexpr ColorBlendAttachmentState normalRgba(VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rgba);

        constexpr ColorBlendAttachmentState normalPremultipliedR(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::r);
        constexpr ColorBlendAttachmentState normalPremultipliedG(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::g);
        constexpr ColorBlendAttachmentState normalPremultipliedB(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::b);
        constexpr ColorBlendAttachmentState normalPremultipliedA(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::a);
        constexpr ColorBlendAttachmentState normalPremultipliedRg(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rg);
        constexpr ColorBlendAttachmentState normalPremultipliedRb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rb);
        constexpr ColorBlendAttachmentState normalPremultipliedRa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::ra);
        constexpr ColorBlendAttachmentState normalPremultipliedGb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::gb);
        constexpr ColorBlendAttachmentState normalPremultipliedGa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::ga);
        constexpr ColorBlendAttachmentState normalPremultipliedBa(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::ba);
        constexpr ColorBlendAttachmentState normalPremultipliedRgb(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rgb);
        constexpr ColorBlendAttachmentState normalPremultipliedRga(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rga);
        constexpr ColorBlendAttachmentState normalPremultipliedRba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rba);
        constexpr ColorBlendAttachmentState normalPremultipliedGba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::gba);
        constexpr ColorBlendAttachmentState normalPremultipliedRgba(VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colormask::rgba);

        // 1 - (1 - a) (1 - b) = a * (1 - b) + b = a + (1 - a) * b
        constexpr ColorBlendAttachmentState screenR(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::r);
        constexpr ColorBlendAttachmentState screenG(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::g);
        constexpr ColorBlendAttachmentState screenB(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::b);
        constexpr ColorBlendAttachmentState screenA(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::a);
        constexpr ColorBlendAttachmentState screenRg(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rg);
        constexpr ColorBlendAttachmentState screenRb(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rb);
        constexpr ColorBlendAttachmentState screenRa(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::ra);
        constexpr ColorBlendAttachmentState screenGb(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::gb);
        constexpr ColorBlendAttachmentState screenGa(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::ga);
        constexpr ColorBlendAttachmentState screenBa(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::ba);
        constexpr ColorBlendAttachmentState screenRgb(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rgb);
        constexpr ColorBlendAttachmentState screenRga(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rga);
        constexpr ColorBlendAttachmentState screenRba(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rba);
        constexpr ColorBlendAttachmentState screenGba(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::gba);
        constexpr ColorBlendAttachmentState screenRgba(VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colormask::rgba);
    } // namespace blendstate
} // namespace magma
