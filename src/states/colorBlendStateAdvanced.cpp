/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
AdvancedColorBlendState::AdvancedColorBlendState(const ColorBlendAttachmentState& attachment,
    bool srcPremultiplied, bool dstPremultiplied, VkBlendOverlapEXT blendOverlap) noexcept:
    colorBlendAdvancedStateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT,
        nullptr,
        MAGMA_BOOLEAN(srcPremultiplied),
        MAGMA_BOOLEAN(dstPremultiplied),
        blendOverlap
    },
    attachments{attachment}
{
    pNext = &colorBlendAdvancedStateInfo;
    attachmentCount = 1;
    pAttachments = attachments.data();
}

AdvancedColorBlendState::AdvancedColorBlendState(const std::vector<ColorBlendAttachmentState>& attachments,
    bool srcPremultiplied, bool dstPremultiplied, VkBlendOverlapEXT blendOverlap) noexcept:
    colorBlendAdvancedStateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT,
        nullptr,
        MAGMA_BOOLEAN(srcPremultiplied),
        MAGMA_BOOLEAN(dstPremultiplied),
        blendOverlap
    },
    attachments(attachments)
{
    pNext = &colorBlendAdvancedStateInfo;
    attachmentCount = core::countof(attachments);
    pAttachments = attachments.data();
}

hash_t AdvancedColorBlendState::hash() const noexcept
{
    hash_t hash = core::hashArgs(
        sType,
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount);
    for (auto const& attachment: attachments)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            attachment.blendEnable,
            attachment.srcColorBlendFactor,
            attachment.dstColorBlendFactor,
            attachment.colorBlendOp,
            attachment.srcAlphaBlendFactor,
            attachment.dstAlphaBlendFactor,
            attachment.alphaBlendOp,
            attachment.colorWriteMask));
    }
    hash = core::hashCombine(hash, core::hashArgs(
        blendConstants[0],
        blendConstants[1],
        blendConstants[2],
        blendConstants[3]));
    hash = core::hashCombine(hash, core::hashArgs(
        colorBlendAdvancedStateInfo.sType,
        colorBlendAdvancedStateInfo.srcPremultiplied,
        colorBlendAdvancedStateInfo.dstPremultiplied,
        colorBlendAdvancedStateInfo.blendOverlap));
    return hash;
}

bool AdvancedColorBlendState::operator==(const AdvancedColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        core::compareArrays(pAttachments, other.pAttachments, attachmentCount) &&
        core::compareArrays(blendConstants, other.blendConstants, 4) &&
        (colorBlendAdvancedStateInfo.srcPremultiplied == other.colorBlendAdvancedStateInfo.srcPremultiplied) &&
        (colorBlendAdvancedStateInfo.dstPremultiplied == other.colorBlendAdvancedStateInfo.dstPremultiplied) &&
        (colorBlendAdvancedStateInfo.blendOverlap == other.colorBlendAdvancedStateInfo.blendOverlap);
}
#endif // VK_EXT_blend_operation_advanced
} // namespace magma
