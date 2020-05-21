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
#include "subpass.h"
#include "../core/copy.h"

namespace magma
{
SubpassDescription::SubpassDescription() noexcept
{
    flags = 0;
    this->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    inputAttachmentCount = 0;
    pInputAttachments = nullptr;
    colorAttachmentCount = 0;
    pColorAttachments = nullptr;
    pResolveAttachments = nullptr;
    pDepthStencilAttachment = nullptr;
    preserveAttachmentCount = 0;
    pPreserveAttachments = nullptr;
}

inline SubpassDescription::SubpassDescription(VkPipelineBindPoint pipelineBindPoint) noexcept
{
    MAGMA_ASSERT(VK_PIPELINE_BIND_POINT_GRAPHICS == pipelineBindPoint);
    flags = 0;
    this->pipelineBindPoint = pipelineBindPoint;
    inputAttachmentCount = 0;
    pInputAttachments = nullptr;
}

SubpassDescription::SubpassDescription(VkImageLayout colorLayout) noexcept:
    SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    VkAttachmentReference *colorReference = new(std::nothrow) VkAttachmentReference[1];
    if (colorReference)
    {
        colorReference->attachment = 0;
        colorReference->layout = colorLayout;
        colorAttachmentCount = 1;
    }
    pColorAttachments = colorReference;
    pResolveAttachments = nullptr;
    pDepthStencilAttachment = nullptr;
    preserveAttachmentCount = 0;
    pPreserveAttachments = nullptr;
}

SubpassDescription::SubpassDescription(VkImageLayout colorLayout, VkImageLayout depthStencilLayout) noexcept:
    SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    if (colorLayout != VK_IMAGE_LAYOUT_UNDEFINED)
    {
        VkAttachmentReference *colorReference = new(std::nothrow) VkAttachmentReference[1];
        if (colorReference)
        {
            colorReference->attachment = 0;
            colorReference->layout = colorLayout;
            colorAttachmentCount = 1;
        }
        pColorAttachments = colorReference;
    }
    pResolveAttachments = nullptr;
    VkAttachmentReference *depthStencilReference = new(std::nothrow) VkAttachmentReference;
    if (depthStencilReference)
    {
        depthStencilReference->attachment = 1;
        depthStencilReference->layout = depthStencilLayout;
    }
    pDepthStencilAttachment = depthStencilReference;
    preserveAttachmentCount = 0;
    pPreserveAttachments = nullptr;
}

SubpassDescription::SubpassDescription(const std::vector<VkImageLayout>& colorLayouts) noexcept:
    SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    if (!colorLayouts.empty())
    {
        VkAttachmentReference *colorReferences = new(std::nothrow) VkAttachmentReference[colorLayouts.size()];
        if (colorReferences)
        {
            for (auto layout : colorLayouts)
            {
                colorReferences[colorAttachmentCount].attachment = colorAttachmentCount;
                colorReferences[colorAttachmentCount].layout = layout;
                ++colorAttachmentCount;
            }
        }
        pColorAttachments = colorReferences;
    }
    pResolveAttachments = nullptr;
    pDepthStencilAttachment = nullptr;
    preserveAttachmentCount = 0;
    pPreserveAttachments = nullptr;
}

SubpassDescription::SubpassDescription(const std::vector<VkImageLayout>& colorLayouts, const VkImageLayout& depthStencilLayout) noexcept:
    SubpassDescription(colorLayouts)
{
    VkAttachmentReference *depthStencilReference = new(std::nothrow) VkAttachmentReference;
    if (depthStencilReference)
    {
        depthStencilReference->attachment = colorAttachmentCount;
        depthStencilReference->layout = depthStencilLayout;
    }
    pDepthStencilAttachment = depthStencilReference;
}

SubpassDescription::SubpassDescription(const SubpassDescription& other) noexcept
{
    core::copy(this, &other);
    if (other.pColorAttachments)
        pColorAttachments = core::copyArray(other.pColorAttachments, colorAttachmentCount);
    if (other.pDepthStencilAttachment)
        pDepthStencilAttachment = core::copy(other.pDepthStencilAttachment);
}

SubpassDescription& SubpassDescription::operator=(const SubpassDescription& other) noexcept
{
    if (this != &other)
    {
        delete[] pColorAttachments;
        delete pDepthStencilAttachment;
        core::copy(this, &other);
        if (other.pColorAttachments)
            pColorAttachments = core::copyArray(other.pColorAttachments, colorAttachmentCount);
        if (other.pDepthStencilAttachment)
            pDepthStencilAttachment = core::copy(other.pDepthStencilAttachment);
    }
    return *this;
}

SubpassDescription::~SubpassDescription()
{
    delete[] pColorAttachments;
    delete pDepthStencilAttachment;
}

std::size_t SubpassDescription::hash() const noexcept
{
    std::size_t hash = core::hashArgs(
        flags,
        pipelineBindPoint,
        inputAttachmentCount,
        colorAttachmentCount,
        preserveAttachmentCount);
    uint32_t i;
    for (i = 0; i < inputAttachmentCount; ++i)
    {
        core::hashCombine(hash, core::hashArgs(
            pInputAttachments[i].attachment,
            pInputAttachments[i].layout));
    }
    for (i = 0; i < colorAttachmentCount; ++i)
    {
        if (pColorAttachments)
        {
            core::hashCombine(hash, core::hashArgs(
                pColorAttachments[i].attachment,
                pColorAttachments[i].layout));
        }
        else if (pResolveAttachments)
        {
            core::hashCombine(hash, core::hashArgs(
                pResolveAttachments[i].attachment,
                pResolveAttachments[i].layout));
        }
    }
    if (pDepthStencilAttachment)
    {
        core::hashCombine(hash, core::hashArgs(
            pDepthStencilAttachment->attachment,
            pDepthStencilAttachment->layout));
    }
    for (i = 0; i < preserveAttachmentCount; ++i)
    {
        core::hashCombine(hash, core::hash(
            pPreserveAttachments[i]));
    }
    return hash;
}

namespace subpasses
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
} // namespace subpasses
} // namespace magma
