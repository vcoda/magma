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
#include "subpass.h"
#include "../helpers/copy.h"

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
    helpers::copy(this, &other);
    if (other.pColorAttachments)
        pColorAttachments = helpers::copyArray(other.pColorAttachments, colorAttachmentCount);
    if (other.pDepthStencilAttachment)
    {
        try {
            pDepthStencilAttachment = helpers::copy(other.pDepthStencilAttachment);
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
        helpers::copy(this, &other);
        if (other.pColorAttachments)
            pColorAttachments = helpers::copyArray(other.pColorAttachments, colorAttachmentCount);
        if (other.pDepthStencilAttachment)
        {
            try {
                pDepthStencilAttachment = helpers::copy(other.pDepthStencilAttachment);
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
    VkAttachmentReference *colorReference = new VkAttachmentReference[1];
    colorReference->attachment = 0;
    colorReference->layout = colorLayout;
    VkAttachmentReference *depthStencilReference = new VkAttachmentReference;
    depthStencilReference->attachment = 1;
    depthStencilReference->layout = depthStencilLayout;
    colorAttachmentCount = 1;
    pColorAttachments = colorReference;
    pDepthStencilAttachment = depthStencilReference;
}

ComputeSubpass::ComputeSubpass():
    Subpass(0, VK_PIPELINE_BIND_POINT_COMPUTE)
{}

namespace subpasses
{
const GraphicsSubpass colorOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorOptimalDepthStencilOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
const GraphicsSubpass colorOptimalDepthStencilReadOnlyOptimal(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
} // namespace subpasses
} // namespace magma
