/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include <algorithm>
#include "renderPass.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../misc/format.h"
#include "../misc/exception.h"

namespace magma
{
RenderPass::RenderPass(std::shared_ptr<Device> device, const AttachmentDescription& attachment,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    RenderPass(std::move(device), {attachment}, std::move(allocator))
{}

RenderPass::RenderPass(std::shared_ptr<Device> device,
    const std::initializer_list<AttachmentDescription>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_RENDER_PASS, std::move(device), std::move(allocator)),
    attachments(attachments)
{
    uint32_t multisampleAttachmentCount = 0;
    uint32_t resolveAttachmentCount = 0;
    for (const auto& attachmentDesc : attachments)
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
    MAGMA_ASSERT(colorAttachmentCount);
    resolveAttachmentCount = std::max(0U, multisampleAttachmentCount);
    MAGMA_STACK_ARRAY(VkAttachmentReference, colorAttachments, colorAttachmentCount);
    MAGMA_STACK_ARRAY(VkAttachmentReference, resolveAttachments, resolveAttachmentCount);
    VkAttachmentReference depthStencilAttachment = {0, VK_IMAGE_LAYOUT_UNDEFINED};
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
    SubpassDescription subpass;
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = colorAttachmentCount;
    subpass.pColorAttachments = colorAttachments;
    subpass.pResolveAttachments = resolveAttachments;
    subpass.pDepthStencilAttachment = &depthStencilAttachment;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;
    SubpassDependency dependencies[2];
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.attachmentCount = MAGMA_COUNT(attachments);
    info.pAttachments = attachments.begin();
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 2;
    info.pDependencies = dependencies;
    const VkResult create = vkCreateRenderPass(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create render pass");
     hash = core::hashArgs(
        info.sType,
        info.flags,
        info.attachmentCount,
        info.subpassCount,
        info.dependencyCount);
    for (const auto& attachment : attachments)
        core::hashCombine(hash, attachment.hash());
    core::hashCombine(hash, subpass.hash()); subpass = {};
    for (const auto& dependency : dependencies)
        core::hashCombine(hash, dependency.hash());
}

RenderPass::RenderPass(std::shared_ptr<Device> device,
    const std::initializer_list<AttachmentDescription>& attachments,
    const std::initializer_list<SubpassDescription>& subpasses,
    const std::initializer_list<SubpassDependency>& dependencies /* {} */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_RENDER_PASS, std::move(device), std::move(allocator))
{
    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.attachmentCount = MAGMA_COUNT(attachments);
    info.pAttachments = attachments.begin();
    info.subpassCount = MAGMA_COUNT(subpasses);
    info.pSubpasses = subpasses.begin();
    info.dependencyCount = MAGMA_COUNT(dependencies);
    info.pDependencies = dependencies.begin();
    const VkResult create = vkCreateRenderPass(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create render pass");
    hash = core::hashArgs(
        info.sType,
        info.flags,
        info.attachmentCount,
        info.subpassCount,
        info.dependencyCount);
    for (const auto& attachment : attachments)
        core::hashCombine(hash, attachment.hash());
    for (const auto& subpass : subpasses)
        core::hashCombine(hash, subpass.hash());
    for (const auto& dependency : dependencies)
        core::hashCombine(hash, dependency.hash());
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

VkImageLayout RenderPass::optimalDepthStencilLayout(const Format& format) const
{
#ifdef VK_KHR_separate_depth_stencil_layouts
    std::shared_ptr<const PhysicalDevice> physicalDevice = device->getPhysicalDevice();
    if (physicalDevice->checkExtensionSupport("VK_KHR_separate_depth_stencil_layouts"))
    {
        if (format.depth())
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR;
        if (format.stencil())
            return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR;
    }
#endif // VK_KHR_separate_depth_stencil_layouts
    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}
} // namespace magma
