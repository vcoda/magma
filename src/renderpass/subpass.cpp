/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "subpass.h"

namespace magma
{
SubpassDescription::SubpassDescription() noexcept:
    VkSubpassDescription{
        0, // flags
        VK_PIPELINE_BIND_POINT_GRAPHICS, // pipelineBindPoint
        0, // inputAttachmentCount
        nullptr, // pInputAttachments
        0, // colorAttachmentCount
        nullptr, // pColorAttachments
        nullptr, // pResolveAttachments
        nullptr, // pDepthStencilAttachment
        0, // preserveAttachmentCount
        nullptr // pPreserveAttachments
    }
{}

SubpassDescription::SubpassDescription(VkPipelineBindPoint pipelineBindPoint) noexcept:
    VkSubpassDescription{
        0, // flags
        pipelineBindPoint,
        0, // inputAttachmentCount
        nullptr, // pInputAttachments
        0, // colorAttachmentCount
        nullptr, // pColorAttachments
        nullptr, // pResolveAttachments
        nullptr, // pDepthStencilAttachment
        0, // preserveAttachmentCount
        nullptr // pPreserveAttachments
    }
{}

SubpassDescription::SubpassDescription(VkImageLayout colorLayout) noexcept:
    SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    VkAttachmentReference *colorReference = MAGMA_NEW VkAttachmentReference[1];
    if (colorReference)
    {
        colorReference->attachment = 0;
        colorReference->layout = colorLayout;
        colorAttachmentCount = 1;
    }
    pColorAttachments = colorReference;
}

SubpassDescription::SubpassDescription(VkImageLayout colorLayout, VkImageLayout depthStencilLayout) noexcept:
    SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    if (colorLayout != VK_IMAGE_LAYOUT_UNDEFINED)
    {
        VkAttachmentReference *colorReference = MAGMA_NEW VkAttachmentReference[1];
        if (colorReference)
        {
            colorReference->attachment = 0;
            colorReference->layout = colorLayout;
            colorAttachmentCount = 1;
        }
        pColorAttachments = colorReference;
    }
    VkAttachmentReference *depthStencilReference = MAGMA_NEW VkAttachmentReference;
    if (depthStencilReference)
    {
        depthStencilReference->attachment = 1;
        depthStencilReference->layout = depthStencilLayout;
    }
    pDepthStencilAttachment = depthStencilReference;
}

SubpassDescription::SubpassDescription(const std::vector<VkImageLayout>& colorLayouts) noexcept:
    SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    if (!colorLayouts.empty())
    {
        VkAttachmentReference *colorReferences = MAGMA_NEW VkAttachmentReference[colorLayouts.size()];
        if (colorReferences)
        {
            for (auto layout: colorLayouts)
            {
                colorReferences[colorAttachmentCount].attachment = colorAttachmentCount;
                colorReferences[colorAttachmentCount].layout = layout;
                ++colorAttachmentCount;
            }
        }
        pColorAttachments = colorReferences;
    }
}

SubpassDescription::SubpassDescription(const std::vector<VkImageLayout>& colorLayouts, const VkImageLayout& depthStencilLayout) noexcept:
    SubpassDescription(colorLayouts)
{
    VkAttachmentReference *depthStencilReference = MAGMA_NEW VkAttachmentReference;
    if (depthStencilReference)
    {
        depthStencilReference->attachment = colorAttachmentCount;
        depthStencilReference->layout = depthStencilLayout;
    }
    pDepthStencilAttachment = depthStencilReference;
}

