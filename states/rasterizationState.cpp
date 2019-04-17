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
#include "rasterizationState.h"
#include "../internal/hash.h"

namespace magma
{
RasterizationState::RasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace,
    bool depthClampEnable /* false */, bool rasterizerDiscardEnable /* false */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->depthClampEnable = MAGMA_BOOLEAN(depthClampEnable);
    this->rasterizerDiscardEnable = MAGMA_BOOLEAN(rasterizerDiscardEnable);
    this->polygonMode = polygonMode;
    this->cullMode = cullMode;
    this->frontFace = frontFace;
    depthBiasEnable = VK_FALSE;
    depthBiasConstantFactor = 0.f;
    depthBiasClamp = 0.f;
    depthBiasSlopeFactor = 0.f;
    lineWidth = 1.f;
}

size_t RasterizationState::hash() const noexcept
{
    return internal::hashArgs(
        flags,
        depthClampEnable,
        rasterizerDiscardEnable,
        polygonMode,
        cullMode,
        frontFace,
        depthBiasEnable,
        depthBiasConstantFactor,
        depthBiasClamp,
        depthBiasSlopeFactor,
        lineWidth);
}

bool RasterizationState::operator==(const RasterizationState& other) const noexcept
{
    return (flags == other.flags) &&
        (depthClampEnable == other.depthClampEnable) &&
        (rasterizerDiscardEnable == other.rasterizerDiscardEnable) &&
        (polygonMode == other.polygonMode) &&
        (cullMode == other.cullMode) &&
        (frontFace == other.frontFace) &&
        (depthBiasEnable == other.depthBiasEnable) &&
        (depthBiasConstantFactor == other.depthBiasConstantFactor) &&
        (depthBiasClamp == other.depthBiasClamp) &&
        (depthBiasSlopeFactor == other.depthBiasSlopeFactor) &&
        (lineWidth == other.lineWidth);
}

DepthBiasRasterizationState::DepthBiasRasterizationState(const RasterizationState& state,
    float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    this->depthBiasEnable = VK_TRUE;
    this->depthBiasConstantFactor = depthBiasConstantFactor;
    this->depthBiasClamp = depthBiasClamp;
    this->depthBiasSlopeFactor = depthBiasSlopeFactor;
}

ConservativeRasterizationState::ConservativeRasterizationState(const RasterizationState& state,
    VkConservativeRasterizationModeEXT conservativeRasterizationMode,
    float extraPrimitiveOverestimationSize /* 0 */) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    conservative.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT;
    conservative.pNext = nullptr;
    conservative.flags = 0;
    conservative.conservativeRasterizationMode = conservativeRasterizationMode;
    conservative.extraPrimitiveOverestimationSize = extraPrimitiveOverestimationSize;
    pNext = &conservative;
}

size_t ConservativeRasterizationState::hash() const noexcept
{
    size_t hash = internal::hashArgs(
        conservative.flags,
        conservative.conservativeRasterizationMode,
        conservative.extraPrimitiveOverestimationSize);
    internal::hashCombine(hash, RasterizationState::hash());
    return hash;
}

bool ConservativeRasterizationState::operator==(const ConservativeRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (conservative.flags == other.conservative.flags) &&
        (conservative.conservativeRasterizationMode == other.conservative.conservativeRasterizationMode) &&
        (conservative.extraPrimitiveOverestimationSize == other.conservative.extraPrimitiveOverestimationSize);
}

RasterizationOrderState::RasterizationOrderState(const RasterizationState& state,
    VkRasterizationOrderAMD rasterizationOrder) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    order.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD;
    order.pNext = nullptr;
    order.rasterizationOrder = rasterizationOrder;
    pNext = &order;
}

size_t RasterizationOrderState::hash() const noexcept
{
    std::hash<VkRasterizationOrderAMD> hasher;
    size_t hash = hasher(order.rasterizationOrder);
    internal::hashCombine(hash, RasterizationState::hash());
    return hash;
}

bool RasterizationOrderState::operator==(const RasterizationOrderState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (order.rasterizationOrder == other.order.rasterizationOrder);
}

namespace renderstates
{
const RasterizationState fillCullNoneCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState fillCullBackCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState fillCullFrontCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState fillCullFrontAndBackCCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

const RasterizationState lineCullNoneCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState lineCullBackCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState lineCullFrontCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState lineCullFrontAndBackCCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

const RasterizationState pointCullNoneCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState pointCullBackCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState pointCullFrontCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
const RasterizationState pointCullFrontAndBackCCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_COUNTER_CLOCKWISE);

