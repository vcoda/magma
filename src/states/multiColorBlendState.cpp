/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "multiColorBlendState.h"

namespace magma
{
MultiColorBlendState::MultiColorBlendState(const std::initializer_list<ColorBlendAttachmentState>& attachments,
    VkPipelineColorBlendStateCreateFlags flags /* 0 */,
    const std::initializer_list<float>& blendConstants_ /* {1, 1, 1, 1} */) noexcept
{
    flags = flags;
    attachmentCount = core::countof(attachments);
    pAttachments = core::copyInitializerList(attachments);
    MAGMA_ASSERT(blendConstants_.size() == 4);
    auto it = blendConstants_.begin();
    blendConstants[0] = *it++;
    blendConstants[1] = *it++;
    blendConstants[2] = *it++;
    blendConstants[3] = *it;
}

MultiColorBlendState::MultiColorBlendState(const ColorBlendState& other) noexcept:
    ColorBlendState(other)
{
    pAttachments = core::copyArray(other.pAttachments, other.attachmentCount);
}

MultiColorBlendState& MultiColorBlendState::operator=(const MultiColorBlendState& other) noexcept
{
    if (this != &other)
    {
        pNext = other.pNext;
        flags = other.flags;
        logicOpEnable = other.logicOpEnable;
        logicOp = other.logicOp;
        attachmentCount = other.attachmentCount;
        delete[] pAttachments;
        pAttachments = core::copyArray(other.pAttachments, other.attachmentCount);
        blendConstants[0] = other.blendConstants[0];
        blendConstants[1] = other.blendConstants[1];
        blendConstants[2] = other.blendConstants[2];
        blendConstants[3] = other.blendConstants[3];
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
