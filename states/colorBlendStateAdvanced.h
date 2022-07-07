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
    /* This extension adds a number of "advanced" blending operations that can be
       used to perform new color blending operations, many of which are more
       complex than the standard blend modes provided by unextended Vulkan. */

    struct AdvancedColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
    public:
        constexpr AdvancedColorBlendAttachmentState(VkBlendOp advancedBlendOp,
            VkColorComponentFlags colorWriteMask = colormask::rgba) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr bool operator==(const AdvancedColorBlendAttachmentState&) const noexcept;
    };

    /* For explanation, see
       https://www.khronos.org/registry/OpenGL/extensions/NV/NV_blend_equation_advanced.txt */

#ifdef VK_EXT_blend_operation_advanced
    struct AdvancedColorBlendState final : ColorBlendState
    {
    public:
        explicit AdvancedColorBlendState(const AdvancedColorBlendAttachmentState& attachment,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap);
        explicit AdvancedColorBlendState(const std::initializer_list<AdvancedColorBlendAttachmentState>& attachments,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap);
        explicit AdvancedColorBlendState(const std::vector<AdvancedColorBlendAttachmentState>& attachments,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap);
        AdvancedColorBlendState(const AdvancedColorBlendState&);
        AdvancedColorBlendState& operator=(const AdvancedColorBlendState&);
        ~AdvancedColorBlendState();
        hash_t hash() const noexcept;
        bool operator==(const AdvancedColorBlendState&) const noexcept;
    };
#endif // VK_EXT_blend_operation_advanced
} // namespace magma

#include "colorBlendStateAdvanced.inl"

namespace magma
{
#define MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(name, blendOp)\
    constexpr AdvancedColorBlendAttachmentState name##R(blendOp, colormask::r);\
    constexpr AdvancedColorBlendAttachmentState name##G(blendOp, colormask::g);\
    constexpr AdvancedColorBlendAttachmentState name##B(blendOp, colormask::b);\
    constexpr AdvancedColorBlendAttachmentState name##A(blendOp, colormask::a);\
    constexpr AdvancedColorBlendAttachmentState name##Rg(blendOp, colormask::rg);\
    constexpr AdvancedColorBlendAttachmentState name##Rb(blendOp, colormask::rb);\
    constexpr AdvancedColorBlendAttachmentState name##Ra(blendOp, colormask::ra);\
    constexpr AdvancedColorBlendAttachmentState name##Gb(blendOp, colormask::gb);\
    constexpr AdvancedColorBlendAttachmentState name##Ga(blendOp, colormask::ga);\
    constexpr AdvancedColorBlendAttachmentState name##Ba(blendOp, colormask::ba);\
    constexpr AdvancedColorBlendAttachmentState name##Rgb(blendOp, colormask::rgb);\
    constexpr AdvancedColorBlendAttachmentState name##Rga(blendOp, colormask::rga);\
    constexpr AdvancedColorBlendAttachmentState name##Rba(blendOp, colormask::rba);\
    constexpr AdvancedColorBlendAttachmentState name##Gba(blendOp, colormask::gba);\
    constexpr AdvancedColorBlendAttachmentState name##Rgba(blendOp, colormask::rgba);

    namespace blendstate
    {
#ifdef VK_EXT_blend_operation_advanced
        namespace advanced
        {
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendZero, VK_BLEND_OP_ZERO_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrc, VK_BLEND_OP_SRC_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDst, VK_BLEND_OP_DST_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcOver, VK_BLEND_OP_SRC_OVER_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstOver, VK_BLEND_OP_DST_OVER_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcIn, VK_BLEND_OP_SRC_IN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstIn, VK_BLEND_OP_DST_IN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcOut, VK_BLEND_OP_SRC_OUT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstOut, VK_BLEND_OP_DST_OUT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSrcAtop, VK_BLEND_OP_SRC_ATOP_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDstAtop, VK_BLEND_OP_DST_ATOP_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendXor, VK_BLEND_OP_XOR_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendMultiply, VK_BLEND_OP_MULTIPLY_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendScreen, VK_BLEND_OP_SCREEN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendOverlay, VK_BLEND_OP_OVERLAY_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDarken, VK_BLEND_OP_DARKEN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLighten, VK_BLEND_OP_LIGHTEN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendColorDodge, VK_BLEND_OP_COLORDODGE_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendColorBurn, VK_BLEND_OP_COLORBURN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHardLight, VK_BLEND_OP_HARDLIGHT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendSoftLight, VK_BLEND_OP_SOFTLIGHT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendDifference, VK_BLEND_OP_DIFFERENCE_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendExclusion, VK_BLEND_OP_EXCLUSION_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendInvert, VK_BLEND_OP_INVERT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendInvertRgb, VK_BLEND_OP_INVERT_RGB_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLinearDodge, VK_BLEND_OP_LINEARDODGE_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLinearBurn, VK_BLEND_OP_LINEARBURN_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendVividLight, VK_BLEND_OP_VIVIDLIGHT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendLinearLight, VK_BLEND_OP_LINEARLIGHT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPinLight, VK_BLEND_OP_PINLIGHT_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHardMix, VK_BLEND_OP_HARDMIX_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslHue, VK_BLEND_OP_HSL_HUE_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslSaturation, VK_BLEND_OP_HSL_SATURATION_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslColor, VK_BLEND_OP_HSL_COLOR_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendHslLuminosity, VK_BLEND_OP_HSL_LUMINOSITY_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlus, VK_BLEND_OP_PLUS_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlusClamped, VK_BLEND_OP_PLUS_CLAMPED_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlusClampedAlpha, VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendPlusDarken, VK_BLEND_OP_PLUS_DARKER_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendMinus, VK_BLEND_OP_MINUS_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendMinusClamped, VK_BLEND_OP_MINUS_CLAMPED_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendContrast, VK_BLEND_OP_CONTRAST_EXT)
            MAGMA_DEFINE_ADVANCED_COLOR_BLEND_ATTACHMENT_STATE_PERMUTATIONS(blendInvertOvg, VK_BLEND_OP_INVERT_OVG_EXT)

            constexpr AdvancedColorBlendAttachmentState blendRed(VK_BLEND_OP_RED_EXT); // (R,G,B,A) = (Rs', Gd, Bd, Ad)
            constexpr AdvancedColorBlendAttachmentState blendGreen(VK_BLEND_OP_GREEN_EXT); // (R,G,B,A) = (Rd, Gs', Bd, Ad)
            constexpr AdvancedColorBlendAttachmentState blendBlue(VK_BLEND_OP_BLUE_EXT); // (R,G,B,A) = (Rd, Gd, Bs', Ad)
        } // namespace advanced
#endif // VK_EXT_blend_operation_advanced
    } // namespace blendstate
} // namespace magma
