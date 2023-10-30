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
#include "colorBlendState.h"

namespace magma
{
MultiColorBlendState::MultiColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    VkPipelineColorBlendStateCreateFlags flags /* 0 */,
    const std::initializer_list<float>& blendConstants_ /* {1, 1, 1, 1} */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = flags;
    logicOpEnable = VK_FALSE;
    logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = core::copyArray<VkPipelineColorBlendAttachmentState>(attachments.data(), attachments.size());
    uint32_t i = 0;
    MAGMA_ASSERT(blendConstants_.size() == 4);
    for (float value: blendConstants_)
        blendConstants[i++] = value;
}

MultiColorBlendState::MultiColorBlendState(const ColorBlendState& blendState) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = blendState.flags;
    logicOpEnable = blendState.logicOpEnable;
    logicOp = blendState.logicOp;
    attachmentCount = blendState.attachmentCount;
    pAttachments = core::copyArray(blendState.pAttachments, blendState.attachmentCount);
    blendConstants[0] = blendState.blendConstants[0];
    blendConstants[1] = blendState.blendConstants[1];
    blendConstants[2] = blendState.blendConstants[2];
    blendConstants[3] = blendState.blendConstants[3];
}

MultiColorBlendState::MultiColorBlendState(const MultiColorBlendState& other) noexcept
{
    core::copy(this, &other);
    pAttachments = core::copyArray(other.pAttachments, attachmentCount);
}

MultiColorBlendState& MultiColorBlendState::operator=(const MultiColorBlendState& other) noexcept
{
    if (this != &other)
    {
        delete[] pAttachments;
        core::copy(this, &other);
        pAttachments = core::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

MultiColorBlendState::~MultiColorBlendState()
{
    delete[] pAttachments;
}

hash_t MultiColorBlendState::hash() const noexcept
{
    hash_t hash = core::hashArgs(
        sType,
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount);
    for (uint32_t i = 0; i < attachmentCount; ++i)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            pAttachments[i].blendEnable,
            pAttachments[i].srcColorBlendFactor,
            pAttachments[i].dstColorBlendFactor,
            pAttachments[i].colorBlendOp,
            pAttachments[i].srcAlphaBlendFactor,
            pAttachments[i].dstAlphaBlendFactor,
            pAttachments[i].alphaBlendOp,
            pAttachments[i].colorWriteMask));
    }
    return core::hashCombine(hash, core::hashArgs(
        blendConstants[0],
        blendConstants[1],
        blendConstants[2],
        blendConstants[3]));
}

bool MultiColorBlendState::operator==(const MultiColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        core::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        core::compareArrays(blendConstants, other.blendConstants, 4);
}
} // namespace magma
