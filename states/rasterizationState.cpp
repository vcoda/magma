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
#include "../utilities/hash.h"
#include "../shared.h"

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
    return utilities::hashVariadic(
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
    float extraPrimitiveOverestimationSize) noexcept:
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
    size_t hash = utilities::hashVariadic(
        conservative.flags,
        conservative.conservativeRasterizationMode,
        conservative.extraPrimitiveOverestimationSize);
    utilities::hashCombineArg(hash, RasterizationState::hash());
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
    size_t hash = 0;
    utilities::hashCombineArg(hash, order.rasterizationOrder);
    utilities::hashCombineArg(hash, RasterizationState::hash());
    return hash;
}

bool RasterizationOrderState::operator==(const RasterizationOrderState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (order.rasterizationOrder == other.order.rasterizationOrder);
}

namespace states
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
} // namespace states
} // namespace magma
