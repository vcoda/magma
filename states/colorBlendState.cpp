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
#include "../shared.h"

namespace magma
{
ColorBlendAttachmentState::ColorBlendAttachmentState(bool blendEnable, 
    VkBlendFactor srcBlendFactor, VkBlendFactor dstBlendFactor, VkBlendOp blendOp,
    VkColorComponentFlags colorWriteMask /* R, G, B, A */)
{
    this->blendEnable = MAGMA_BOOLEAN(blendEnable);
    srcColorBlendFactor = srcBlendFactor;
    dstColorBlendFactor = dstBlendFactor;
    colorBlendOp = blendOp;
    srcAlphaBlendFactor = srcBlendFactor;
    dstAlphaBlendFactor = dstBlendFactor;
    alphaBlendOp = blendOp;
    this->colorWriteMask = colorWriteMask;
}

ColorBlendState::ColorBlendState(const ColorBlendAttachmentState& attachment):
    ColorBlendState(std::vector<ColorBlendAttachmentState>{attachment})
{}

ColorBlendState::ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    logicOpEnable = VK_FALSE;
    logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = copy(new VkPipelineColorBlendAttachmentState[attachmentCount], attachments);
    blendConstants[0] = 1.f;
    blendConstants[1] = 1.f;
    blendConstants[2] = 1.f;
    blendConstants[3] = 1.f;
}

ColorBlendState::ColorBlendState(const ColorBlendState& other)
{
    copy(this, &other);
    pAttachments = copy(new VkPipelineColorBlendAttachmentState[attachmentCount], other.pAttachments, attachmentCount);
}

ColorBlendState::~ColorBlendState()
{
    delete[] pAttachments;
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
