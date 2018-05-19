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
#include "../helpers/copy.h"
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

ColorBlendState::ColorBlendState(const ColorBlendAttachmentState& attachment,
    bool logicOpEnable /* false */,
    VkLogicOp logicOp /* VK_LOGIC_OP_CLEAR */)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = MAGMA_BOOLEAN(logicOpEnable);
    this->logicOp = logicOp;
    attachmentCount = 1;
    pAttachments = helpers::copy(new VkPipelineColorBlendAttachmentState[1], static_cast<const VkPipelineColorBlendAttachmentState *>(&attachment));
    blendConstants[0] = 1.f;
    blendConstants[1] = 1.f;
    blendConstants[2] = 1.f;
    blendConstants[3] = 1.f;
}

ColorBlendState::ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    bool logicOpEnable /* false */,
    VkLogicOp logicOp /* VK_LOGIC_OP_CLEAR */)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = MAGMA_BOOLEAN(logicOpEnable);
    this->logicOp = logicOp;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = helpers::copy(new VkPipelineColorBlendAttachmentState[attachmentCount], attachments);
    blendConstants[0] = 1.f;
    blendConstants[1] = 1.f;
    blendConstants[2] = 1.f;
    blendConstants[3] = 1.f;
}

ColorBlendState::ColorBlendState(const ColorBlendState& other)
{
    helpers::copy(this, &other);
    pAttachments = helpers::copy(new VkPipelineColorBlendAttachmentState[attachmentCount], other.pAttachments, attachmentCount);
}

ColorBlendState& ColorBlendState::operator=(const ColorBlendState& other)
{
    if (this != &other)
    {
        helpers::copy(this, &other);
        pAttachments = helpers::copy(new VkPipelineColorBlendAttachmentState[attachmentCount], other.pAttachments, attachmentCount);
    }
    return *this;
}

ColorBlendState::~ColorBlendState()
{
    delete[] pAttachments;
}

namespace colorblends
{
const ColorBlendAttachmentState passR(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState passRG(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState passRGB(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState passRGBA(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState addR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState addRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState addRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState addRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState subtractR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState subtractRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState subtractRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState subtractRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState reverseSubtractR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState reverseSubtractRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState reverseSubtractRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState reverseSubtractRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState minR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState minRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState minRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState minRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState maxR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState maxRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState maxRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState maxRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState normalR(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT);
const ColorBlendAttachmentState normalRG(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const ColorBlendAttachmentState normalRGB(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const ColorBlendAttachmentState normalRGBA(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
} // namespace colorblends

namespace states
{
const ColorBlendState dontBlendWriteR(colorblends::passR);
const ColorBlendState dontBlendWriteRG(colorblends::passRG);
const ColorBlendState dontBlendWriteRGB(colorblends::passRGB);
const ColorBlendState dontBlendWriteRGBA(colorblends::passRGBA);

const ColorBlendState blendAddWriteR(colorblends::addR);
const ColorBlendState blendAddWriteRG(colorblends::addRG);
const ColorBlendState blendAddWriteRGB(colorblends::addRGB);
const ColorBlendState blendAddWriteRGBA(colorblends::addRGBA);

const ColorBlendState blendSubtractWriteR(colorblends::subtractR);
const ColorBlendState blendSubtractWriteRG(colorblends::subtractRG);
const ColorBlendState blendSubractWriteRGB(colorblends::subtractRGB);
const ColorBlendState blendSubractWriteRGBA(colorblends::subtractRGBA);

const ColorBlendState blendReverseSubtractWriteR(colorblends::reverseSubtractR);
const ColorBlendState blendReverseSubtractWriteRG(colorblends::reverseSubtractRG);
const ColorBlendState blendReverseSubractWriteRGB(colorblends::reverseSubtractRGB);
const ColorBlendState blendReverseSubractWriteRGBA(colorblends::reverseSubtractRGBA);

const ColorBlendState blendMinWriteR(colorblends::minR);
const ColorBlendState blendMinWriteRG(colorblends::minRG);
const ColorBlendState blendMinWriteRGB(colorblends::minRGB);
const ColorBlendState blendMinWriteRGBA(colorblends::minRGBA);

const ColorBlendState blendMaxWriteR(colorblends::maxR);
const ColorBlendState blendMaxWriteRG(colorblends::maxRG);
const ColorBlendState blendMaxWriteRGB(colorblends::maxRGB);
const ColorBlendState blendMaxWriteRGBA(colorblends::maxRGBA);

const ColorBlendState blendNormalWriteR(colorblends::normalR);
const ColorBlendState blendNormalWriteRG(colorblends::normalRG);
const ColorBlendState blendNormalWriteRGB(colorblends::normalRGB);
const ColorBlendState blendNormalWriteRGBA(colorblends::normalRGBA);

const ColorBlendState logicClearWriteR(colorblends::passR, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteR(colorblends::passR, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteR(colorblends::passR, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteR(colorblends::passR, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteR(colorblends::passR, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteR(colorblends::passR, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteR(colorblends::passR, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteR(colorblends::passR, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteR(colorblends::passR, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteR(colorblends::passR, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertR(colorblends::passR, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteR(colorblends::passR, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteR(colorblends::passR, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteR(colorblends::passR, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteR(colorblends::passR, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteR(colorblends::passR, true, VK_LOGIC_OP_SET);

const ColorBlendState logicClearWriteRG(colorblends::passRG, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteRG(colorblends::passRG, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteRG(colorblends::passRG, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteRG(colorblends::passRG, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteRG(colorblends::passRG, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteRG(colorblends::passRG, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteRG(colorblends::passRG, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteRG(colorblends::passRG, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteRG(colorblends::passRG, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteRG(colorblends::passRG, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertRG(colorblends::passRG, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteRG(colorblends::passRG, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteRG(colorblends::passRG, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteRG(colorblends::passRG, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteRG(colorblends::passRG, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteRG(colorblends::passRG, true, VK_LOGIC_OP_SET);

const ColorBlendState logicClearWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertRGB(colorblends::passRGB, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteRGB(colorblends::passRGB, true, VK_LOGIC_OP_SET);

const ColorBlendState logicClearWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteRGBA(colorblends::passRGBA, true, VK_LOGIC_OP_SET);
} // namespace states
} // namespace magma
