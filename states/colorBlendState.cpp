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
#include "../internal/copy.h"
#include "../internal/compareArrays.h"

namespace magma
{
MultiColorBlendState::MultiColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    const std::initializer_list<float>& blendConstants /* {} */)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = VK_FALSE;
    this->logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = internal::copyArray(static_cast<const VkPipelineColorBlendAttachmentState *>(attachments.data()), attachments.size());
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

MultiColorBlendState::MultiColorBlendState(const MultiColorBlendState& other)
{
    internal::copy(this, &other);
    pAttachments = internal::copyArray(other.pAttachments, attachmentCount);
}

MultiColorBlendState& MultiColorBlendState::operator=(const MultiColorBlendState& other)
{
    if (this != &other)
    {
        internal::copy(this, &other);
        pAttachments = internal::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

MultiColorBlendState::~MultiColorBlendState()
{
    delete[] pAttachments;
}

size_t MultiColorBlendState::hash() const noexcept
{
    size_t hash = internal::hashArgs(
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

bool MultiColorBlendState::operator==(const MultiColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        internal::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        internal::compareArrays(blendConstants, other.blendConstants, 4);
}

MultiColorLogicOpState::MultiColorLogicOpState(const std::vector<ColorBlendAttachmentState>& attachments, VkLogicOp logicOp):
    MultiColorBlendState(attachments)
{
    logicOpEnable = VK_TRUE;
    this->logicOp = logicOp;
}
} // namespace magma
