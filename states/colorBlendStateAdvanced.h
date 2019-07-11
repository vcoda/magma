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
        AdvancedColorBlendState(const std::initializer_list<AdvancedColorBlendAttachmentState>& attachments,
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
            constexpr AdvancedColorBlendAttachmentState blendZeroR(VK_BLEND_OP_ZERO_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcR(VK_BLEND_OP_SRC_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstR(VK_BLEND_OP_DST_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOverR(VK_BLEND_OP_SRC_OVER_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOverR(VK_BLEND_OP_DST_OVER_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcInR(VK_BLEND_OP_SRC_IN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstInR(VK_BLEND_OP_DST_IN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOutR(VK_BLEND_OP_SRC_OUT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOutR(VK_BLEND_OP_DST_OUT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcAtopR(VK_BLEND_OP_SRC_ATOP_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstAtopR(VK_BLEND_OP_DST_ATOP_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendXorR(VK_BLEND_OP_XOR_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMultiplyR(VK_BLEND_OP_MULTIPLY_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendScreenR(VK_BLEND_OP_SCREEN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendOverlayR(VK_BLEND_OP_OVERLAY_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDarkenR(VK_BLEND_OP_DARKEN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLightenR(VK_BLEND_OP_LIGHTEN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorDodgeR(VK_BLEND_OP_COLORDODGE_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorBurnR(VK_BLEND_OP_COLORBURN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardLightR(VK_BLEND_OP_HARDLIGHT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSoftLightR(VK_BLEND_OP_SOFTLIGHT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDifferenceR(VK_BLEND_OP_DIFFERENCE_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendExclusionR(VK_BLEND_OP_EXCLUSION_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertR(VK_BLEND_OP_INVERT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgbR(VK_BLEND_OP_INVERT_RGB_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearDodgeR(VK_BLEND_OP_LINEARDODGE_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearBurnR(VK_BLEND_OP_LINEARBURN_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendVividLightR(VK_BLEND_OP_VIVIDLIGHT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearLightR(VK_BLEND_OP_LINEARLIGHT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPinLightR(VK_BLEND_OP_PINLIGHT_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardMixR(VK_BLEND_OP_HARDMIX_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslHueR(VK_BLEND_OP_HSL_HUE_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslSaturationR(VK_BLEND_OP_HSL_SATURATION_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslColorR(VK_BLEND_OP_HSL_COLOR_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslLuminosityR(VK_BLEND_OP_HSL_LUMINOSITY_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusR(VK_BLEND_OP_PLUS_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedR(VK_BLEND_OP_PLUS_CLAMPED_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedAlphaR(VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusDarkenR(VK_BLEND_OP_PLUS_DARKER_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusR(VK_BLEND_OP_MINUS_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusClampedR(VK_BLEND_OP_MINUS_CLAMPED_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendContrastR(VK_BLEND_OP_CONTRAST_EXT, rWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertOvgR(VK_BLEND_OP_INVERT_OVG_EXT, rWriteMask);

            constexpr AdvancedColorBlendAttachmentState blendZeroRg(VK_BLEND_OP_ZERO_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcRg(VK_BLEND_OP_SRC_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstRg(VK_BLEND_OP_DST_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOverRg(VK_BLEND_OP_SRC_OVER_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOverRg(VK_BLEND_OP_DST_OVER_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcInRg(VK_BLEND_OP_SRC_IN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstInRg(VK_BLEND_OP_DST_IN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOutRg(VK_BLEND_OP_SRC_OUT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOutRg(VK_BLEND_OP_DST_OUT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcAtopRg(VK_BLEND_OP_SRC_ATOP_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstAtopRg(VK_BLEND_OP_DST_ATOP_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendXorRg(VK_BLEND_OP_XOR_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMultiplyRg(VK_BLEND_OP_MULTIPLY_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendScreenRg(VK_BLEND_OP_SCREEN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendOverlayRg(VK_BLEND_OP_OVERLAY_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDarkenRg(VK_BLEND_OP_DARKEN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLightenRg(VK_BLEND_OP_LIGHTEN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorDodgeRg(VK_BLEND_OP_COLORDODGE_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorBurnRg(VK_BLEND_OP_COLORBURN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardLightRg(VK_BLEND_OP_HARDLIGHT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSoftLightRg(VK_BLEND_OP_SOFTLIGHT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDifferenceRg(VK_BLEND_OP_DIFFERENCE_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendExclusionRg(VK_BLEND_OP_EXCLUSION_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRg(VK_BLEND_OP_INVERT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgbRg(VK_BLEND_OP_INVERT_RGB_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearDodgeRg(VK_BLEND_OP_LINEARDODGE_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearBurnRg(VK_BLEND_OP_LINEARBURN_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendVividLightRg(VK_BLEND_OP_VIVIDLIGHT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearLightRg(VK_BLEND_OP_LINEARLIGHT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPinLightRg(VK_BLEND_OP_PINLIGHT_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardMixRg(VK_BLEND_OP_HARDMIX_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslHueRg(VK_BLEND_OP_HSL_HUE_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslSaturationRg(VK_BLEND_OP_HSL_SATURATION_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslColorRg(VK_BLEND_OP_HSL_COLOR_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslLuminosityRg(VK_BLEND_OP_HSL_LUMINOSITY_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusRg(VK_BLEND_OP_PLUS_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedRg(VK_BLEND_OP_PLUS_CLAMPED_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedAlphaRg(VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusDarkenRg(VK_BLEND_OP_PLUS_DARKER_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusRg(VK_BLEND_OP_MINUS_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusClampedRg(VK_BLEND_OP_MINUS_CLAMPED_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendContrastRg(VK_BLEND_OP_CONTRAST_EXT, rgWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertOvgRg(VK_BLEND_OP_INVERT_OVG_EXT, rgWriteMask);

            constexpr AdvancedColorBlendAttachmentState blendZeroRgb(VK_BLEND_OP_ZERO_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcRgb(VK_BLEND_OP_SRC_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstRgb(VK_BLEND_OP_DST_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOverRgb(VK_BLEND_OP_SRC_OVER_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOverRgb(VK_BLEND_OP_DST_OVER_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcInRgb(VK_BLEND_OP_SRC_IN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstInRgb(VK_BLEND_OP_DST_IN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOutRgb(VK_BLEND_OP_SRC_OUT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOutRgb(VK_BLEND_OP_DST_OUT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcAtopRgb(VK_BLEND_OP_SRC_ATOP_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstAtopRgb(VK_BLEND_OP_DST_ATOP_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendXorRgb(VK_BLEND_OP_XOR_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMultiplyRgb(VK_BLEND_OP_MULTIPLY_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendScreenRgb(VK_BLEND_OP_SCREEN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendOverlayRgb(VK_BLEND_OP_OVERLAY_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDarkenRgb(VK_BLEND_OP_DARKEN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLightenRgb(VK_BLEND_OP_LIGHTEN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorDodgeRgb(VK_BLEND_OP_COLORDODGE_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorBurnRgb(VK_BLEND_OP_COLORBURN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardLightRgb(VK_BLEND_OP_HARDLIGHT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSoftLightRgb(VK_BLEND_OP_SOFTLIGHT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDifferenceRgb(VK_BLEND_OP_DIFFERENCE_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendExclusionRgb(VK_BLEND_OP_EXCLUSION_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgb(VK_BLEND_OP_INVERT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgbRgb(VK_BLEND_OP_INVERT_RGB_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearDodgeRgb(VK_BLEND_OP_LINEARDODGE_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearBurnRgb(VK_BLEND_OP_LINEARBURN_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendVividLightRgb(VK_BLEND_OP_VIVIDLIGHT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearLightRgb(VK_BLEND_OP_LINEARLIGHT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPinLightRgb(VK_BLEND_OP_PINLIGHT_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardMixRgb(VK_BLEND_OP_HARDMIX_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslHueRgb(VK_BLEND_OP_HSL_HUE_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslSaturationRgb(VK_BLEND_OP_HSL_SATURATION_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslColorRgb(VK_BLEND_OP_HSL_COLOR_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslLuminosityRgb(VK_BLEND_OP_HSL_LUMINOSITY_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusRgb(VK_BLEND_OP_PLUS_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedRgb(VK_BLEND_OP_PLUS_CLAMPED_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedAlphaRgb(VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusDarkenRgb(VK_BLEND_OP_PLUS_DARKER_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusRgb(VK_BLEND_OP_MINUS_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusClampedRgb(VK_BLEND_OP_MINUS_CLAMPED_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendContrastRgb(VK_BLEND_OP_CONTRAST_EXT, rgbWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertOvgRgb(VK_BLEND_OP_INVERT_OVG_EXT, rgbWriteMask);

            constexpr AdvancedColorBlendAttachmentState blendZeroRgba(VK_BLEND_OP_ZERO_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcRgba(VK_BLEND_OP_SRC_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstRgba(VK_BLEND_OP_DST_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOverRgba(VK_BLEND_OP_SRC_OVER_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOverRgba(VK_BLEND_OP_DST_OVER_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcInRgba(VK_BLEND_OP_SRC_IN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstInRgba(VK_BLEND_OP_DST_IN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcOutRgba(VK_BLEND_OP_SRC_OUT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstOutRgba(VK_BLEND_OP_DST_OUT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSrcAtopRgba(VK_BLEND_OP_SRC_ATOP_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDstAtopRgba(VK_BLEND_OP_DST_ATOP_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendXorRgba(VK_BLEND_OP_XOR_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMultiplyRgba(VK_BLEND_OP_MULTIPLY_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendScreenRgba(VK_BLEND_OP_SCREEN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendOverlayRgba(VK_BLEND_OP_OVERLAY_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDarkenRgba(VK_BLEND_OP_DARKEN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLightenRgba(VK_BLEND_OP_LIGHTEN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorDodgeRgba(VK_BLEND_OP_COLORDODGE_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendColorBurnRgba(VK_BLEND_OP_COLORBURN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardLightRgba(VK_BLEND_OP_HARDLIGHT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendSoftLightRgba(VK_BLEND_OP_SOFTLIGHT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendDifferenceRgba(VK_BLEND_OP_DIFFERENCE_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendExclusionRgba(VK_BLEND_OP_EXCLUSION_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgba(VK_BLEND_OP_INVERT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertRgbRgba(VK_BLEND_OP_INVERT_RGB_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearDodgeRgba(VK_BLEND_OP_LINEARDODGE_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearBurnRgba(VK_BLEND_OP_LINEARBURN_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendVividLightRgba(VK_BLEND_OP_VIVIDLIGHT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendLinearLightRgba(VK_BLEND_OP_LINEARLIGHT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPinLightRgba(VK_BLEND_OP_PINLIGHT_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHardMixRgba(VK_BLEND_OP_HARDMIX_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslHueRgba(VK_BLEND_OP_HSL_HUE_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslSaturationRgba(VK_BLEND_OP_HSL_SATURATION_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslColorRgba(VK_BLEND_OP_HSL_COLOR_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendHslLuminosityRgba(VK_BLEND_OP_HSL_LUMINOSITY_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusRgba(VK_BLEND_OP_PLUS_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedRgba(VK_BLEND_OP_PLUS_CLAMPED_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusClampedAlphaRgba(VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendPlusDarkenRgba(VK_BLEND_OP_PLUS_DARKER_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusRgba(VK_BLEND_OP_MINUS_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendMinusClampedRgba(VK_BLEND_OP_MINUS_CLAMPED_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendContrastRgba(VK_BLEND_OP_CONTRAST_EXT, rgbaWriteMask);
            constexpr AdvancedColorBlendAttachmentState blendInvertOvgRgba(VK_BLEND_OP_INVERT_OVG_EXT, rgbaWriteMask);

            constexpr AdvancedColorBlendAttachmentState blendRed(VK_BLEND_OP_RED_EXT); // (R,G,B,A) = (Rs', Gd, Bd, Ad)
            constexpr AdvancedColorBlendAttachmentState blendGreen(VK_BLEND_OP_GREEN_EXT); // (R,G,B,A) = (Rd, Gs', Bd, Ad)
            constexpr AdvancedColorBlendAttachmentState blendBlue(VK_BLEND_OP_BLUE_EXT); // (R,G,B,A) = (Rd, Gd, Bs', Ad)
        } // namespace advanced
    } // namespace blendstates
} // namespace magma
