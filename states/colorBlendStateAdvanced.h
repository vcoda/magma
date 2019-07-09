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
#include "colorBlendState.h"

namespace magma
{
    /* This extension adds a number of "advanced" blending operations that can:
       be used to perform new color blending operations, many of which are more
       complex than the standard blend modes provided by unextended Vulkan. */

    struct AdvancedColorBlendAttachmentState : VkPipelineColorBlendAttachmentState
    {
    public:
        constexpr AdvancedColorBlendAttachmentState(VkBlendOp advancedBlendOp,
            VkColorComponentFlags colorWriteMask = 
                VK_COLOR_COMPONENT_R_BIT | 
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT | 
                VK_COLOR_COMPONENT_A_BIT) noexcept;
        constexpr size_t hash() const noexcept;
        constexpr bool operator==(const AdvancedColorBlendAttachmentState&) const noexcept;
    };

    struct AdvancedColorBlendState final : ColorBlendState
    {
    public:
        AdvancedColorBlendState(const AdvancedColorBlendAttachmentState& attachment,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap);
        AdvancedColorBlendState(const std::vector<AdvancedColorBlendAttachmentState>& attachments,
            bool srcPremultiplied,
            bool dstPremultiplied,
            VkBlendOverlapEXT blendOverlap);
        AdvancedColorBlendState(const AdvancedColorBlendState&);
        AdvancedColorBlendState& operator=(const AdvancedColorBlendState&);
        ~AdvancedColorBlendState();
        size_t hash() const noexcept;
        bool operator==(const AdvancedColorBlendState&) const noexcept;
    };
} // namespace magma

#include "colorBlendStateAdvanced.inl"

namespace magma
{
    namespace blendstates
    {
        namespace advanced
        {
            constexpr AdvancedColorBlendAttachmentState blendZero(VK_BLEND_OP_ZERO_EXT);
            constexpr AdvancedColorBlendAttachmentState blendSrc(VK_BLEND_OP_SRC_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDst(VK_BLEND_OP_DST_EXT);
            constexpr AdvancedColorBlendAttachmentState blendSrcOver(VK_BLEND_OP_SRC_OVER_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDstOver(VK_BLEND_OP_DST_OVER_EXT);
            constexpr AdvancedColorBlendAttachmentState blendSrcIn(VK_BLEND_OP_SRC_IN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDstIn(VK_BLEND_OP_DST_IN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendSrcOut(VK_BLEND_OP_SRC_OUT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDstOut(VK_BLEND_OP_DST_OUT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendSrcAtop(VK_BLEND_OP_SRC_ATOP_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDstAtop(VK_BLEND_OP_DST_ATOP_EXT);
            constexpr AdvancedColorBlendAttachmentState blendXor(VK_BLEND_OP_XOR_EXT);
            constexpr AdvancedColorBlendAttachmentState blendMultiply(VK_BLEND_OP_MULTIPLY_EXT);
            constexpr AdvancedColorBlendAttachmentState blendScreen(VK_BLEND_OP_SCREEN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendOverlay(VK_BLEND_OP_OVERLAY_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDarken(VK_BLEND_OP_DARKEN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendLighten(VK_BLEND_OP_LIGHTEN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendColorDodge(VK_BLEND_OP_COLORDODGE_EXT);
            constexpr AdvancedColorBlendAttachmentState blendColorBurn(VK_BLEND_OP_COLORBURN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendHardLight(VK_BLEND_OP_HARDLIGHT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendSoftLight(VK_BLEND_OP_SOFTLIGHT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendDifference(VK_BLEND_OP_DIFFERENCE_EXT);
            constexpr AdvancedColorBlendAttachmentState blendExclusion(VK_BLEND_OP_EXCLUSION_EXT);
            constexpr AdvancedColorBlendAttachmentState blendInvert(VK_BLEND_OP_INVERT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgb(VK_BLEND_OP_INVERT_RGB_EXT);
            constexpr AdvancedColorBlendAttachmentState blendLinearDodge(VK_BLEND_OP_LINEARDODGE_EXT);
            constexpr AdvancedColorBlendAttachmentState blendLinearBurn(VK_BLEND_OP_LINEARBURN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendVividLight(VK_BLEND_OP_VIVIDLIGHT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendLinearLight(VK_BLEND_OP_LINEARLIGHT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendPinLight(VK_BLEND_OP_PINLIGHT_EXT);
            constexpr AdvancedColorBlendAttachmentState blendHardMix(VK_BLEND_OP_HARDMIX_EXT);
            constexpr AdvancedColorBlendAttachmentState blendHslHue(VK_BLEND_OP_HSL_HUE_EXT);
            constexpr AdvancedColorBlendAttachmentState blendHslSaturation(VK_BLEND_OP_HSL_SATURATION_EXT);
            constexpr AdvancedColorBlendAttachmentState blendHslColor(VK_BLEND_OP_HSL_COLOR_EXT);
            constexpr AdvancedColorBlendAttachmentState blendHslLuminosity(VK_BLEND_OP_HSL_LUMINOSITY_EXT);
            constexpr AdvancedColorBlendAttachmentState blendPlus(VK_BLEND_OP_PLUS_EXT);
            constexpr AdvancedColorBlendAttachmentState blendPlusClamped(VK_BLEND_OP_PLUS_CLAMPED_EXT);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedAlpha(VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT);
            constexpr AdvancedColorBlendAttachmentState blendPlusDarken(VK_BLEND_OP_PLUS_DARKER_EXT);
            constexpr AdvancedColorBlendAttachmentState blendMinus(VK_BLEND_OP_MINUS_EXT);
            constexpr AdvancedColorBlendAttachmentState blendMinusClamped(VK_BLEND_OP_MINUS_CLAMPED_EXT);
            constexpr AdvancedColorBlendAttachmentState blendContrast(VK_BLEND_OP_CONTRAST_EXT);
            constexpr AdvancedColorBlendAttachmentState blendInvertOvg(VK_BLEND_OP_INVERT_OVG_EXT);
            constexpr AdvancedColorBlendAttachmentState blendRed(VK_BLEND_OP_RED_EXT);
            constexpr AdvancedColorBlendAttachmentState blendGreen(VK_BLEND_OP_GREEN_EXT);
            constexpr AdvancedColorBlendAttachmentState blendBlue(VK_BLEND_OP_BLUE_EXT);
        } // namespace advanced
    } // namespace blendstates
} // namespace magma
