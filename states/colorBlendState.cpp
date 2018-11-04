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
#include "../utilities/copy.h"
#include "../utilities/hash.h"
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

size_t ColorBlendAttachmentState::hash() const noexcept
{
    return utilities::hashVariadic(
        blendEnable,
        srcColorBlendFactor,
        dstColorBlendFactor,
        colorBlendOp,
        srcAlphaBlendFactor,
        dstAlphaBlendFactor,
        alphaBlendOp,
        colorWriteMask);
}

bool ColorBlendAttachmentState::operator==(const ColorBlendAttachmentState& other) const noexcept
{
    return (blendEnable == other.blendEnable) &&
        (srcColorBlendFactor == other.srcColorBlendFactor) &&
        (dstColorBlendFactor == other.dstColorBlendFactor) &&
        (colorBlendOp == other.colorBlendOp) &&
        (srcAlphaBlendFactor == other.srcAlphaBlendFactor) &&
        (dstAlphaBlendFactor == other.dstAlphaBlendFactor) &&
        (alphaBlendOp == other.alphaBlendOp) &&
        (colorWriteMask == other.colorWriteMask);
}

ColorBlendState::ColorBlendState(const ColorBlendAttachmentState& attachment,
    const std::initializer_list<float>& blendConstants /* {} */):
    ColorBlendState(std::vector<ColorBlendAttachmentState>{attachment}, blendConstants)
{}

ColorBlendState::ColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    const std::initializer_list<float>& blendConstants /* {} */)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = VK_FALSE;
    this->logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = utilities::copyArray(static_cast<const VkPipelineColorBlendAttachmentState *>(attachments.data()), attachments.size());
    if (0 == blendConstants.size())
    {
        this->blendConstants[0] = 1.f;
        this->blendConstants[1] = 1.f;
        this->blendConstants[2] = 1.f;
        this->blendConstants[3] = 1.f;
    }
    else
    {
        MAGMA_ASSERT(blendConstants.size() >= 4);
        memcpy(this->blendConstants, blendConstants.begin(), sizeof(this->blendConstants));
    }
}

ColorBlendState::ColorBlendState(const ColorBlendState& other)
{
    utilities::copy(this, &other);
    pAttachments = utilities::copyArray(other.pAttachments, attachmentCount);
}

ColorBlendState& ColorBlendState::operator=(const ColorBlendState& other)
{
    if (this != &other)
    {
        utilities::copy(this, &other);
        pAttachments = utilities::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

ColorBlendState::~ColorBlendState()
{
    delete[] pAttachments;
}

size_t ColorBlendState::hash() const noexcept
{
    size_t hash = utilities::hashVariadic(
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount);
    for (uint32_t i = 0; i < attachmentCount; ++i)
    {
        utilities::hashCombine(hash, utilities::hashVariadic(
            pAttachments[i].blendEnable,
            pAttachments[i].srcColorBlendFactor,
            pAttachments[i].dstColorBlendFactor,
            pAttachments[i].colorBlendOp,
            pAttachments[i].srcAlphaBlendFactor,
            pAttachments[i].dstAlphaBlendFactor,
            pAttachments[i].alphaBlendOp,
            pAttachments[i].colorWriteMask));
    }
    utilities::hashCombine(hash, utilities::hashArray(blendConstants, 4));
    return hash;
}

bool ColorBlendState::operator==(const ColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        utilities::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        utilities::compareArrays(blendConstants, other.blendConstants, 4);
}

ColorLogicOpState::ColorLogicOpState(const ColorBlendAttachmentState& attachment, VkLogicOp logicOp):
    ColorBlendState(attachment)
{
    logicOpEnable = VK_TRUE;
    this->logicOp = logicOp;
}

ColorLogicOpState::ColorLogicOpState(const std::vector<ColorBlendAttachmentState>& attachments, VkLogicOp logicOp):
    ColorBlendState(attachments)
{
    logicOpEnable = VK_TRUE;
    this->logicOp = logicOp;
}

namespace blendstates
{
const VkColorComponentFlags colorWriteMaskR(VK_COLOR_COMPONENT_R_BIT);
const VkColorComponentFlags colorWriteMaskRG(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
const VkColorComponentFlags colorWriteMaskRGB(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
const VkColorComponentFlags colorWriteMaskRGBA(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

const ColorBlendAttachmentState passR(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskR);
const ColorBlendAttachmentState passRG(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskRG);
const ColorBlendAttachmentState passRGB(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskRGB);
const ColorBlendAttachmentState passRGBA(false, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, colorWriteMaskRGBA);

const ColorBlendAttachmentState addR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskR);
const ColorBlendAttachmentState addRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskRG);
const ColorBlendAttachmentState addRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskRGB);
const ColorBlendAttachmentState addRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, colorWriteMaskRGBA);

const ColorBlendAttachmentState subtractR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskR);
const ColorBlendAttachmentState subtractRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskRG);
const ColorBlendAttachmentState subtractRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskRGB);
const ColorBlendAttachmentState subtractRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_SUBTRACT, colorWriteMaskRGBA);

