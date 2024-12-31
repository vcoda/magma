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
#include "multiviewRenderPass.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/format.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_multiview
MultiviewRenderPass::MultiviewRenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    uint32_t viewMask, uint32_t correlationMask,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    MultiviewRenderPass(std::move(device), attachments, {viewMask}, {/* viewOffsets */}, {correlationMask}, std::move(allocator), extendedInfo)
{}

MultiviewRenderPass::MultiviewRenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    const std::vector<uint32_t>& viewMasks, const std::vector<int32_t> viewOffsets, const std::vector<uint32_t>& correlationMasks,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::move(allocator), attachments, 0),
    viewMasks(viewMasks),
    viewOffsets(viewOffsets),
    correlationMasks(correlationMasks)
{
    uint32_t multisampleAttachmentCount = 0;
    uint32_t resolveAttachmentCount = 0;
    for (auto const& attachmentDesc: attachments)
    {
        const Format format(attachmentDesc.format);
        if (!format.depth() && !format.stencil() && !format.depthStencil())
        {
            if (attachmentDesc.samples > 1)
                ++multisampleAttachmentCount;
            else
                ++resolveAttachmentCount;
        }
    }
    const uint32_t colorAttachmentCount = multisampleAttachmentCount ? multisampleAttachmentCount : resolveAttachmentCount;
    resolveAttachmentCount = std::max(0U, multisampleAttachmentCount);
    MAGMA_VLA(VkAttachmentReference, colorAttachments, colorAttachmentCount);
    MAGMA_VLA(VkAttachmentReference, resolveAttachments, resolveAttachmentCount);
    VkAttachmentReference depthStencilAttachment = {0, VK_IMAGE_LAYOUT_UNDEFINED};
    bool hasDepthStencilAttachment = false;
    uint32_t attachmentIndex = 0, colorIndex = 0, resolveIndex = 0;
    for (auto const& attachmentDesc: attachments)
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
    subpassDescription.colorAttachmentCount = colorAttachmentCount;
    subpassDescription.pColorAttachments = colorAttachments;
    subpassDescription.pResolveAttachments = resolveAttachments;
    subpassDescription.pDepthStencilAttachment = hasDepthStencilAttachment ? &depthStencilAttachment : nullptr;
    SubpassDependency dependencies[] = {
        // Dependency at the beginning of the render pass
        subpassBeginDependency(colorAttachmentCount > 0, hasDepthStencilAttachment),
        // Dependency at the end of the render pass
        subpassEndDependency(colorAttachmentCount > 0, hasDepthStencilAttachment)
    };
    // Create render pass
    VkRenderPassCreateInfo renderPassInfo;
    VkRenderPassMultiviewCreateInfoKHR renderPassMultiviewInfo;
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = &renderPassMultiviewInfo;
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = core::countof(attachments);
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = 2;
    renderPassInfo.pDependencies = dependencies;
    renderPassMultiviewInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO_KHR;
    renderPassMultiviewInfo.pNext = extendedInfo.headNode();
    renderPassMultiviewInfo.subpassCount = core::countof(this->viewMasks);
    renderPassMultiviewInfo.pViewMasks = this->viewMasks.data();
    renderPassMultiviewInfo.dependencyCount = core::countof(this->viewOffsets);
    renderPassMultiviewInfo.pViewOffsets = this->viewOffsets.data();
    renderPassMultiviewInfo.correlationMaskCount = core::countof(this->correlationMasks);
    renderPassMultiviewInfo.pCorrelationMasks = this->correlationMasks.data();
    const VkResult result = vkCreateRenderPass(getNativeDevice(), &renderPassInfo, MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create multiview render pass");
    hash = core::hashArgs(
        renderPassInfo.sType,
        renderPassInfo.flags,
        renderPassInfo.attachmentCount,
        renderPassInfo.subpassCount,
        renderPassInfo.dependencyCount,
        renderPassMultiviewInfo.sType,
        renderPassMultiviewInfo.subpassCount,
        renderPassMultiviewInfo.dependencyCount,
        renderPassMultiviewInfo.correlationMaskCount,
        extendedInfo.hash());
    for (auto const& attachment: attachments)
        hash = core::hashCombine(hash, attachment.hash());
    hash = core::hashCombine(hash, subpassDescription.getHash());
    for (auto const& dependency: dependencies)
        hash = core::hashCombine(hash, dependency.hash());
    for (uint32_t i = 0; i < renderPassMultiviewInfo.subpassCount; ++i)
        hash = core::hashCombine(hash, renderPassMultiviewInfo.pViewMasks[i]);
    for (uint32_t i = 0; i < renderPassMultiviewInfo.dependencyCount; ++i)
        hash = core::hashCombine(hash, renderPassMultiviewInfo.pViewOffsets[i]);
    for (uint32_t i = 0; i < renderPassMultiviewInfo.correlationMaskCount; ++i)
        hash = core::hashCombine(hash, renderPassMultiviewInfo.pCorrelationMasks[i]);
    subpassDescription.pColorAttachments = nullptr;
    subpassDescription.pResolveAttachments = nullptr;
    subpassDescription.pDepthStencilAttachment = nullptr;
}
#endif // VK_KHR_multiview
} // namespace magma
