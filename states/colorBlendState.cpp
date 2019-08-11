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
#include "pch.h"
#pragma hdrstop
#include "colorBlendState.h"
#include "../internal/copy.h"
#include "../internal/compare.h"

namespace magma
{
ManagedColorBlendState::ManagedColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    const std::initializer_list<float>& blendConstants /* {1, 1, 1, 1} */)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = VK_FALSE;
    this->logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = internal::copyArray<VkPipelineColorBlendAttachmentState>(attachments.data(), attachments.size());
    MAGMA_ASSERT(blendConstants.size() >= 4);
    const auto c = blendConstants.begin();
    this->blendConstants[0] = c[0];
    this->blendConstants[1] = c[1];
    this->blendConstants[2] = c[2];
    this->blendConstants[3] = c[3];
}

ManagedColorBlendState::ManagedColorBlendState(const ColorBlendState& blendState)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = blendState.flags;
    logicOpEnable = blendState.logicOpEnable;
    logicOp = blendState.logicOp;
    attachmentCount = blendState.attachmentCount;
    pAttachments = internal::copyArray(blendState.pAttachments, blendState.attachmentCount);
    blendConstants[0] = blendState.blendConstants[0];
    blendConstants[1] = blendState.blendConstants[1];
    blendConstants[2] = blendState.blendConstants[2];
    blendConstants[3] = blendState.blendConstants[3];
}

ManagedColorBlendState::ManagedColorBlendState(const ManagedColorBlendState& other)
{
    internal::copy(this, &other);
    pAttachments = internal::copyArray(other.pAttachments, attachmentCount);
}

ManagedColorBlendState& ManagedColorBlendState::operator=(const ManagedColorBlendState& other)
{
    if (this != &other)
    {
        internal::copy(this, &other);
        pAttachments = internal::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

ManagedColorBlendState::~ManagedColorBlendState()
{
    delete[] pAttachments;
}

std::size_t ManagedColorBlendState::hash() const noexcept
{
    std::size_t hash = internal::hashArgs(
        sType,
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount);
    for (uint32_t i = 0; i < attachmentCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pAttachments[i].blendEnable,
            pAttachments[i].srcColorBlendFactor,
            pAttachments[i].dstColorBlendFactor,
            pAttachments[i].colorBlendOp,
            pAttachments[i].srcAlphaBlendFactor,
            pAttachments[i].dstAlphaBlendFactor,
            pAttachments[i].alphaBlendOp,
            pAttachments[i].colorWriteMask));
    }
    internal::hashCombine(hash, internal::hashArray(blendConstants, 4));
    return hash;
}

bool ManagedColorBlendState::operator==(const ManagedColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        internal::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        internal::compareArrays(blendConstants, other.blendConstants, 4);
}
} // namespace magma