const ColorBlendAttachmentState reverseSubtractR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskR);
const ColorBlendAttachmentState reverseSubtractRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskRG);
const ColorBlendAttachmentState reverseSubtractRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskRGB);
const ColorBlendAttachmentState reverseSubtractRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_REVERSE_SUBTRACT, colorWriteMaskRGBA);

const ColorBlendAttachmentState minR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskR);
const ColorBlendAttachmentState minRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskRG);
const ColorBlendAttachmentState minRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskRGB);
const ColorBlendAttachmentState minRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MIN, colorWriteMaskRGBA);

const ColorBlendAttachmentState maxR(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskR);
const ColorBlendAttachmentState maxRG(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskRG);
const ColorBlendAttachmentState maxRGB(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskRGB);
const ColorBlendAttachmentState maxRGBA(true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_MAX, colorWriteMaskRGBA);

const ColorBlendAttachmentState normalR(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskR);
const ColorBlendAttachmentState normalRG(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskRG);
const ColorBlendAttachmentState normalRGB(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskRGB);
const ColorBlendAttachmentState normalRGBA(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, colorWriteMaskRGBA);
} // namespace blendstates

namespace renderstates
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

const ColorLogicOpState logicClearWriteR(blendstates::passR, VK_LOGIC_OP_CLEAR);
const ColorLogicOpState logicAndWriteR(blendstates::passR, VK_LOGIC_OP_AND);
const ColorLogicOpState logicReverseAndWriteR(blendstates::passR, VK_LOGIC_OP_AND_REVERSE);
const ColorLogicOpState logicCopyWriteR(blendstates::passR, VK_LOGIC_OP_COPY);
const ColorLogicOpState logicInvertedAndWriteR(blendstates::passR, VK_LOGIC_OP_AND_INVERTED);
const ColorLogicOpState logicNoOpWriteR(blendstates::passR, VK_LOGIC_OP_NO_OP);
const ColorLogicOpState logicXorWriteR(blendstates::passR, VK_LOGIC_OP_XOR);
const ColorLogicOpState logicOrWriteR(blendstates::passR, VK_LOGIC_OP_OR);
const ColorLogicOpState logicNorWriteR(blendstates::passR, VK_LOGIC_OP_NOR);
const ColorLogicOpState logicEquivalentWriteR(blendstates::passR, VK_LOGIC_OP_EQUIVALENT);
const ColorLogicOpState logicInvertR(blendstates::passR, VK_LOGIC_OP_INVERT);
const ColorLogicOpState logicReverseOrWriteR(blendstates::passR, VK_LOGIC_OP_OR_REVERSE);
const ColorLogicOpState logicInvertedCopyWriteR(blendstates::passR, VK_LOGIC_OP_COPY_INVERTED);
const ColorLogicOpState logicInvertedOrWriteR(blendstates::passR, VK_LOGIC_OP_OR_INVERTED);
const ColorLogicOpState logicNandWriteR(blendstates::passR, VK_LOGIC_OP_NAND);
const ColorLogicOpState logicSetWriteR(blendstates::passR, VK_LOGIC_OP_SET);