const RasterizationState fillCullNoneCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState fillCullBackCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState fillCullFrontCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState fillCullFrontAndBackCW(VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

const RasterizationState lineCullNoneCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState lineCullBackCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState lineCullFrontCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState lineCullFrontAndBackCW(VK_POLYGON_MODE_LINE, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

const RasterizationState pointCullNoneCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState pointCullBackCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState pointCullFrontCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_BIT, VK_FRONT_FACE_CLOCKWISE);
const RasterizationState pointCullFrontAndBackCW(VK_POLYGON_MODE_POINT, VK_CULL_MODE_FRONT_AND_BACK, VK_FRONT_FACE_CLOCKWISE);

const ConservativeRasterizationState fillCullNoneCCWOverestimate(fillCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState fillCullBackCCWOverestimate(fillCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontCCWOverestimate(fillCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontAndBackCCWOverestimate(fillCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

const ConservativeRasterizationState lineCullNoneCCWOverestimate(lineCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState lineCullBackCCWOverestimate(lineCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontCCWOverestimate(lineCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontAndBackCCWOverestimate(lineCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

const ConservativeRasterizationState pointCullNoneCCWOverestimate(pointCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState pointCullBackCCWOverestimate(pointCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontCCWOverestimate(pointCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontAndBackCCWOverestimate(pointCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

const ConservativeRasterizationState fillCullNoneCWOverestimate(fillCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState fillCullBackCWOverestimate(fillCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontCWOverestimate(fillCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontAndBackCWOverestimate(fillCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

const ConservativeRasterizationState lineCullNoneCWOverestimate(lineCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState lineCullBackCWOverestimate(lineCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontCWOverestimate(lineCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontAndBackCWOverestimate(lineCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

const ConservativeRasterizationState pointCullNoneCWOverestimate(pointCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState pointCullBackCWOverestimate(pointCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontCWOverestimate(pointCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontAndBackCWOverestimate(pointCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_OVERESTIMATE_EXT);

const ConservativeRasterizationState fillCullNoneCCWUnderestimate(fillCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState fillCullBackCCWUnderestimate(fillCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontCCWUnderestimate(fillCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontAndBackCCWUnderestimate(fillCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

const ConservativeRasterizationState lineCullNoneCCWUnderestimate(lineCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState lineCullBackCCWUnderestimate(lineCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontCCWUnderestimate(lineCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontAndBackCCWUnderestimate(lineCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

const ConservativeRasterizationState pointCullNoneCCWUnderestimate(pointCullNoneCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState pointCullBackCCWUnderestimate(pointCullBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontCCWUnderestimate(pointCullFrontCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontAndBackCCWUnderestimate(pointCullFrontAndBackCCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

const ConservativeRasterizationState fillCullNoneCWUnderestimate(fillCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState fillCullBackCWUnderestimate(fillCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontCWUnderestimate(fillCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState fillCullFrontAndBackCWUnderestimate(fillCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

const ConservativeRasterizationState lineCullNoneCWUnderestimate(lineCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState lineCullBackCWUnderestimate(lineCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontCWUnderestimate(lineCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState lineCullFrontAndBackCWUnderestimate(lineCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

const ConservativeRasterizationState pointCullNoneCWUnderestimate(pointCullNoneCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState pointCullBackCWUnderestimate(pointCullBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontCWUnderestimate(pointCullFrontCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);
const ConservativeRasterizationState pointCullFrontAndBackCWUnderestimate(pointCullFrontAndBackCW, VK_CONSERVATIVE_RASTERIZATION_MODE_UNDERESTIMATE_EXT);

const RasterizationOrderState fillCullNoneCCWRelaxed(fillCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState fillCullBackCCWRelaxed(fillCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState fillCullFrontCCWRelaxed(fillCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState fillCullFrontAndBackCCWRelaxed(fillCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

const RasterizationOrderState lineCullNoneCCWRelaxed(lineCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState lineCullBackCCWRelaxed(lineCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState lineCullFrontCCWRelaxed(lineCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState lineCullFrontAndBackCCWRelaxed(lineCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

const RasterizationOrderState pointCullNoneCCWRelaxed(pointCullNoneCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState pointCullBackCCWRelaxed(pointCullBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState pointCullFrontCCWRelaxed(pointCullFrontCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState pointCullFrontAndBackCCWRelaxed(pointCullFrontAndBackCCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

const RasterizationOrderState fillCullNoneCWRelaxed(fillCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState fillCullBackCWRelaxed(fillCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState fillCullFrontCWRelaxed(fillCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState fillCullFrontAndBackCWRelaxed(fillCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

const RasterizationOrderState lineCullNoneCWRelaxed(lineCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState lineCullBackCWRelaxed(lineCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState lineCullFrontCWRelaxed(lineCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState lineCullFrontAndBackCWRelaxed(lineCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);

const RasterizationOrderState pointCullNoneCWRelaxed(pointCullNoneCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState pointCullBackCWRelaxed(pointCullBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState pointCullFrontCWRelaxed(pointCullFrontCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
const RasterizationOrderState pointCullFrontAndBackCWRelaxed(pointCullFrontAndBackCW, VK_RASTERIZATION_ORDER_RELAXED_AMD);
} // namespace renderstates
} // namespace magma
