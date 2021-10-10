/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass,
    uint32_t width, uint32_t height, uint32_t layerCount, uint32_t attachmentCount,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */,
    const CreateInfo& chainedInfo /* CreateInfo() */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass)),
    extent({width, height}),
    layerCount(layerCount)
{
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = chainedInfo.getNode();
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = attachmentCount;
    framebufferInfo.pAttachments = nullptr;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create imageless framebuffer");
}

Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass, std::shared_ptr<ImageView> attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass)),
    attachments({attachment}),
    extent(attachment->getExtent()),
    layerCount(attachment->getArrayLayerCount())
{
    const VkImageView imageView = *attachment;
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = nullptr;
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &imageView;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create framebuffer");
}

Framebuffer::Framebuffer(std::shared_ptr<const RenderPass> renderPass, const std::vector<std::shared_ptr<ImageView>>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkFramebufferCreateFlags flags /* 0 */):
    NonDispatchable(VK_OBJECT_TYPE_FRAMEBUFFER, renderPass->getDevice(), std::move(allocator)),
    renderPass(std::move(renderPass)),
    attachments(attachments),
    extent(attachments.front()->getExtent()),
    layerCount(attachments.front()->getArrayLayerCount())
{
    MAGMA_STACK_ARRAY(VkImageView, dereferencedAttachments, this->attachments.size());
    for (auto& attachment : this->attachments)
        dereferencedAttachments.put(*attachment);
    VkFramebufferCreateInfo framebufferInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = nullptr;
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = MAGMA_COUNT(dereferencedAttachments);
    framebufferInfo.pAttachments = dereferencedAttachments;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create framebuffer");
}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}
} // namespace magma
