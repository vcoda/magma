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
#include "colorBlendStateAdvanced.h"
#include "../internal/copy.h"
#include "../internal/compare.h"

namespace magma
{
AdvancedColorBlendState::AdvancedColorBlendState(const AdvancedColorBlendAttachmentState& attachment,
    bool srcPremultiplied, bool dstPremultiplied, VkBlendOverlapEXT blendOverlap)
{
    VkPipelineColorBlendAdvancedStateCreateInfoEXT *colorBlendAdvanced = new VkPipelineColorBlendAdvancedStateCreateInfoEXT;
    colorBlendAdvanced->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT;
    colorBlendAdvanced->pNext = nullptr;
    colorBlendAdvanced->srcPremultiplied = MAGMA_BOOLEAN(srcPremultiplied);
    colorBlendAdvanced->dstPremultiplied = MAGMA_BOOLEAN(dstPremultiplied);
    colorBlendAdvanced->blendOverlap = blendOverlap;
    pNext = colorBlendAdvanced;
    attachmentCount = 1;
    pAttachments = internal::copyArray<VkPipelineColorBlendAttachmentState>(&attachment, 1);
}

AdvancedColorBlendState::AdvancedColorBlendState(const std::vector<AdvancedColorBlendAttachmentState>& attachments,
    bool srcPremultiplied, bool dstPremultiplied, VkBlendOverlapEXT blendOverlap)
{
    VkPipelineColorBlendAdvancedStateCreateInfoEXT *colorBlendAdvanced = new VkPipelineColorBlendAdvancedStateCreateInfoEXT;
    colorBlendAdvanced->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT;
    colorBlendAdvanced->pNext = nullptr;
    colorBlendAdvanced->srcPremultiplied = MAGMA_BOOLEAN(srcPremultiplied);
    colorBlendAdvanced->dstPremultiplied = MAGMA_BOOLEAN(dstPremultiplied);
    colorBlendAdvanced->blendOverlap = blendOverlap;
    pNext = colorBlendAdvanced;
    attachmentCount = MAGMA_COUNT(attachments);
    pAttachments = internal::copyArray<VkPipelineColorBlendAttachmentState>(attachments.data(), attachments.size());
}

AdvancedColorBlendState::AdvancedColorBlendState(const AdvancedColorBlendState& other)
{
    internal::copy(this, &other);
    pNext = internal::copy<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(other.pNext);
    pAttachments = internal::copyArray(other.pAttachments, other.attachmentCount);
}
        
AdvancedColorBlendState& AdvancedColorBlendState::operator=(const AdvancedColorBlendState& other)
{
    if (this != &other)
    {
        internal::copy(this, &other);
        pNext = internal::copy<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(other.pNext);
        pAttachments = internal::copyArray(other.pAttachments, other.attachmentCount);
    }
    return *this;
}

AdvancedColorBlendState::~AdvancedColorBlendState()
{
    delete reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
    delete[] pAttachments;
}

size_t AdvancedColorBlendState::hash() const noexcept
{
    auto colorBlendAdvanced = reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
    size_t hash = internal::hashArgs(
        colorBlendAdvanced->sType,
        colorBlendAdvanced->srcPremultiplied,
        colorBlendAdvanced->dstPremultiplied,
        colorBlendAdvanced->blendOverlap);
    internal::hashCombine(hash, internal::hashArgs(
        sType,
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount));
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

bool AdvancedColorBlendState::operator==(const AdvancedColorBlendState& other) const noexcept
{
    return (sType == other.sType) &&
        (internal::compare<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(pNext, other.pNext)) &&
        (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        internal::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        (blendConstants[0] == other.blendConstants[0]) &&
        (blendConstants[1] == other.blendConstants[1]) &&
        (blendConstants[2] == other.blendConstants[2]) &&
        (blendConstants[3] == other.blendConstants[3]);
}
} // namespace magma
