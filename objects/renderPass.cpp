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
#include <algorithm>
#include "renderPass.h"
#include "device.h"
#include "../misc/format.h"
#include "../helpers/stackArray.h"

namespace magma
{
RenderPass::RenderPass(std::shared_ptr<const Device> device, const AttachmentDescription& attachment):
    RenderPass(device, {attachment})
{}

RenderPass::RenderPass(std::shared_ptr<const Device> device, 
    const std::initializer_list<AttachmentDescription>& attachments):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, device)
{
    // Count multisample and single sample attachments
    uint32_t multisampleAttachments = 0;
    uint32_t resolveAttachments = 0;
    for (const auto& desc : attachments)
    {
        if (!Format(desc.format).depth())
        {
            if (desc.samples > 1)
                ++multisampleAttachments;
            else
                ++resolveAttachments;
        }
    }
    // In a typical usage scenario we could fill subpass descriptor by himself
    VkSubpassDescription subpass;
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    if (0 == multisampleAttachments)
        subpass.colorAttachmentCount = resolveAttachments;
    else
        subpass.colorAttachmentCount = multisampleAttachments;
    resolveAttachments = std::max(0U, multisampleAttachments);
    MAGMA_ASSERT(subpass.colorAttachmentCount);
    MAGMA_STACK_ARRAY(VkAttachmentReference, colorReferences, subpass.colorAttachmentCount);
    MAGMA_STACK_ARRAY(VkAttachmentReference, resolveReferences, resolveAttachments);
    VkAttachmentReference *depthStencilReference = nullptr;
    uint32_t attachmentIndex = 0;
    uint32_t colorIndex = 0;
    uint32_t resolveIndex = 0;
    // Color/depth/multisample attachments go to their slots
    for (const auto& desc : attachments)
    {
        if (Format(desc.format).depth())
        {
            if (!depthStencilReference)
            {
                depthStencilReference = new VkAttachmentReference;
                depthStencilReference->attachment = attachmentIndex;
                depthStencilReference->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }
        }
        else
        {
            if (resolveAttachments < 1 || desc.samples > 1)
            {
                colorReferences[colorIndex].attachment = attachmentIndex;
                colorReferences[colorIndex].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                ++colorIndex;
            }
            else
            {
                resolveReferences[resolveIndex].attachment = attachmentIndex;
                resolveReferences[resolveIndex].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                ++resolveIndex;
            }
        }
        ++attachmentIndex;
    }
    subpass.pColorAttachments = colorReferences;
    subpass.pResolveAttachments = resolveReferences;
    subpass.pDepthStencilAttachment = depthStencilReference;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;
    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.attachmentCount = MAGMA_COUNT(attachments);
    info.pAttachments = attachments.begin();
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 0;
    info.pDependencies = nullptr;
    const VkResult create = vkCreateRenderPass(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create render pass");
}

RenderPass::RenderPass(std::shared_ptr<const Device> device,
    const std::initializer_list<AttachmentDescription>& attachments,
    const std::initializer_list<Subpass>& subpasses):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, device)
{
    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.attachmentCount = MAGMA_COUNT(attachments);
    info.pAttachments = attachments.begin();
    info.subpassCount = MAGMA_COUNT(subpasses);
    info.pSubpasses = subpasses.begin();
    info.dependencyCount = 0;
    info.pDependencies = nullptr;
    const VkResult create = vkCreateRenderPass(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create render pass");
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(*device, handle, nullptr);
}
} // namespace magma
