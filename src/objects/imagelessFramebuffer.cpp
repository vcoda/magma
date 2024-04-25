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
#include "imagelessFramebuffer.h"
#include "device.h"
#include "renderPass.h"
#include "../misc/framebufferAttachmentImage.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_imageless_framebuffer
ImagelessFramebuffer::ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass,
    uint32_t width, uint32_t height, uint32_t layerCount, VkImageUsageFlags usage,
    const std::vector<VkFormat>& viewFormats,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkImageCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    Framebuffer(std::move(renderPass), VkExtent2D{width, height}, layerCount, std::move(allocator))
{
    VkFramebufferCreateInfo framebufferInfo;
    VkFramebufferAttachmentsCreateInfoKHR framebufferAttachmentsInfo;
    VkFramebufferAttachmentImageInfoKHR framebufferAttachmentImageInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = &framebufferAttachmentsInfo;
    framebufferInfo.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT_KHR;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = nullptr;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = layerCount;
    framebufferAttachmentsInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
    framebufferAttachmentsInfo.pNext = extendedInfo.getChain();
    framebufferAttachmentsInfo.attachmentImageInfoCount = framebufferInfo.attachmentCount;
    framebufferAttachmentsInfo.pAttachmentImageInfos = &framebufferAttachmentImageInfo;
    framebufferAttachmentImageInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO_KHR;
    framebufferAttachmentImageInfo.pNext = nullptr;
    framebufferAttachmentImageInfo.flags = flags;
    framebufferAttachmentImageInfo.usage = usage;
    framebufferAttachmentImageInfo.width = width;
    framebufferAttachmentImageInfo.height = height;
    framebufferAttachmentImageInfo.layerCount = layerCount;
    framebufferAttachmentImageInfo.viewFormatCount = MAGMA_COUNT(viewFormats);
    framebufferAttachmentImageInfo.pViewFormats = viewFormats.data();
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create imageless framebuffer");
}

ImagelessFramebuffer::ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass, const FramebufferAttachmentImage& attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    Framebuffer(std::move(renderPass), attachment.getExtent(), attachment.layerCount, std::move(allocator))
{
    VkFramebufferCreateInfo framebufferInfo;
    VkFramebufferAttachmentsCreateInfoKHR framebufferAttachmentsInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = &framebufferAttachmentsInfo;
    framebufferInfo.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT_KHR;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = nullptr;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    framebufferAttachmentsInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
    framebufferAttachmentsInfo.pNext = extendedInfo.getChain();
    framebufferAttachmentsInfo.attachmentImageInfoCount = framebufferInfo.attachmentCount;
    framebufferAttachmentsInfo.pAttachmentImageInfos = &attachment;
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create imageless framebuffer");
}

ImagelessFramebuffer::ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass, const std::vector<FramebufferAttachmentImage>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    Framebuffer(std::move(renderPass), attachments.front().getExtent(), attachments.front().layerCount, std::move(allocator))
{
    VkFramebufferCreateInfo framebufferInfo;
    VkFramebufferAttachmentsCreateInfoKHR framebufferAttachmentsInfo;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = &framebufferAttachmentsInfo;
    framebufferInfo.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT_KHR;
    framebufferInfo.renderPass = MAGMA_HANDLE(renderPass);
    framebufferInfo.attachmentCount = MAGMA_COUNT(attachments);
    framebufferInfo.pAttachments = nullptr;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = layerCount;
    framebufferAttachmentsInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
    framebufferAttachmentsInfo.pNext = extendedInfo.getChain();
    framebufferAttachmentsInfo.attachmentImageInfoCount = framebufferInfo.attachmentCount;
    framebufferAttachmentsInfo.pAttachmentImageInfos = attachments.data();
    const VkResult result = vkCreateFramebuffer(MAGMA_HANDLE(device), &framebufferInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create multi-attachment imageless framebuffer");
}
#endif // VK_KHR_imageless_framebuffer
} // namespace magma
