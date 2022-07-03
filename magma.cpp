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
#include "pch.h"
#pragma hdrstop

//#define MAGMA_VERIFY_CONSTEXPR
#ifdef MAGMA_VERIFY_CONSTEXPR
#include "descriptors/attachment.h"
#include "descriptors/subpass.h"
#include "states/inputAssemblyState.h"
#include "states/tesselationState.h"
#include "states/rasterizationState.h"
#include "states/conservativeRasterizationState.h"
#include "states/rasterizationOrderState.h"
#include "states/streamRasterizationState.h"
#include "states/depthStencilState.h"
#include "states/colorBlendState.h"
#include "states/colorBlendStateAdvanced.h"
#include "states/multisampleState.h"
#include "states/samplerState.h"
#include "states/depthComparisonSamplerState.h"
#include "states/minmaxSamplerState.h"
#include "states/ycbcrSamplerState.h"
#include "misc/borderColor.h"
#include "misc/pushConstantRange.h"
#endif // MAGMA_VERIFY_CONSTEXPR

namespace magma
{
#ifdef MAGMA_VERIFY_CONSTEXPR
    // Attachment description
    constexpr size_t loadStoreHash = op::loadStore.hash();
    constexpr AttachmentDescription rgbaUnormAttachment(VK_FORMAT_R8G8B8A8_UNORM, 4, op::load, op::store,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    constexpr size_t attachmentHash = rgbaUnormAttachment.hash();
    // Subpass dependency
    constexpr SubpassDependency subpassDependency(0, 1, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    constexpr size_t subpassDependencyHash = subpassDependency.hash();
    // Input assembly state
    constexpr size_t triangleListHash = renderstate::triangleList.hash();
    // Tesselation state
    constexpr TesselationState eightControlPointsPatch(8);
    constexpr size_t eightControlPointsPatchHash = eightControlPointsPatch.hash();
    // Rasterization state
    constexpr size_t fillCullBackCCWHash = renderstate::fillCullBackCCW.hash();
    // Depth/stencil states
    constexpr size_t depthLessOrEqualHash = renderstate::depthLessOrEqual.hash();
    constexpr size_t stencilOneDepthPassHash = renderstate::stencilOneDepthPass.hash();
    // Blend states
    constexpr size_t writeNoneHash = blendstate::writeNone.hash();
    constexpr size_t dontWriteRgbaHash = renderstate::dontWriteRgba.hash();
    // Sampler state
    constexpr size_t magMinMipLinearRepeatHash = sampler::magMinMipLinearRepeat.hash();
    // Depth comparison sampler state
    constexpr size_t magMinNearestLessOrEqualHash = sampler::magMinNearestLessOrEqual.hash();
    // Multisample state
    constexpr size_t multisampleFourHash = renderstate::multisampleFour.hash();
    // Extended states
#ifdef VK_AMD_rasterization_order
    constexpr size_t fillCullNoneStrictCCWHash = renderstate::fillCullNoneStrictCCW.hash();
#endif
#ifdef VK_EXT_conservative_rasterization
    constexpr size_t fillCullNoneOverestimateCCWHash = renderstate::fillCullNoneOverestimateCCW.hash();
#endif
#ifdef VK_EXT_transform_feedback
    constexpr StreamRasterizationState fillCullNoneCCWStreamOne(renderstate::fillCullNoneCCW, 1);
    constexpr size_t fillCullNoneCCWStreamOneHash = fillCullNoneCCWStreamOne.hash();
#endif
#ifdef VK_EXT_blend_operation_advanced
    constexpr size_t blendOverlayRgbaHash = blendstate::advanced::blendOverlayRgba.hash();
#endif
#ifdef VK_EXT_sampler_filter_minmax
    constexpr size_t magMinMipLinearRepeatWeightedAverageHash = sampler::magMinMipLinearRepeatWeightedAverage.hash();
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
    constexpr size_t rgbIdentityFullNearestHash = sampler::rgbIdentityFullNearest.hash();
#endif
    // Custom border color
    constexpr VkClearColorValue redColor{1.f, 0.f, 0.f, 1.f};
    constexpr BorderColor customBorderColor(redColor);
    constexpr size_t customBorderColorHash = customBorderColor.hash();
    // Push constant
    struct PushConstants
    {
        float m[16];
    };
    constexpr pushconstant::VertexFragmentConstantRange<PushConstants> pushConstant;
    constexpr size_t pushConstantHash = pushConstant.hash();
#endif // MAGMA_VERIFY_CONSTEXPR
} // namespace magma
