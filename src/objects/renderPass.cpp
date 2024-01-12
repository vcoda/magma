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
#include "renderPass.h"
#include "framebuffer.h"
#include "image.h"
#include "imageView.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../misc/deviceFeatures.h"
#include "../misc/format.h"
#include "../exceptions/errorResult.h"
#include "../core/foreach.h"

namespace magma
{
RenderPass::RenderPass(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator,
    const std::vector<AttachmentDescription>& attachments):
    NonDispatchable(VK_OBJECT_TYPE_RENDER_PASS, std::move(device), std::move(allocator)),
    attachments(attachments),
    hash(0ull)
{}

RenderPass::RenderPass(std::shared_ptr<Device> device, const AttachmentDescription& attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */, const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::vector<AttachmentDescription>{attachment}, std::move(allocator), extendedInfo)
{}

RenderPass::RenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */, const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::move(allocator), attachments)
{
    uint32_t multisampleAttachmentCount = 0;
    uint32_t colorAttachmentCount = 0;
    for (auto const& attachmentDesc: attachments)
    {
        const Format format(attachmentDesc.format);
        if (!format.depth() && !format.stencil() && !format.depthStencil())
        {
            if (attachmentDesc.samples > 1)
                ++multisampleAttachmentCount;
            else
                ++colorAttachmentCount;
        }
    }
    const uint32_t resolveAttachmentCount = multisampleAttachmentCount ? colorAttachmentCount : 0;
    colorAttachmentCount = std::max(multisampleAttachmentCount, colorAttachmentCount); // Any non-depth attachment
    MAGMA_STACK_ARRAY(VkAttachmentReference, colorAttachments, colorAttachmentCount);
    MAGMA_STACK_ARRAY(VkAttachmentReference, resolveAttachments, resolveAttachmentCount);
    VkAttachmentReference depthStencilAttachment = {0, VK_IMAGE_LAYOUT_UNDEFINED};
    bool hasDepthStencilAttachment = false;
    uint32_t attachmentIndex = 0, colorIndex = 0, resolveIndex = 0;
    for (const auto& attachmentDesc : attachments)
    {
        const Format format(attachmentDesc.format);
        if (format.depth() || format.stencil() || format.depthStencil())
        {
            if (VK_IMAGE_LAYOUT_UNDEFINED == depthStencilAttachment.layout)
            {
                const VkImageLayout depthStencilLayout = optimalDepthStencilLayout(format);
                depthStencilAttachment = {attachmentIndex, depthStencilLayout};
                hasDepthStencilAttachment = true;
            }
        }
        else
        {
            if (attachmentDesc.samples > 1 || resolveAttachmentCount < 1)
                colorAttachments[colorIndex++] = {attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
            else
                resolveAttachments[resolveIndex++] = {attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
        }
        ++attachmentIndex;
    }
    // Describe render pass
    SubpassDescription subpassDescription;
    subpassDescription.flags = 0;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.colorAttachmentCount = colorAttachmentCount;
    subpassDescription.pColorAttachments = colorAttachments;
    subpassDescription.pResolveAttachments = resolveAttachments;
    subpassDescription.pDepthStencilAttachment = hasDepthStencilAttachment ? &depthStencilAttachment : nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;
    SubpassDependency dependencies[] = {
        // Dependency at the beginning of the render pass
        subpassBeginDependency(colorAttachmentCount > 0, hasDepthStencilAttachment),
        // Dependency at the end of the render pass
        subpassEndDependency(colorAttachmentCount > 0, hasDepthStencilAttachment)
    };
    // Create render pass
    VkRenderPassCreateInfo renderPassInfo;
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = extendedInfo.chainNodes();
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = MAGMA_COUNT(attachments);
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = 2;
    renderPassInfo.pDependencies = dependencies;
    const VkResult result = vkCreateRenderPass(MAGMA_HANDLE(device), &renderPassInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create render pass");
    hash = core::hashArgs(
        renderPassInfo.sType,
        renderPassInfo.flags,
        renderPassInfo.attachmentCount,
        renderPassInfo.subpassCount,
        renderPassInfo.dependencyCount,
        extendedInfo.getHash());
    for (const auto& attachment : attachments)
        hash = core::hashCombine(hash, attachment.hash());
    hash = core::hashCombine(hash, subpassDescription.getHash());
    core::zeroMemory(subpassDescription); // Aware destructor
    for (const auto& dependency : dependencies)
        hash = core::hashCombine(hash, dependency.hash());
}

RenderPass::RenderPass(std::shared_ptr<Device> device,
    const std::vector<AttachmentDescription>& attachments,
    const std::vector<SubpassDescription>& subpasses,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), attachments, subpasses, {}, std::move(allocator), extendedInfo)
{}

RenderPass::RenderPass(std::shared_ptr<Device> device,
    const std::vector<AttachmentDescription>& attachments,
    const std::vector<SubpassDescription>& subpasses,
    const std::vector<SubpassDependency>& dependencies,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_RENDER_PASS, std::move(device), std::move(allocator)),
    hash(0)
{
    VkRenderPassCreateInfo renderPassInfo;
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = extendedInfo.chainNodes();
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = MAGMA_COUNT(attachments);
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = MAGMA_COUNT(subpasses);
    renderPassInfo.pSubpasses = subpasses.data();
    renderPassInfo.dependencyCount = MAGMA_COUNT(dependencies);
    renderPassInfo.pDependencies = dependencies.data();
    const VkResult result = vkCreateRenderPass(MAGMA_HANDLE(device), &renderPassInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create render pass");
    hash = core::hashArgs(
        renderPassInfo.sType,
        renderPassInfo.flags,
        renderPassInfo.attachmentCount,
        renderPassInfo.subpassCount,
        renderPassInfo.dependencyCount,
        extendedInfo.getHash());
    for (const auto& attachment : attachments)
        hash = core::hashCombine(hash, attachment.hash());
    for (const auto& subpass : subpasses)
        hash = core::hashCombine(hash, subpass.getHash());
    for (const auto& dependency : dependencies)
        hash = core::hashCombine(hash, dependency.hash());
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool RenderPass::hasClearOp() const noexcept
{
    for (const auto& attachment : attachments)
    {
        if (VK_ATTACHMENT_LOAD_OP_CLEAR == attachment.loadOp)
            return true;
    }
    return false;
}

VkImageLayout RenderPass::optimalDepthStencilLayout(const Format& format) const
{
#ifdef VK_KHR_separate_depth_stencil_layouts
    if (device->getDeviceFeatures()->separateDepthStencilLayoutsEnabled())
    {
        if (format.depth())
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR;
        if (format.stencil())
            return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR;
    }
#else
    MAGMA_UNUSED(format);
#endif // VK_KHR_separate_depth_stencil_layouts
    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}

SubpassDependency RenderPass::subpassBeginDependency(bool colorAttachment, bool depthStencilAttachment) const noexcept
{
    SubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    subpassDependency.dstStageMask = 0;
    subpassDependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    subpassDependency.dstAccessMask = 0;
    if (colorAttachment)
    {
        subpassDependency.dstStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (depthStencilAttachment)
    {
        subpassDependency.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    return subpassDependency;
}

SubpassDependency RenderPass::subpassEndDependency(bool colorAttachment, bool depthStencilAttachment) const noexcept
{
    SubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    if (colorAttachment)
    {
        subpassDependency.srcStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (depthStencilAttachment)
    {
        subpassDependency.srcStageMask |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        subpassDependency.srcAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    return subpassDependency;
}

void RenderPass::begin(std::shared_ptr<Framebuffer> framebuffer) const noexcept
{
    core::forConstEach(framebuffer->getAttachments(), attachments,
        [](auto& attachment, auto& attachmentDesc)
        {   // initialLayout is the layout the attachment image subresource
            // will be in when a render pass instance begins
            const std::shared_ptr<Image>& image = (*attachment)->getImage();
            image->setLayout(attachmentDesc->initialLayout);
        });
}

void RenderPass::end(std::shared_ptr<Framebuffer> framebuffer) const noexcept
{
    core::forConstEach(framebuffer->getAttachments(), attachments,
        [](auto& attachment, auto& attachmentDesc)
        {   // finalLayout is the layout the attachment image subresource
            // will be transitioned to when a render pass instance ends.
            const std::shared_ptr<Image>& image = (*attachment)->getImage();
            image->setLayout(attachmentDesc->finalLayout);
        });
}
} // namespace magma