const ColorLogicOpState logicClearWriteRG(blendstates::passRG, VK_LOGIC_OP_CLEAR);
const ColorLogicOpState logicAndWriteRG(blendstates::passRG, VK_LOGIC_OP_AND);
const ColorLogicOpState logicReverseAndWriteRG(blendstates::passRG, VK_LOGIC_OP_AND_REVERSE);
const ColorLogicOpState logicCopyWriteRG(blendstates::passRG, VK_LOGIC_OP_COPY);
const ColorLogicOpState logicInvertedAndWriteRG(blendstates::passRG, VK_LOGIC_OP_AND_INVERTED);
const ColorLogicOpState logicNoOpWriteRG(blendstates::passRG, VK_LOGIC_OP_NO_OP);
const ColorLogicOpState logicXorWriteRG(blendstates::passRG, VK_LOGIC_OP_XOR);
const ColorLogicOpState logicOrWriteRG(blendstates::passRG, VK_LOGIC_OP_OR);
const ColorLogicOpState logicNorWriteRG(blendstates::passRG, VK_LOGIC_OP_NOR);
const ColorLogicOpState logicEquivalentWriteRG(blendstates::passRG, VK_LOGIC_OP_EQUIVALENT);
const ColorLogicOpState logicInvertRG(blendstates::passRG, VK_LOGIC_OP_INVERT);
const ColorLogicOpState logicReverseOrWriteRG(blendstates::passRG, VK_LOGIC_OP_OR_REVERSE);
const ColorLogicOpState logicInvertedCopyWriteRG(blendstates::passRG, VK_LOGIC_OP_COPY_INVERTED);
const ColorLogicOpState logicInvertedOrWriteRG(blendstates::passRG, VK_LOGIC_OP_OR_INVERTED);
const ColorLogicOpState logicNandWriteRG(blendstates::passRG, VK_LOGIC_OP_NAND);
const ColorLogicOpState logicSetWriteRG(blendstates::passRG, VK_LOGIC_OP_SET);

const ColorLogicOpState logicClearWriteRGB(blendstates::passRGB, VK_LOGIC_OP_CLEAR);
const ColorLogicOpState logicAndWriteRGB(blendstates::passRGB, VK_LOGIC_OP_AND);
const ColorLogicOpState logicReverseAndWriteRGB(blendstates::passRGB, VK_LOGIC_OP_AND_REVERSE);
const ColorLogicOpState logicCopyWriteRGB(blendstates::passRGB, VK_LOGIC_OP_COPY);
const ColorLogicOpState logicInvertedAndWriteRGB(blendstates::passRGB, VK_LOGIC_OP_AND_INVERTED);
const ColorLogicOpState logicNoOpWriteRGB(blendstates::passRGB, VK_LOGIC_OP_NO_OP);
const ColorLogicOpState logicXorWriteRGB(blendstates::passRGB, VK_LOGIC_OP_XOR);
const ColorLogicOpState logicOrWriteRGB(blendstates::passRGB, VK_LOGIC_OP_OR);
const ColorLogicOpState logicNorWriteRGB(blendstates::passRGB, VK_LOGIC_OP_NOR);
const ColorLogicOpState logicEquivalentWriteRGB(blendstates::passRGB, VK_LOGIC_OP_EQUIVALENT);
const ColorLogicOpState logicInvertRGB(blendstates::passRGB, VK_LOGIC_OP_INVERT);
const ColorLogicOpState logicReverseOrWriteRGB(blendstates::passRGB, VK_LOGIC_OP_OR_REVERSE);
const ColorLogicOpState logicInvertedCopyWriteRGB(blendstates::passRGB, VK_LOGIC_OP_COPY_INVERTED);
const ColorLogicOpState logicInvertedOrWriteRGB(blendstates::passRGB, VK_LOGIC_OP_OR_INVERTED);
const ColorLogicOpState logicNandWriteRGB(blendstates::passRGB, VK_LOGIC_OP_NAND);
const ColorLogicOpState logicSetWriteRGB(blendstates::passRGB, VK_LOGIC_OP_SET);

const ColorLogicOpState logicClearWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_CLEAR);
const ColorLogicOpState logicAndWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_AND);
const ColorLogicOpState logicReverseAndWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_AND_REVERSE);
const ColorLogicOpState logicCopyWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_COPY);
const ColorLogicOpState logicInvertedAndWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_AND_INVERTED);
const ColorLogicOpState logicNoOpWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_NO_OP);
const ColorLogicOpState logicXorWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_XOR);
const ColorLogicOpState logicOrWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_OR);
const ColorLogicOpState logicNorWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_NOR);
const ColorLogicOpState logicEquivalentWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_EQUIVALENT);
const ColorLogicOpState logicInvertRGBA(blendstates::passRGBA, VK_LOGIC_OP_INVERT);
const ColorLogicOpState logicReverseOrWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_OR_REVERSE);
const ColorLogicOpState logicInvertedCopyWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_COPY_INVERTED);
const ColorLogicOpState logicInvertedOrWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_OR_INVERTED);
const ColorLogicOpState logicNandWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_NAND);
const ColorLogicOpState logicSetWriteRGBA(blendstates::passRGBA, VK_LOGIC_OP_SET);
} // namespace renderstates
} // namespace magma
