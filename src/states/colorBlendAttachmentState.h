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
#include "colorMask.h"

namespace magma
{
    /* Defines blend state for each individual color attachment. */

    struct ColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
        constexpr ColorBlendAttachmentState(VkColorComponentFlags colorWriteMask = colormask::none) noexcept;
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
        constexpr ColorBlendAttachmentState(VkBlendOp advancedBlendOp,
            VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const ColorBlendAttachmentState&) const noexcept;
    };
} // namespace magma

#include "colorBlendAttachmentState.inl"

#define MAGMA_COLOR_WRITE_ATTACHMENT_STATE_PERMUTATIONS(name)\
    constexpr ColorBlendAttachmentState name##R(colormask::r);\
    constexpr ColorBlendAttachmentState name##G(colormask::g);\
    constexpr ColorBlendAttachmentState name##B(colormask::b);\
    constexpr ColorBlendAttachmentState name##A(colormask::a);\
    constexpr ColorBlendAttachmentState name##Rg(colormask::rg);\
    constexpr ColorBlendAttachmentState name##Rb(colormask::rb);\
    constexpr ColorBlendAttachmentState name##Ra(colormask::ra);\
    constexpr ColorBlendAttachmentState name##Gb(colormask::gb);\
    constexpr ColorBlendAttachmentState name##Ga(colormask::ga);\
    constexpr ColorBlendAttachmentState name##Ba(colormask::ba);\
    constexpr ColorBlendAttachmentState name##Rgb(colormask::rgb);\
    constexpr ColorBlendAttachmentState name##Rga(colormask::rga);\
    constexpr ColorBlendAttachmentState name##Rba(colormask::rba);\
    constexpr ColorBlendAttachmentState name##Gba(colormask::gba);\
    constexpr ColorBlendAttachmentState name##Rgba(colormask::rgba);

#define MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(name, srcBlend, dstBlend, blendOp)\
    constexpr ColorBlendAttachmentState name##R(srcBlend, dstBlend, blendOp, colormask::r);\
    constexpr ColorBlendAttachmentState name##G(srcBlend, dstBlend, blendOp, colormask::g);\
    constexpr ColorBlendAttachmentState name##B(srcBlend, dstBlend, blendOp, colormask::b);\
    constexpr ColorBlendAttachmentState name##A(srcBlend, dstBlend, blendOp, colormask::a);\
    constexpr ColorBlendAttachmentState name##Rg(srcBlend, dstBlend, blendOp, colormask::rg);\
    constexpr ColorBlendAttachmentState name##Rb(srcBlend, dstBlend, blendOp, colormask::rb);\
    constexpr ColorBlendAttachmentState name##Ra(srcBlend, dstBlend, blendOp, colormask::ra);\
    constexpr ColorBlendAttachmentState name##Gb(srcBlend, dstBlend, blendOp, colormask::gb);\
    constexpr ColorBlendAttachmentState name##Ga(srcBlend, dstBlend, blendOp, colormask::ga);\
    constexpr ColorBlendAttachmentState name##Ba(srcBlend, dstBlend, blendOp, colormask::ba);\
    constexpr ColorBlendAttachmentState name##Rgb(srcBlend, dstBlend, blendOp, colormask::rgb);\
    constexpr ColorBlendAttachmentState name##Rga(srcBlend, dstBlend, blendOp, colormask::rga);\
    constexpr ColorBlendAttachmentState name##Rba(srcBlend, dstBlend, blendOp, colormask::rba);\
    constexpr ColorBlendAttachmentState name##Gba(srcBlend, dstBlend, blendOp, colormask::gba);\
    constexpr ColorBlendAttachmentState name##Rgba(srcBlend, dstBlend, blendOp, colormask::rgba);

