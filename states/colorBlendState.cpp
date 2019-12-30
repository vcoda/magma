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
#include "../detail/copy.h"
#include "../detail/compare.h"

namespace magma
{
ManagedColorBlendState::ManagedColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    const std::initializer_list<float>& blendConstants /* {1, 1, 1, 1} */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = VK_FALSE;
    this->logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = detail::copyArray<VkPipelineColorBlendAttachmentState>(attachments.data(), attachments.size());
    MAGMA_ASSERT(blendConstants.size() >= 4);
    const auto c = blendConstants.begin();
    this->blendConstants[0] = c[0];
    this->blendConstants[1] = c[1];
    this->blendConstants[2] = c[2];
    this->blendConstants[3] = c[3];
}

ManagedColorBlendState::ManagedColorBlendState(const ColorBlendState& blendState) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = blendState.flags;
    logicOpEnable = blendState.logicOpEnable;
    logicOp = blendState.logicOp;
    attachmentCount = blendState.attachmentCount;
    pAttachments = detail::copyArray(blendState.pAttachments, blendState.attachmentCount);
    blendConstants[0] = blendState.blendConstants[0];
    blendConstants[1] = blendState.blendConstants[1];
    blendConstants[2] = blendState.blendConstants[2];
    blendConstants[3] = blendState.blendConstants[3];
}

ManagedColorBlendState::ManagedColorBlendState(const ManagedColorBlendState& other) noexcept
{
    detail::copy(this, &other);
    pAttachments = detail::copyArray(other.pAttachments, attachmentCount);
}

ManagedColorBlendState& ManagedColorBlendState::operator=(const ManagedColorBlendState& other) noexcept
{
    if (this != &other)
    {
        delete[] pAttachments;
        detail::copy(this, &other);
        pAttachments = detail::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

ManagedColorBlendState::~ManagedColorBlendState()
{
    delete[] pAttachments;
}

std::size_t ManagedColorBlendState::hash() const noexcept
{
    std::size_t hash = detail::hashArgs(
        sType,
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount);
    for (uint32_t i = 0; i < attachmentCount; ++i)
    {
        detail::hashCombine(hash, detail::hashArgs(
            pAttachments[i].blendEnable,
            pAttachments[i].srcColorBlendFactor,
            pAttachments[i].dstColorBlendFactor,
            pAttachments[i].colorBlendOp,
            pAttachments[i].srcAlphaBlendFactor,
            pAttachments[i].dstAlphaBlendFactor,
            pAttachments[i].alphaBlendOp,
            pAttachments[i].colorWriteMask));
    }
    detail::hashCombine(hash, detail::hashArray(blendConstants, 4));
    return hash;
}

bool ManagedColorBlendState::operator==(const ManagedColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        detail::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        detail::compareArrays(blendConstants, other.blendConstants, 4);
}
} // namespace magma
