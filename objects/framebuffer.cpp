/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../helpers/stackArray.h"

namespace magma
{
Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass, const VkExtent2D& extent,
    uint32_t layerCount, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass)),
    extent(extent),
    layerCount(layerCount)
{}

Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass, std::shared_ptr<ImageView> attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Framebuffer(std::move(renderPass), attachment->getExtent(), attachment->getArrayLayerCount(), std::move(allocator))
{
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = extendedInfo.chainNodes();
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachment->getHandleAddress();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create framebuffer");
    attachments.push_back(attachment);
}

Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass, const std::vector<std::shared_ptr<ImageView>>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Framebuffer(std::move(renderPass), attachments.front()->getExtent(), attachments.front()->getArrayLayerCount(), std::move(allocator))
{
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, attachments.size());
    for (auto& attachment : attachments)
        dereferencedAttachments.put(*attachment);
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = extendedInfo.chainNodes();
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    framebufferInfo.pAttachments = dereferencedAttachments;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create multi-attachment framebuffer");
    this->attachments = attachments;
}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
} // namespace magma
