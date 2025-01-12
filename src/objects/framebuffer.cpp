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
#include "framebuffer.h"
#include "device.h"
#include "image.h"
#include "imageView.h"
#include "renderPass.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Framebuffer::Framebuffer(std::shared_ptr<Device> device, const VkExtent2D& extent, uint32_t layerCount,
    std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, std::move(device), std::move(allocator)),
    extent(extent),
    layerCount(layerCount)
{}

Framebuffer::Framebuffer(lent_ptr<const RenderPass> renderPass, std::shared_ptr<ImageView> attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    extent(attachment->getExtent2D()),
    layerCount(attachment->getArrayLayerCount())
{
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = extendedInfo.headNode();
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = *renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachment->getHandleAddress();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(getNativeDevice(), &framebufferInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create framebuffer");
    attachments.emplace_back(std::move(attachment));
}

Framebuffer::Framebuffer(lent_ptr<const RenderPass> renderPass, const std::vector<std::shared_ptr<ImageView>>& attachments_,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    extent(attachments_.front()->getExtent2D()),
    layerCount(attachments_.front()->getArrayLayerCount())
{
    MAGMA_VLA(VkImageView, dereferencedAttachments, attachments_.size());
    for (auto& attachment: attachments_)
        dereferencedAttachments.put(*attachment);
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = extendedInfo.headNode();
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = *renderPass;
    framebufferInfo.attachmentCount = dereferencedAttachments.count();
    framebufferInfo.pAttachments = dereferencedAttachments;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(getNativeDevice(), &framebufferInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create multi-attachment framebuffer");
    attachments = attachments_;
}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(getNativeDevice(), handle, MAGMA_OPTIONAL(hostAllocator));
}
} // namespace magma
