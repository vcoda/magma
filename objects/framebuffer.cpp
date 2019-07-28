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
#include "framebuffer.h"
#include "device.h"
#include "image.h"
#include "imageView.h"
#include "renderPass.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"
#include "../helpers/stackArray.h"

namespace magma
{
Framebuffer::Framebuffer(std::shared_ptr<RenderPass> renderPass, std::shared_ptr<ImageView> attachment,
    VkFramebufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, std::move(renderPass->getDevice()), std::move(allocator)),
    attachments(attachments),
    extent{attachment->getImage()->getMipExtent(0).width, 
        attachment->getImage()->getMipExtent(0).height}
{
    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.renderPass = *renderPass;
    info.attachmentCount = 1;
    const VkImageView imageView = *attachment;
    info.pAttachments = &imageView;
    info.width = extent.width;
    info.height = extent.height;
    info.layers = 1;
    const VkResult create = vkCreateFramebuffer(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create framebuffer");
}

Framebuffer::Framebuffer(std::shared_ptr<RenderPass> renderPass, const std::vector<std::shared_ptr<ImageView>>& attachments,
    VkFramebufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, std::move(renderPass->getDevice()), std::move(allocator)),
    attachments(std::move(attachments)),
    extent{this->attachments.front()->getImage()->getMipExtent(0).width, 
        this->attachments.front()->getImage()->getMipExtent(0).height}
{
    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.renderPass = *renderPass;
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, this->attachments.size());
    for (auto& attachment : this->attachments)
        dereferencedAttachments.put(*attachment);
    info.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    info.pAttachments = dereferencedAttachments;
    info.width = extent.width;
    info.height = extent.height;
    info.layers = 1;
    const VkResult create = vkCreateFramebuffer(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create framebuffer");
}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
