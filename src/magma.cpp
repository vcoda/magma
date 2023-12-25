/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
//#define MAGMA_VERIFY_CONSTEXPR
#if defined(MAGMA_VERIFY_CONSTEXPR)
#include "../magma.h"
#endif

namespace magma
{
#ifdef MAGMA_VERIFY_CONSTEXPR
    // Attachment description
    constexpr AttachmentDescription rgbaUnormAttachment(VK_FORMAT_R8G8B8A8_UNORM, 4, op::load, op::store,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    constexpr hash_t attachmentHash = rgbaUnormAttachment.hash();
    // Subpass dependency
    constexpr SubpassExternalDependency subpassDependency(VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
    constexpr hash_t subpassDependencyHash = subpassDependency.hash();
    // Input assembly state
    constexpr hash_t triangleListHash = renderstate::triangleList.hash();
    // Tesselation state
    constexpr TesselationState eightControlPointsPatch(8);
    constexpr hash_t eightControlPointsPatchHash = eightControlPointsPatch.hash();
    // Rasterization state
    constexpr hash_t fillCullBackCcwHash = renderstate::fillCullBackCcw.hash();
    // Depth/stencil states
    constexpr hash_t depthLessOrEqualHash = renderstate::depthLessOrEqual.hash();
    constexpr hash_t stencilOneDepthPassHash = renderstate::stencilOneDepthPass.hash();
    // Blend states
    constexpr hash_t writeNoneHash = blendstate::writeNone.hash();
    constexpr hash_t dontWriteRgbaHash = renderstate::dontWriteRgba.hash();
    // Sampler state
    constexpr hash_t magMinMipLinearRepeatHash = sampler::magMinMipLinearRepeat.hash();
    // Depth comparison sampler state
    constexpr hash_t magMinNearestLessOrEqualHash = sampler::magMinNearestLessOrEqual.hash();
    // Multisample state
    constexpr hash_t multisampleFourHash = renderstate::multisampleFour.hash();
    // Extended states
#ifdef VK_AMD_rasterization_order
    constexpr hash_t fillCullNoneCcwStrictHash = renderstate::fillCullNoneCcwStrict.hash();
#endif
#ifdef VK_EXT_conservative_rasterization
    constexpr hash_t fillCullNoneCcwOverestimateHash = renderstate::fillCullNoneCcwOverestimate.hash();
#endif
#ifdef VK_EXT_line_rasterization
    constexpr hash_t stippledLineBresenhamPatternAx2Hash = renderstate::stippledLineBresenhamPatternAx2.hash();
#endif
#ifdef VK_EXT_provoking_vertex
    constexpr hash_t fillCullBackCcwProvokingFirstHash = renderstate::fillCullBackCcwProvokingFirst.hash();
#endif
#ifdef VK_EXT_transform_feedback
    constexpr hash_t fillCullNoneCcwStream1Hash = renderstate::fillCullNoneCcwStream1.hash();
#endif
#ifdef VK_EXT_blend_operation_advanced
    constexpr hash_t blendOverlayRgbaHash = blendstate::advanced::blendOverlayRgba.hash();
#endif
#ifdef VK_EXT_sampler_filter_minmax
    constexpr hash_t magMinMipLinearRepeatWeightedAverageHash = sampler::magMinMipLinearRepeatWeightedAverage.hash();
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
    constexpr hash_t rgbIdentityFullNearestHash = sampler::rgbIdentityFullNearest.hash();
#endif
    // Custom border color
    constexpr VkClearColorValue redColor{1.f, 0.f, 0.f, 1.f};
    constexpr BorderColor customBorderColor(redColor);
    constexpr hash_t customBorderColorHash = customBorderColor.hash();
    // Push constant
    struct PushConstants
    {
        float m[16];
    };
    constexpr pushconstant::VertexFragmentConstantRange<PushConstants> pushConstant;
    constexpr hash_t pushConstantHash = pushConstant.hash();

    // Mipmap filters
    static_assert(sampler::magMinMipNearestRepeat.getMipFilter() == MipFilter::Nearest, "expected nearest filter");
    static_assert(sampler::magMinLinearMipNearestRepeat.getMipFilter() == MipFilter::Bilinear, "expected bilinear filter");
    static_assert(sampler::magMinMipLinearRepeat.getMipFilter() == MipFilter::Trilinear, "expected trilinear filter");
    static_assert(sampler::magMinLinearMipAnisotropicRepeat.getMipFilter() == MipFilter::Anisotropic, "expected anisotropic filter");
    static_assert(sampler::magLinearMinNearestMipLinearRepeat.getMipFilter() == MipFilter::Partial, "expected mixed filter");

    // Numeric formats
    static_assert(UByteNorm::numeric() == Numeric::UNorm, "expected UNORM format");
    static_assert(ByteNorm::numeric() == Numeric::SNorm, "expected SNORM format");
    static_assert(UByte::numeric() == Numeric::UInt, "expected UINT format");
    static_assert(Byte::numeric() == Numeric::SInt, "expected SINT format");
    static_assert(UShortNorm::numeric() == Numeric::UNorm, "expected UNORM format");
    static_assert(ShortNorm::numeric() == Numeric::SNorm, "expected SNORM format");
    static_assert(UShort::numeric() == Numeric::UInt, "expected UINT format");
    static_assert(Short::numeric() == Numeric::SInt, "expected SINT format");
    static_assert(Half::numeric() == Numeric::SFloat, "expected SFLOAT format");
    static_assert(UInt::numeric() == Numeric::UInt, "expected UINT format");
    static_assert(Int::numeric() == Numeric::SInt, "expected SINT format");
    static_assert(Float::numeric() == Numeric::SFloat, "expected SFLOAT format");
    static_assert(LargeUInt::numeric() == Numeric::UInt, "expected UINT format");
    static_assert(LargeInt::numeric() == Numeric::SInt, "expected SINT format");
    static_assert(Double::numeric() == Numeric::SFloat, "expected SFLOAT format");
#endif // MAGMA_VERIFY_CONSTEXPR
} // namespace magma
