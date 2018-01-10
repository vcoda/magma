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
#include "colorBlendState.h"

namespace magma
{
ColorBlendAttachmentState::ColorBlendAttachmentState(bool blendEnable, 
    VkBlendFactor srcBlendFactor, VkBlendFactor dstBlendFactor, VkBlendOp blendOp,
    VkColorComponentFlags colorWriteMask /* R, G, B, A */)
{
    state.blendEnable = MAGMA_BOOLEAN(blendEnable);
    state.srcColorBlendFactor = srcBlendFactor;
    state.dstColorBlendFactor = dstBlendFactor;
    state.colorBlendOp = blendOp;
    state.srcAlphaBlendFactor = srcBlendFactor;
    state.dstAlphaBlendFactor = dstBlendFactor;
    state.alphaBlendOp = blendOp;
    state.colorWriteMask = colorWriteMask;
}

ColorBlendState::ColorBlendState(const ColorBlendAttachmentState& attachment):
    ColorBlendState(std::vector<ColorBlendAttachmentState>{attachment})
{
}

ColorBlendState::ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments)
{
    MAGMA_ASSERT(!attachments.empty());
    state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    state.pNext = nullptr;
    state.flags = 0;
    state.logicOpEnable = VK_FALSE;
    state.logicOp = VK_LOGIC_OP_CLEAR;
    state.attachmentCount = MAGMA_COUNT(attachments);
    state.pAttachments = new VkPipelineColorBlendAttachmentState[state.attachmentCount];
    MAGMA_COPY_VECTOR(state.pAttachments, attachments);
    state.blendConstants[0] = 1.f;
    state.blendConstants[1] = 1.f;
    state.blendConstants[2] = 1.f;
    state.blendConstants[3] = 1.f;
}

ColorBlendState::~ColorBlendState()
{
    delete[] state.pAttachments;
}

namespace blends
{
const ColorBlendAttachmentState passR(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState passRG(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState passRGB(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState passRGBA(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState addR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState addRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState addRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState addRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState normalR(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState normalRG(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState normalRGB(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState normalRGBA(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
} // namespace blends

namespace states
{
using namespace blends;
const ColorBlendState dontBlendWriteR(passR);
const ColorBlendState dontBlendWriteRG(passRG);
const ColorBlendState dontBlendWriteRGB(passRGB);
const ColorBlendState dontBlendWriteRGBA(passRGBA);

const ColorBlendState blendAddWriteR(addR);
const ColorBlendState blendAddWriteRG(addRG);
const ColorBlendState blendAddWriteRGB(addRGB);
const ColorBlendState blendAddWriteRGBA(addRGBA);

const ColorBlendState blendNormalWriteR(normalR);
const ColorBlendState blendNormalWriteRG(normalRG);
const ColorBlendState blendNormalWriteRGB(normalRGB);
const ColorBlendState blendNormalWriteRGBA(normalRGBA);
} // namespace states
} // namespace magma
