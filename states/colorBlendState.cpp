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
    VkColorComponentFlags colorWriteMask /* R, G, B, A */) noexcept
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
    pAttachments = helpers::copyArray(static_cast<const VkPipelineColorBlendAttachmentState *>(&attachment), 1);
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
    pAttachments = helpers::copyArray(static_cast<const VkPipelineColorBlendAttachmentState *>(attachments.data()), attachments.size());
    blendConstants[0] = 1.f;
    blendConstants[1] = 1.f;
    blendConstants[2] = 1.f;
    blendConstants[3] = 1.f;
}

ColorBlendState::ColorBlendState(const ColorBlendState& other)
{
    helpers::copy(this, &other);
    pAttachments = helpers::copyArray(other.pAttachments, attachmentCount);
}

ColorBlendState& ColorBlendState::operator=(const ColorBlendState& other)
{
    if (this != &other)
    {
        helpers::copy(this, &other);
        pAttachments = helpers::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

ColorBlendState::~ColorBlendState()
{
    delete[] pAttachments;
}

namespace blendstates
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
} // namespace blendstates

namespace states
{
const ColorBlendState dontBlendWriteR(blendstates::passR);
const ColorBlendState dontBlendWriteRG(blendstates::passRG);
const ColorBlendState dontBlendWriteRGB(blendstates::passRGB);
const ColorBlendState dontBlendWriteRGBA(blendstates::passRGBA);

const ColorBlendState blendAddWriteR(blendstates::addR);
const ColorBlendState blendAddWriteRG(blendstates::addRG);
const ColorBlendState blendAddWriteRGB(blendstates::addRGB);
const ColorBlendState blendAddWriteRGBA(blendstates::addRGBA);

const ColorBlendState blendSubtractWriteR(blendstates::subtractR);
const ColorBlendState blendSubtractWriteRG(blendstates::subtractRG);
const ColorBlendState blendSubractWriteRGB(blendstates::subtractRGB);
const ColorBlendState blendSubractWriteRGBA(blendstates::subtractRGBA);

const ColorBlendState blendReverseSubtractWriteR(blendstates::reverseSubtractR);
const ColorBlendState blendReverseSubtractWriteRG(blendstates::reverseSubtractRG);
const ColorBlendState blendReverseSubractWriteRGB(blendstates::reverseSubtractRGB);
const ColorBlendState blendReverseSubractWriteRGBA(blendstates::reverseSubtractRGBA);

const ColorBlendState blendMinWriteR(blendstates::minR);
const ColorBlendState blendMinWriteRG(blendstates::minRG);
const ColorBlendState blendMinWriteRGB(blendstates::minRGB);
const ColorBlendState blendMinWriteRGBA(blendstates::minRGBA);

const ColorBlendState blendMaxWriteR(blendstates::maxR);
const ColorBlendState blendMaxWriteRG(blendstates::maxRG);
const ColorBlendState blendMaxWriteRGB(blendstates::maxRGB);
const ColorBlendState blendMaxWriteRGBA(blendstates::maxRGBA);

const ColorBlendState blendNormalWriteR(blendstates::normalR);
const ColorBlendState blendNormalWriteRG(blendstates::normalRG);
const ColorBlendState blendNormalWriteRGB(blendstates::normalRGB);
const ColorBlendState blendNormalWriteRGBA(blendstates::normalRGBA);

const ColorBlendState logicClearWriteR(blendstates::passR, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteR(blendstates::passR, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteR(blendstates::passR, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteR(blendstates::passR, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteR(blendstates::passR, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteR(blendstates::passR, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteR(blendstates::passR, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteR(blendstates::passR, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteR(blendstates::passR, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteR(blendstates::passR, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertR(blendstates::passR, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteR(blendstates::passR, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteR(blendstates::passR, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteR(blendstates::passR, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteR(blendstates::passR, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteR(blendstates::passR, true, VK_LOGIC_OP_SET);

const ColorBlendState logicClearWriteRG(blendstates::passRG, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteRG(blendstates::passRG, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteRG(blendstates::passRG, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteRG(blendstates::passRG, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteRG(blendstates::passRG, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteRG(blendstates::passRG, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteRG(blendstates::passRG, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteRG(blendstates::passRG, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteRG(blendstates::passRG, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteRG(blendstates::passRG, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertRG(blendstates::passRG, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteRG(blendstates::passRG, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteRG(blendstates::passRG, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteRG(blendstates::passRG, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteRG(blendstates::passRG, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteRG(blendstates::passRG, true, VK_LOGIC_OP_SET);

const ColorBlendState logicClearWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertRGB(blendstates::passRGB, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteRGB(blendstates::passRGB, true, VK_LOGIC_OP_SET);

const ColorBlendState logicClearWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_CLEAR);
const ColorBlendState logicAndWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_AND);
const ColorBlendState logicReverseAndWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_AND_REVERSE);
const ColorBlendState logicCopyWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_COPY);
const ColorBlendState logicInvertedAndWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_AND_INVERTED);
const ColorBlendState logicNoOpWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_NO_OP);
const ColorBlendState logicXorWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_XOR);
const ColorBlendState logicOrWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_OR);
const ColorBlendState logicNorWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_NOR);
const ColorBlendState logicEquivalentWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_EQUIVALENT);
const ColorBlendState logicInvertRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_INVERT);
const ColorBlendState logicReverseOrWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_OR_REVERSE);
const ColorBlendState logicInvertedCopyWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_COPY_INVERTED);
const ColorBlendState logicInvertedOrWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_OR_INVERTED);
const ColorBlendState logicNandWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_NAND);
const ColorBlendState logicSetWriteRGBA(blendstates::passRGBA, true, VK_LOGIC_OP_SET);
} // namespace states
} // namespace magma
