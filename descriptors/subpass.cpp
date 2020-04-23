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
Subpass::Subpass(VkSubpassDescriptionFlags flags, VkPipelineBindPoint pipelineBindPoint) noexcept
{
    this->flags = flags;
    this->pipelineBindPoint = pipelineBindPoint;
    inputAttachmentCount = 0;
    pInputAttachments = nullptr;
    colorAttachmentCount = 0;
    pColorAttachments = nullptr;
    pResolveAttachments = nullptr;
    pDepthStencilAttachment = nullptr;
    preserveAttachmentCount = 0;
    pPreserveAttachments = nullptr;
}

Subpass::Subpass(const Subpass& other) noexcept
{
    core::copy(this, &other);
    if (other.pColorAttachments)
        pColorAttachments = core::copyArray(other.pColorAttachments, colorAttachmentCount);
    if (other.pDepthStencilAttachment)
        pDepthStencilAttachment = core::copy(other.pDepthStencilAttachment);
}

Subpass& Subpass::operator=(const Subpass& other) noexcept
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

Subpass::~Subpass()
{
    delete[] pColorAttachments;
    delete pDepthStencilAttachment;
}

std::size_t Subpass::hash() const noexcept
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

GraphicsSubpass::GraphicsSubpass(VkImageLayout colorLayout):
    Subpass(0, VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    VkAttachmentReference *colorReference = new VkAttachmentReference[1];
    colorReference->attachment = 0;
    colorReference->layout = colorLayout;
    colorAttachmentCount = 1;
    pColorAttachments = colorReference;
}

GraphicsSubpass::GraphicsSubpass(VkImageLayout colorLayout, VkImageLayout depthStencilLayout):
    Subpass(0, VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    if (colorLayout != VK_IMAGE_LAYOUT_UNDEFINED)
    {
        VkAttachmentReference *colorReference = new VkAttachmentReference[1];
        colorReference->attachment = 0;
        colorReference->layout = colorLayout;
        colorAttachmentCount = 1;
        pColorAttachments = colorReference;
    }
    VkAttachmentReference *depthStencilReference = new VkAttachmentReference;
    depthStencilReference->attachment = 1;
    depthStencilReference->layout = depthStencilLayout;
    pDepthStencilAttachment = depthStencilReference;
}

GraphicsSubpass::GraphicsSubpass(const std::vector<VkImageLayout>& colorLayouts):
    Subpass(0, VK_PIPELINE_BIND_POINT_GRAPHICS)
{
    if (!colorLayouts.empty())
    {
        VkAttachmentReference *colorReferences = new VkAttachmentReference[colorLayouts.size()];
        for (auto layout : colorLayouts)
        {
            colorReferences[colorAttachmentCount].attachment = colorAttachmentCount;
            colorReferences[colorAttachmentCount].layout = layout;
            ++colorAttachmentCount;
        }
        pColorAttachments = colorReferences;
    }
}

GraphicsSubpass::GraphicsSubpass(const std::vector<VkImageLayout>& colorLayouts, const VkImageLayout& depthStencilLayout):
    GraphicsSubpass(colorLayouts)
{
    VkAttachmentReference *depthStencilReference = new VkAttachmentReference;
    depthStencilReference->attachment = colorAttachmentCount;
    depthStencilReference->layout = depthStencilLayout;
    pDepthStencilAttachment = depthStencilReference;
}

ComputeSubpass::ComputeSubpass() noexcept:
    Subpass(0, VK_PIPELINE_BIND_POINT_COMPUTE)
{}

namespace subpasses
{
const GraphicsSubpass colorAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorDepthStencilAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorAttachmentDepthStencilReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
const GraphicsSubpass depthStencilAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass depthStencilReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

#ifdef VK_KHR_maintenance2
const GraphicsSubpass colorStencilAttachmentDepthReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const GraphicsSubpass colorDepthAttachmentStencilReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR);
const GraphicsSubpass depthReadOnlyStencilAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const GraphicsSubpass depthAttachmentStencilReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR);
#endif // VK_KHR_maintenance2

#ifdef VK_KHR_separate_depth_stencil_layouts
const GraphicsSubpass colorDepthAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR);
const GraphicsSubpass colorStencilAttachment(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const GraphicsSubpass colorAttachmentDepthReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR);
const GraphicsSubpass colorAttachmentStencilReadOnly(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR);
const GraphicsSubpass depthAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR);
const GraphicsSubpass stencilAttachment(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR);
const GraphicsSubpass depthReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR);
const GraphicsSubpass stencilReadOnly(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR);
#endif // VK_KHR_separate_depth_stencil_layouts
} // namespace subpasses
} // namespace magma
