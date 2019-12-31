/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "colorBlendStateAdvanced.h"
#include "../detail/copy.h"
#include "../detail/compare.h"

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
    pAttachments = detail::copyArray<VkPipelineColorBlendAttachmentState>(&attachment, 1);
}

AdvancedColorBlendState::AdvancedColorBlendState(const std::initializer_list<AdvancedColorBlendAttachmentState>& attachments,
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
    pAttachments = detail::copyInitializerList<VkPipelineColorBlendAttachmentState>(attachments);
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
    pAttachments = detail::copyVector<VkPipelineColorBlendAttachmentState>(attachments);
}

AdvancedColorBlendState::AdvancedColorBlendState(const AdvancedColorBlendState& other)
{
    sType = other.sType;
    pNext = detail::copy<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(other.pNext);
    flags = other.flags;
    logicOpEnable = other.logicOpEnable;
    logicOp = other.logicOp;
    attachmentCount = other.attachmentCount;
    pAttachments = detail::copyArray(other.pAttachments, other.attachmentCount);
    blendConstants[0] = other.blendConstants[0];
    blendConstants[1] = other.blendConstants[1];
    blendConstants[2] = other.blendConstants[2];
    blendConstants[3] = other.blendConstants[3];
}

AdvancedColorBlendState& AdvancedColorBlendState::operator=(const AdvancedColorBlendState& other)
{
    if (this != &other)
    {
        delete reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
        delete[] pAttachments;
        detail::copy(this, &other);
        pNext = detail::copy<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(other.pNext);
        pAttachments = detail::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

AdvancedColorBlendState::~AdvancedColorBlendState()
{
    delete reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
    delete[] pAttachments;
}

std::size_t AdvancedColorBlendState::hash() const noexcept
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
    auto colorBlendAdvanced = reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
    detail::hashCombine(hash, detail::hashArgs(
        colorBlendAdvanced->sType,
        colorBlendAdvanced->srcPremultiplied,
        colorBlendAdvanced->dstPremultiplied,
        colorBlendAdvanced->blendOverlap));
    return hash;
}

bool AdvancedColorBlendState::operator==(const AdvancedColorBlendState& other) const noexcept
{
    return (sType == other.sType) &&
        (detail::compare<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(pNext, other.pNext)) &&
        (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        detail::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        (blendConstants[0] == other.blendConstants[0]) &&
        (blendConstants[1] == other.blendConstants[1]) &&
        (blendConstants[2] == other.blendConstants[2]) &&
        (blendConstants[3] == other.blendConstants[3]);
}
} // namespace magma
