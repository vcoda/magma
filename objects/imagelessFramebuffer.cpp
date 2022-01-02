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
#include "imagelessFramebuffer.h"

namespace magma
{
#ifdef VK_KHR_imageless_framebuffer
class FramebufferAttachmentsCreateInfo : public CreateInfo
{
public:
    FramebufferAttachmentsCreateInfo(uint32_t width, uint32_t height, uint32_t layerCount,
        VkImageCreateFlags flags, VkImageUsageFlags usage,
        const std::vector<VkFormat>& viewFormats,
        const CreateInfo& chainedInfo = CreateInfo()) noexcept:
        viewFormats(viewFormats)
    {
        MAGMA_ASSERT(!viewFormats.empty());
        VkFramebufferAttachmentImageInfoKHR attachmentInfo;
        attachmentInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO_KHR;
        attachmentInfo.pNext = chainedInfo.getNode();
        attachmentInfo.flags = flags;
        attachmentInfo.usage = usage;
        attachmentInfo.width = width;
        attachmentInfo.height = height;
        attachmentInfo.layerCount = layerCount;
        attachmentInfo.viewFormatCount = MAGMA_COUNT(this->viewFormats);
        attachmentInfo.pViewFormats = this->viewFormats.data();
        attachmentInfos.push_back(attachmentInfo);
        framebufferAttachmentsInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
        framebufferAttachmentsInfo.pNext = chainedInfo.getNode();
        framebufferAttachmentsInfo.attachmentImageInfoCount = 1;
        framebufferAttachmentsInfo.pAttachmentImageInfos = attachmentInfos.data();
    }

    FramebufferAttachmentsCreateInfo(const std::vector<ImagelessFramebuffer::AttachmentImageInfo>& attachments,
        const CreateInfo& chainedInfo = CreateInfo()) noexcept
    {
        for (const auto& info : attachments)
        {
            MAGMA_ASSERT(!info.viewFormats.empty());
            VkFramebufferAttachmentImageInfoKHR attachmentInfo;
            attachmentInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO_KHR;
            attachmentInfo.pNext = chainedInfo.getNode();
            attachmentInfo.flags = info.flags;
            attachmentInfo.usage = info.usage;
            attachmentInfo.width = info.width;
            attachmentInfo.height = info.height;
            attachmentInfo.layerCount = info.layerCount;
            attachmentInfo.viewFormatCount = MAGMA_COUNT(info.viewFormats);
            attachmentInfo.pViewFormats = info.viewFormats.data();
            attachmentInfos.push_back(attachmentInfo);
        }
        framebufferAttachmentsInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO_KHR;
        framebufferAttachmentsInfo.pNext = chainedInfo.getNode();
        framebufferAttachmentsInfo.attachmentImageInfoCount = MAGMA_COUNT(attachmentInfos);
        framebufferAttachmentsInfo.pAttachmentImageInfos = attachmentInfos.data();
    }

    const void *getNode() const noexcept override
    {
        return &framebufferAttachmentsInfo;
    }

private:
    const std::vector<VkFormat> viewFormats;
    std::vector<VkFramebufferAttachmentImageInfoKHR> attachmentInfos;
    VkFramebufferAttachmentsCreateInfoKHR framebufferAttachmentsInfo;
};

ImagelessFramebuffer::ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass,
    uint32_t width, uint32_t height, uint32_t layerCount, VkImageUsageFlags usage,
    const std::vector<VkFormat>& viewFormats,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkImageCreateFlags flags /* 0 */,
    const CreateInfo& chainedInfo /* default */):
    Framebuffer(std::move(renderPass),
        width,
        height,
        layerCount,
        1,
        std::move(allocator),
        VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT_KHR,
        FramebufferAttachmentsCreateInfo(width, height, layerCount, flags, usage, viewFormats, chainedInfo))
{}

ImagelessFramebuffer::ImagelessFramebuffer(std::shared_ptr<const RenderPass> renderPass, const std::vector<AttachmentImageInfo>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const CreateInfo& chainedInfo /* default */):
    Framebuffer(std::move(renderPass),
        attachments.front().width,
        attachments.front().height,
        attachments.front().layerCount,
        MAGMA_COUNT(attachments),
        std::move(allocator),
        VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT_KHR,
        FramebufferAttachmentsCreateInfo(attachments, chainedInfo))
{}
#endif // VK_KHR_imageless_framebuffer
} // namespace magma
