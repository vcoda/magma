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
#include "image.h"
#include "imageView.h"
#include "device.h"
#include "physicalDevice.h"
#include "../allocator/allocator.h"
#include "../helpers/stackArray.h"
#include "../misc/deviceFeatures.h"
#include "../misc/format.h"
#include "../exceptions/errorResult.h"
#include "../helpers/streamInsertOperators.h"
#include "../helpers/stringifyFlags.h"
#include "../core/foreach.h"

namespace magma
{
RenderPass::RenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkRenderPassCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::move(allocator), attachments, flags)
{
    uint32_t multisampleAttachmentCount = 0;
    uint32_t colorAttachmentCount = 0;
    for (auto const& attachment: attachments)
    {
        const Format format(attachment.format);
        if (!format.depth() && !format.stencil() && !format.depthStencil())
        {
            if (attachment.samples > 1)
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
    for (auto const& attachment: attachments)
    {
        const Format format(attachment.format);
        if (format.depth() || format.stencil() || format.depthStencil())
        {
            if (VK_IMAGE_LAYOUT_UNDEFINED == depthStencilAttachment.layout)
            {
                VkImageLayout depthStencilLayout = optimalDepthStencilLayout(format);
                depthStencilAttachment = {attachmentIndex, depthStencilLayout};
                hasDepthStencilAttachment = true;
            }
        }
        else
        {
            if ((attachment.samples > 1) || (resolveAttachmentCount < 1))
                colorAttachments[colorIndex++] = {attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
            else
                resolveAttachments[resolveIndex++] = {attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
        }
        ++attachmentIndex;
    }
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
    for (auto const& attachment: attachments)
        hash = core::hashCombine(hash, attachment.hash());
    hash = core::hashCombine(hash, subpassDescription.getHash());
    core::zeroMemory(subpassDescription); // Aware destructor
    for (auto const& dependency: dependencies)
        hash = core::hashCombine(hash, dependency.hash());
}

RenderPass::RenderPass(std::shared_ptr<Device> device, const std::vector<AttachmentDescription>& attachments,
    const std::vector<SubpassDescription>& subpasses, const std::vector<SubpassDependency>& dependencies,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkRenderPassCreateFlags flags /* 0 */,
    const StructureChain& extendedInfo /* default */):
    RenderPass(std::move(device), std::move(allocator), attachments, flags)
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
    for (auto const& attachment : attachments)
        hash = core::hashCombine(hash, attachment.hash());
    for (auto const& subpass : subpasses)
        hash = core::hashCombine(hash, subpass.getHash());
    for (auto const& dependency : dependencies)
        hash = core::hashCombine(hash, dependency.hash());
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool RenderPass::usesClear() const noexcept
{
    auto it = std::find_if(attachments.begin(), attachments.end(),
        [](auto const& attachment)
        {
            return (VK_ATTACHMENT_LOAD_OP_CLEAR == attachment.loadOp);
        });
    return it != attachments.end();
}

bool RenderPass::usesMultisampling() const noexcept
{
    auto it = std::find_if(attachments.begin(), attachments.end(),
        [](auto const& attachment)
        {
            return (attachment.samples > 1);
        });
    return it != attachments.end();
}

VkImageLayout RenderPass::optimalDepthStencilLayout(const Format& format) const
{
#ifdef VK_KHR_separate_depth_stencil_layouts
    if (device->getFeatures()->separateDepthStencilLayoutsEnabled())
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

void RenderPass::begin(const FramebufferAttachments& attachments) const noexcept
{
    core::forConstEach(attachments, this->attachments,
        [](auto& attachment, auto& attachmentDesc)
        {   // initialLayout is the layout the attachment image subresource
            // will be in when a render pass instance begins
            const std::shared_ptr<Image>& image = (*attachment)->getImage();
            image->setLayout(attachmentDesc->initialLayout);
        });
}

void RenderPass::end(const FramebufferAttachments& attachments) const noexcept
{
    core::forConstEach(attachments, this->attachments,
        [](auto& attachment, auto& attachmentDesc)
        {   // finalLayout is the layout the attachment image subresource
            // will be transitioned to when a render pass instance ends.
            const std::shared_ptr<Image>& image = (*attachment)->getImage();
            switch (attachmentDesc->finalLayout)
            {
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
                break;
        #ifdef VK_KHR_maintenance2
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
                MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        #endif // VK_KHR_maintenance2
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                MAGMA_ASSERT(image->getUsage() & (VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT));
                break;
            }
            image->setLayout(attachmentDesc->finalLayout);
        });
}

std::ostream& operator<<(std::ostream& out, const RenderPass& renderPass)
{
    out << "VkRenderPassCreateInfo [" << std::endl
        << "\tflags: " << helpers::strinfigyRenderPassFlags(renderPass.flags) << std::endl
        << "\tattachmentCount: " << renderPass.attachments.size() << std::endl
        << "\tpAttachments: " << std::endl;
    for (auto const& attachment: renderPass.attachments)
    {
        out << "\t[" << std::endl
            << "\t\tflags: " << helpers::strinfigyAttachmentDescriptionFlags(attachment.flags) << std::endl
            << "\t\tformat: " << attachment.format << std::endl
            << "\t\tsamples: " << attachment.samples << std::endl
            << "\t\tloadOp: " << attachment.loadOp << std::endl
            << "\t\tstoreOp: " << attachment.storeOp << std::endl
            << "\t\tstencilLoadOp: " << attachment.stencilLoadOp << std::endl
            << "\t\tstencilStoreOp: " << attachment.stencilStoreOp << std::endl
            << "\t\tinitialLayout: " << attachment.initialLayout << std::endl
            << "\t\tfinalLayout: " << attachment.finalLayout << std::endl
            << "\t]" << std::endl;
    }
    // subpassCount
    // pSubpasses
    // dependencyCount
    // pDependencies
    out << "]";
    return out;
}
} // namespace magma
