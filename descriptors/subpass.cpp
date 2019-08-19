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
#include "subpass.h"
#include "../internal/copy.h"

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

Subpass::Subpass(const Subpass& other)
{
    internal::copy(this, &other);
    if (other.pColorAttachments)
        pColorAttachments = internal::copyArray(other.pColorAttachments, colorAttachmentCount);
    if (other.pDepthStencilAttachment)
    {
        try {
            pDepthStencilAttachment = internal::copy(other.pDepthStencilAttachment);
        } catch (const std::bad_alloc& exc)
        {
            delete[] pColorAttachments;
            throw exc;
        }
    }
}

Subpass& Subpass::operator=(const Subpass& other)
{
    if (this != &other)
    {
        internal::copy(this, &other);
        if (other.pColorAttachments)
            pColorAttachments = internal::copyArray(other.pColorAttachments, colorAttachmentCount);
        if (other.pDepthStencilAttachment)
        {
            try {
                pDepthStencilAttachment = internal::copy(other.pDepthStencilAttachment);
            } catch (const std::bad_alloc& exc)
            {
                delete[] pColorAttachments;
                throw exc;
            }
        }
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
    std::size_t hash = internal::hashArgs(
        flags,
        pipelineBindPoint,
        inputAttachmentCount,
        colorAttachmentCount,
        preserveAttachmentCount
    );
    uint32_t i;
    for (i = 0; i < inputAttachmentCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pInputAttachments[i].attachment,
            pInputAttachments[i].layout
        ));
    }
    for (i = 0; i < colorAttachmentCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pColorAttachments[i].attachment,
            pColorAttachments[i].layout
        ));
    }
    if (pDepthStencilAttachment)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pDepthStencilAttachment->attachment,
            pDepthStencilAttachment->layout
        ));
    }
    for (i = 0; i < preserveAttachmentCount; ++i)
        internal::hashCombine(hash, internal::hash(pPreserveAttachments[i]));
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
    GraphicsSubpass(colorLayout)
{
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
const GraphicsSubpass colorOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorOptimalDepthStencilOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorOptimalDepthReadOnlyStencilOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorOptimalDepthOptimalStencilReadOnlyOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL);

const GraphicsSubpass depthStencilOptimal({}, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass depthReadOnlyStencilOptimal({}, VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass depthOptimalStencilReadOnlyOptimal({}, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL);
} // namespace subpasses
} // namespace magma
