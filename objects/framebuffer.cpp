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
#include "framebuffer.h"
#include "device.h"
#include "image.h"
#include "imageView.h"
#include "renderPass.h"
#include "../helpers/stackArray.h"

namespace magma
{
Framebuffer::Framebuffer(std::shared_ptr<const Device> device, std::shared_ptr<const RenderPass> renderPass, const std::vector<std::shared_ptr<const ImageView>>& attachments,
    VkFramebufferCreateFlags flags /* 0 */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, device),
    attachments(attachments),
    extent(attachments[0]->getImage()->getExtent2D())
{
    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.renderPass = *renderPass;
    MAGMA_STACK_ARRAY(VkImageView, nativeAttachments, attachments.size());
    for (auto& attachment : attachments)
        nativeAttachments.put(*attachment);
    info.attachmentCount = MAGMA_COUNT(nativeAttachments);
    info.pAttachments = nativeAttachments;
    info.width = extent.width;
    info.height = extent.height;
    info.layers = 1;
    const VkResult create = vkCreateFramebuffer(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create framebuffer");
}

Framebuffer::Framebuffer(std::shared_ptr<const Device> device, std::shared_ptr<const RenderPass> renderPass, std::shared_ptr<const ImageView> attachment, 
    VkFramebufferCreateFlags flags /* 0 */):
    Framebuffer(device, renderPass, std::vector<std::shared_ptr<const ImageView>>{attachment}, flags)
{}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(*device, handle, nullptr);
    attachments.clear();
}
} // namespace magma