SubpassDescription::SubpassDescription(const SubpassDescription& other) noexcept
{
    flags = other.flags;
    pipelineBindPoint = other.pipelineBindPoint;
    inputAttachmentCount = other.inputAttachmentCount;
    pInputAttachments = core::copyArray(other.pInputAttachments, other.inputAttachmentCount);
    colorAttachmentCount = other.colorAttachmentCount;
    pColorAttachments = core::copyArray(other.pColorAttachments, other.colorAttachmentCount);
    pResolveAttachments = core::copyArray(other.pResolveAttachments, other.colorAttachmentCount);
    pDepthStencilAttachment = core::copy(other.pDepthStencilAttachment);
    preserveAttachmentCount = other.preserveAttachmentCount;
    pPreserveAttachments = core::copyArray(other.pPreserveAttachments, other.preserveAttachmentCount);
}

SubpassDescription& SubpassDescription::operator=(const SubpassDescription& other) noexcept
{
    if (this != &other)
    {
        this->~SubpassDescription();
        flags = other.flags;
        pipelineBindPoint = other.pipelineBindPoint;
        inputAttachmentCount = other.inputAttachmentCount;
        pInputAttachments = core::copyArray(other.pInputAttachments, other.inputAttachmentCount);
        colorAttachmentCount = other.colorAttachmentCount;
        pColorAttachments = core::copyArray(other.pColorAttachments, other.colorAttachmentCount);
        pResolveAttachments = core::copyArray(other.pResolveAttachments, other.colorAttachmentCount);
        pDepthStencilAttachment = core::copy(other.pDepthStencilAttachment);
        preserveAttachmentCount = other.preserveAttachmentCount;
        pPreserveAttachments = core::copyArray(other.pPreserveAttachments, other.preserveAttachmentCount);
    }
    return *this;
}

SubpassDescription::~SubpassDescription()
{
    delete[] pInputAttachments;
    delete[] pColorAttachments;
    delete[] pResolveAttachments;
    delete pDepthStencilAttachment;
    delete[] pPreserveAttachments;
}

hash_t SubpassDescription::getHash() const noexcept
{
    hash_t hash = core::hashArgs(
        flags,
        pipelineBindPoint,
        inputAttachmentCount,
        colorAttachmentCount,
        preserveAttachmentCount);
    uint32_t i;
    for (i = 0; i < inputAttachmentCount; ++i)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            pInputAttachments[i].attachment,
            pInputAttachments[i].layout));
    }
    for (i = 0; i < colorAttachmentCount; ++i)
    {
        if (pColorAttachments)
        {
            hash = core::hashCombine(hash, core::hashArgs(
                pColorAttachments[i].attachment,
                pColorAttachments[i].layout));
        }
        else if (pResolveAttachments)
        {
            hash = core::hashCombine(hash, core::hashArgs(
                pResolveAttachments[i].attachment,
                pResolveAttachments[i].layout));
        }
    }
    if (pDepthStencilAttachment)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            pDepthStencilAttachment->attachment,
            pDepthStencilAttachment->layout));
    }
    for (i = 0; i < preserveAttachmentCount; ++i)
    {
        hash = core::hashCombine(hash, core::hash(
            pPreserveAttachments[i]));
    }
    return hash;
}

namespace subpass
{
const SubpassDescription colorAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
const SubpassDescription colorDepthStencilAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const SubpassDescription colorAttachmentDepthStencilReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
const SubpassDescription depthStencilAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const SubpassDescription depthStencilReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

#ifdef VK_KHR_maintenance2
const SubpassDescription colorStencilAttachmentDepthReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const SubpassDescription colorDepthAttachmentStencilReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR);
const SubpassDescription depthReadOnlyStencilAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const SubpassDescription depthAttachmentStencilReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR);
#endif // VK_KHR_maintenance2

#ifdef VK_KHR_separate_depth_stencil_layouts
const SubpassDescription colorDepthAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR);
const SubpassDescription colorStencilAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const SubpassDescription colorAttachmentDepthReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR);
const SubpassDescription colorAttachmentStencilReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR);
const SubpassDescription depthAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR);
const SubpassDescription stencilAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const SubpassDescription depthReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR);
const SubpassDescription stencilReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR);
#endif // VK_KHR_separate_depth_stencil_layouts
} // namespace subpass
} // namespace magma
