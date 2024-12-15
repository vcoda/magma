/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass, const VkExtent2D& extent,
    uint32_t layerCount, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass)),
    extent(extent),
    layerCount(layerCount)
{}

Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass_, std::shared_ptr<ImageView> attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass_->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass_)),
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
    const VkResult result = vkCreateFramebuffer(getNativeDevice(), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create framebuffer");
    attachments.push_back(attachment);
}

Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass_, const std::vector<std::shared_ptr<ImageView>>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass_->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass_)),
    extent(attachments.front()->getExtent2D()),
    layerCount(attachments.front()->getArrayLayerCount())
{
    MAGMA_VLA(VkImageView, dereferencedAttachments, attachments.size());
    for (auto& attachment: attachments)
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
    const VkResult result = vkCreateFramebuffer(getNativeDevice(), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create multi-attachment framebuffer");
    this->attachments = attachments;
}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(getNativeDevice(), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

std::vector<ImageView *> Framebuffer::getAttachments() const
{
    std::vector<ImageView *> views;
    for (auto const& attachment: attachments)
        views.push_back(attachment.get());
    return views;
}
} // namespace magma
