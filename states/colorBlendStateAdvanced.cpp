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
#include "colorBlendStateAdvanced.h"

namespace magma
{
#ifdef VK_EXT_blend_operation_advanced
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
    pAttachments = core::copyArray<VkPipelineColorBlendAttachmentState>(&attachment, 1);
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
    pAttachments = core::copyInitializerList<VkPipelineColorBlendAttachmentState>(attachments);
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
    pAttachments = core::copyVector<VkPipelineColorBlendAttachmentState>(attachments);
}

AdvancedColorBlendState::AdvancedColorBlendState(const AdvancedColorBlendState& other)
{
    sType = other.sType;
    pNext = core::copy((VkPipelineColorBlendAdvancedStateCreateInfoEXT *)other.pNext);
    flags = other.flags;
    logicOpEnable = other.logicOpEnable;
    logicOp = other.logicOp;
    attachmentCount = other.attachmentCount;
    pAttachments = core::copyArray(other.pAttachments, other.attachmentCount);
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
        core::copy(this, &other);
        pNext = core::copy((VkPipelineColorBlendAdvancedStateCreateInfoEXT *)other.pNext);
        pAttachments = core::copyArray(other.pAttachments, attachmentCount);
    }
    return *this;
}

AdvancedColorBlendState::~AdvancedColorBlendState()
{
    delete reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
    delete[] pAttachments;
}

hash_t AdvancedColorBlendState::hash() const noexcept
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
    hash = core::hashCombine(hash, core::hashArray(blendConstants, 4));
    auto colorBlendAdvanced = reinterpret_cast<const VkPipelineColorBlendAdvancedStateCreateInfoEXT *>(pNext);
    return core::hashCombine(hash, core::hashArgs(
        colorBlendAdvanced->sType,
        colorBlendAdvanced->srcPremultiplied,
        colorBlendAdvanced->dstPremultiplied,
        colorBlendAdvanced->blendOverlap));
}

bool AdvancedColorBlendState::operator==(const AdvancedColorBlendState& other) const noexcept
{
    return (sType == other.sType) &&
        (core::compare<VkPipelineColorBlendAdvancedStateCreateInfoEXT>(pNext, other.pNext)) &&
        (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        core::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        (blendConstants[0] == other.blendConstants[0]) &&
        (blendConstants[1] == other.blendConstants[1]) &&
        (blendConstants[2] == other.blendConstants[2]) &&
        (blendConstants[3] == other.blendConstants[3]);
}
#endif // VK_EXT_blend_operation_advanced
} // namespace magma