namespace magma::blendstate
{
    constexpr ColorBlendAttachmentState writeNone(colormask::none);
    MAGMA_COLOR_WRITE_ATTACHMENT_STATE_PERMUTATIONS(write)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(add, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(subtract, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(reverseSubtract, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(min, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(max, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(multiply, VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(normal, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(normalPremultiplied, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD)
    MAGMA_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(screen, VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD) // 1 - (1 - a) * (1 - b) = a * (1 - b) + b = a + (1 - a) * b
} // namespace magma::blendstate

#ifdef VK_EXT_blend_operation_advanced
#define MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(name, advancedBlendOp)\
    constexpr ColorBlendAttachmentState name##R(advancedBlendOp, colormask::r);\
    constexpr ColorBlendAttachmentState name##G(advancedBlendOp, colormask::g);\
    constexpr ColorBlendAttachmentState name##B(advancedBlendOp, colormask::b);\
    constexpr ColorBlendAttachmentState name##A(advancedBlendOp, colormask::a);\
    constexpr ColorBlendAttachmentState name##Rg(advancedBlendOp, colormask::rg);\
    constexpr ColorBlendAttachmentState name##Rb(advancedBlendOp, colormask::rb);\
    constexpr ColorBlendAttachmentState name##Ra(advancedBlendOp, colormask::ra);\
    constexpr ColorBlendAttachmentState name##Gb(advancedBlendOp, colormask::gb);\
    constexpr ColorBlendAttachmentState name##Ga(advancedBlendOp, colormask::ga);\
    constexpr ColorBlendAttachmentState name##Ba(advancedBlendOp, colormask::ba);\
    constexpr ColorBlendAttachmentState name##Rgb(advancedBlendOp, colormask::rgb);\
    constexpr ColorBlendAttachmentState name##Rga(advancedBlendOp, colormask::rga);\
    constexpr ColorBlendAttachmentState name##Rba(advancedBlendOp, colormask::rba);\
    constexpr ColorBlendAttachmentState name##Gba(advancedBlendOp, colormask::gba);\
    constexpr ColorBlendAttachmentState name##Rgba(advancedBlendOp, colormask::rgba);

namespace magma::blendstate::advanced
{
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendZero, VK_BLEND_OP_ZERO_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrc, VK_BLEND_OP_SRC_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDst, VK_BLEND_OP_DST_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcOver, VK_BLEND_OP_SRC_OVER_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstOver, VK_BLEND_OP_DST_OVER_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcIn, VK_BLEND_OP_SRC_IN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstIn, VK_BLEND_OP_DST_IN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcOut, VK_BLEND_OP_SRC_OUT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstOut, VK_BLEND_OP_DST_OUT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcAtop, VK_BLEND_OP_SRC_ATOP_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstAtop, VK_BLEND_OP_DST_ATOP_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendXor, VK_BLEND_OP_XOR_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendMultiply, VK_BLEND_OP_MULTIPLY_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendScreen, VK_BLEND_OP_SCREEN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendOverlay, VK_BLEND_OP_OVERLAY_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDarken, VK_BLEND_OP_DARKEN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLighten, VK_BLEND_OP_LIGHTEN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendColorDodge, VK_BLEND_OP_COLORDODGE_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendColorBurn, VK_BLEND_OP_COLORBURN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHardLight, VK_BLEND_OP_HARDLIGHT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSoftLight, VK_BLEND_OP_SOFTLIGHT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDifference, VK_BLEND_OP_DIFFERENCE_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendExclusion, VK_BLEND_OP_EXCLUSION_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendInvert, VK_BLEND_OP_INVERT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendInvertRgb, VK_BLEND_OP_INVERT_RGB_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLinearDodge, VK_BLEND_OP_LINEARDODGE_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLinearBurn, VK_BLEND_OP_LINEARBURN_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendVividLight, VK_BLEND_OP_VIVIDLIGHT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLinearLight, VK_BLEND_OP_LINEARLIGHT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPinLight, VK_BLEND_OP_PINLIGHT_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHardMix, VK_BLEND_OP_HARDMIX_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslHue, VK_BLEND_OP_HSL_HUE_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslSaturation, VK_BLEND_OP_HSL_SATURATION_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslColor, VK_BLEND_OP_HSL_COLOR_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslLuminosity, VK_BLEND_OP_HSL_LUMINOSITY_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlus, VK_BLEND_OP_PLUS_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlusClamped, VK_BLEND_OP_PLUS_CLAMPED_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlusClampedAlpha, VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlusDarken, VK_BLEND_OP_PLUS_DARKER_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendMinus, VK_BLEND_OP_MINUS_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendMinusClamped, VK_BLEND_OP_MINUS_CLAMPED_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendContrast, VK_BLEND_OP_CONTRAST_EXT)
    MAGMA_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendInvertOvg, VK_BLEND_OP_INVERT_OVG_EXT)

    constexpr ColorBlendAttachmentState blendRed(VK_BLEND_OP_RED_EXT); // (R,G,B,A) = (Rs', Gd, Bd, Ad)
    constexpr ColorBlendAttachmentState blendGreen(VK_BLEND_OP_GREEN_EXT); // (R,G,B,A) = (Rd, Gs', Bd, Ad)
    constexpr ColorBlendAttachmentState blendBlue(VK_BLEND_OP_BLUE_EXT); // (R,G,B,A) = (Rd, Gd, Bs', Ad)
} // namespace magma::blendstate::advanced
#endif // VK_EXT_blend_operation_advanced
