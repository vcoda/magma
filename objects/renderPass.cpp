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
#include "renderPass.h"
#include "device.h"

namespace magma
{
RenderPass::RenderPass(std::shared_ptr<const Device> device, const std::vector<Attachment>& attachments, const Subpass& subpass):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, device)
{
    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.attachmentCount = MAGMA_COUNT(attachments);
    info.pAttachments = attachments.front();
    info.subpassCount = 1;
    info.pSubpasses = subpass;
    info.dependencyCount = 0;
    info.pDependencies = nullptr;
    const VkResult create = vkCreateRenderPass(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create render pass");
}

RenderPass::RenderPass(std::shared_ptr<const Device> device, const Attachment& attachment, const Subpass& subpass):
    RenderPass(device, std::vector<Attachment>{attachment}, subpass)
{}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(*device, handle, nullptr);
}
} // namespace magma
