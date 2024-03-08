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
#include "pch.h"
#pragma hdrstop
#include "vertexInputState.h"
#include "inputAssemblyState.h"
#include "tesselationState.h"
#include "viewportState.h"
#include "rasterizationState.h"
#include "multisampleState.h"
#include "depthStencilState.h"
#include "colorBlendState.h"
#include "../helpers/streamInsertOperators.h"
#include "../helpers/stringifyFlags.h"

namespace magma
{
using namespace helpers;

std::ostream& operator<<(std::ostream& out, const VertexInputState& state)
{
    out << "VkPipelineVertexInputStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\tvertexBindingDescriptionCount: " << state.vertexBindingDescriptionCount << std::endl
        << "\tpVertexBindingDescriptions: " << std::endl;
    for (uint32_t i = 0; i < state.vertexBindingDescriptionCount; ++i)
    {
        auto const& vertexBinding = state.pVertexBindingDescriptions[i];
        out << "\t[" << std::endl
            << "\t\tbinding: " << vertexBinding.binding << std::endl
            << "\t\tstride: " << vertexBinding.stride << std::endl
            << "\t\tinputRate: " << vertexBinding.inputRate << std::endl
            << "\t]" << std::endl;
    }
    out << "\tvertexAttributeDescriptionCount: " << state.vertexAttributeDescriptionCount  << std::endl
        << "\tpVertexAttributeDescriptions: " << std::endl;
    for (uint32_t i = 0; i < state.vertexAttributeDescriptionCount; ++i)
    {
        auto const& vertexAttribute = state.pVertexAttributeDescriptions[i];
        out << "\t[" << std::endl
            << "\t\tlocation: " << vertexAttribute.location << std::endl
            << "\t\tbinding: " << vertexAttribute.binding << std::endl
            << "\t\tformat: " << vertexAttribute.format << std::endl
            << "\t\toffset: " << vertexAttribute.offset << std::endl
            << "\t]" << std::endl;
    }
    out << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const InputAssemblyState& state)
{
    out << "VkPipelineInputAssemblyStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\ttopology: " << state.topology << std::endl
        << "\tprimitiveRestartEnable: " << Bool(state.primitiveRestartEnable) << std::endl
        << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const TesselationState& state)
{
    out << "VkPipelineTessellationStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\tpatchControlPoints: " << state.patchControlPoints << std::endl
        << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const ViewportState& state)
{
    out << "VkPipelineViewportStateCreateInfo [" << std::endl
        << "\tviewportCount: " << state.viewportCount << std::endl
        << "\tpViewports: ";
    if (!state.pViewports)
        out << "NULL" << std::endl;
    else
    {
        out << std::endl;
        for (uint32_t i = 0; i < state.viewportCount; ++i)
        {
            const VkViewport& vp = state.pViewports[i];
            out << "\t[" << std::endl
                << "\t\tx: " << vp.x << std::endl
                << "\t\ty: " << vp.y << std::endl
                << "\t\twidth: " << vp.width << std::endl
                << "\t\theight: " << vp.height << std::endl
                << "\t\tminDepth: " << vp.minDepth << std::endl
                << "\t\tmaxDepth: " << vp.maxDepth << std::endl
                << "\t]" << std::endl;
        }
    }
    out << "\tscissorCount: " << state.scissorCount << std::endl
        << "\tpScissors: ";
    if (!state.pScissors)
        out << "NULL" << std::endl;
    else
    {
        out << std::endl;
        for (uint32_t i = 0; i < state.scissorCount; ++i)
        {
            const VkRect2D& scissor = state.pScissors[i];
            out << "\t[" << std::endl
                << "\t\toffset: [" << scissor.offset.x << ", " << scissor.offset.y << "]" << std::endl
                << "\t\textent: [" << scissor.extent.width << ", " << scissor.extent.height << "]" << std::endl
                << "\t]" << std::endl;
        }
    }
    out << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const RasterizationState& state)
{
    out << "VkPipelineRasterizationStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\tdepthClampEnable: " << Bool(state.depthClampEnable) << std::endl
        << "\trasterizerDiscardEnable: " << Bool(state.rasterizerDiscardEnable) << std::endl
        << "\tpolygonMode: " << state.polygonMode << std::endl
        << "\tcullMode: " << VkCullModeFlagBits(state.cullMode) << std::endl
        << "\tfrontFace: " << state.frontFace << std::endl
        << "\tdepthBiasEnable: " << state.depthBiasEnable << std::endl
        << "\tdepthBiasConstantFactor: " << state.depthBiasConstantFactor << std::endl
        << "\tdepthBiasClamp: " << state.depthBiasClamp << std::endl
        << "\tdepthBiasSlopeFactor: " << state.depthBiasSlopeFactor << std::endl
        << "\tlineWidth: " << state.lineWidth << std::endl
        << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const MultisampleState& state)
{
    out << "VkPipelineMultisampleStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\trasterizationSamples: " << state.rasterizationSamples << std::endl
        << "\tsampleShadingEnable: " << Bool(state.sampleShadingEnable) << std::endl
        << "\tminSampleShading: " << state.minSampleShading << std::endl
        << "\tsampleMask: ";
        if (!state.pSampleMask)
            out << "NULL";
        else for (int i = state.rasterizationSamples - 1; i >= 0; --i)
            out << (*state.pSampleMask & (1 << i) ? 1 : 0);
        out << std::endl << "\talphaToCoverageEnable: " << Bool(state.alphaToCoverageEnable) << std::endl
        << "\talphaToOneEnable: " << Bool(state.alphaToOneEnable) << std::endl
        << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const DepthStencilState& state)
{
    out << "VkPipelineDepthStencilStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\tdepthTestEnable: " << Bool(state.depthTestEnable) << std::endl
        << "\tdepthWriteEnable: " << Bool(state.depthWriteEnable) << std::endl
        << "\tdepthCompareOp: " << state.depthCompareOp << std::endl
        << "\tdepthBoundsTestEnable " << Bool(state.depthBoundsTestEnable)  << std::endl
        << "\tstencilTestEnable: " << Bool(state.stencilTestEnable) << std::endl
        << "\tfront: [" << std::endl
        << "\t\tfailOp: " << state.front.failOp << std::endl
        << "\t\tpassOp: " << state.front.passOp << std::endl
        << "\t\tdepthFailOp: " << state.front.depthFailOp << std::endl
        << "\t\tcompareOp: " << state.front.compareOp << std::endl
        << "\t\tcompareMask: " << stringifyMask(state.front.compareMask) << std::endl
        << "\t\twriteMask: " << stringifyMask(state.front.writeMask) << std::endl
        << "\t\treference: " << stringifyMask(state.front.reference) << std::endl
        << "\t]" << std::endl
        << "\tback: [" << std::endl
        << "\t\tfailOp: " << state.back.failOp << std::endl
        << "\t\tpassOp: " << state.back.passOp << std::endl
        << "\t\tdepthFailOp: " << state.back.depthFailOp << std::endl
        << "\t\tcompareOp: " << state.back.compareOp << std::endl
        << "\t\tcompareMask: " << stringifyMask(state.back.compareMask) << std::endl
        << "\t\twriteMask: " << stringifyMask(state.back.writeMask) << std::endl
        << "\t\treference: " << stringifyMask(state.back.reference) << std::endl
        << "\t]" << std::endl
        << "\tminDepthBounds: " << state.minDepthBounds << std::endl
        << "\tmaxDepthBounds: " << state.maxDepthBounds << std::endl
        << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const ColorBlendState& state)
{
    out << "VkPipelineColorBlendStateCreateInfo [" << std::endl
        << "\tflags: " << state.flags << std::endl
        << "\tlogicOpEnable: " << Bool(state.logicOpEnable) << std::endl
        << "\tlogicOp: " << state.logicOp << std::endl
        << "\tattachmentCount: " << state.attachmentCount << std::endl
        << "\tpAttachments:" << std::endl;
    for (uint32_t i = 0; i < state.attachmentCount; ++i)
    {
        auto const& attachment = state.pAttachments[i];
        out << "\t[" << std::endl
            << "\t\tblendEnable: " << Bool(attachment.blendEnable) << std::endl
            << "\t\tsrcColorBlendFactor: " << attachment.srcColorBlendFactor << std::endl
            << "\t\tdstColorBlendFactor: " << attachment.dstColorBlendFactor << std::endl
            << "\t\tcolorBlendOp: " << attachment.colorBlendOp << std::endl
            << "\t\tsrcAlphaBlendFactor: " << attachment.srcAlphaBlendFactor << std::endl
            << "\t\tdstAlphaBlendFactor: " << attachment.dstAlphaBlendFactor << std::endl
            << "\t\talphaBlendOp: " << attachment.alphaBlendOp << std::endl
            << "\t\tcolorWriteMask: " << stringifyColorMask(attachment.colorWriteMask) << std::endl
            << "\t]" << std::endl;
    }
    out << "\tblendConstants: ["
        << state.blendConstants[0] << ", "
        << state.blendConstants[1] << ", "
        << state.blendConstants[2] << ", "
        << state.blendConstants[3] << "]" << std::endl
        << "]";
    return out;
}
} // namespace magma
