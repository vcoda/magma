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
#pragma once
#include "nondispatchable.h"
#include "../renderpass/attachment.h"
#include "../renderpass/subpass.h"
#include "../barriers/subpassDependency.h"

namespace magma
{
    class Format;
    class Framebuffer;

    /* A render pass represents a collection of attachments,
       subpasses, and dependencies between the subpasses,
       and describes how the attachments are used over the
       course of the subpasses. The use of a render pass
       in a command buffer is a render pass instance. */

    class RenderPass : public NonDispatchable<VkRenderPass>
    {
    public:
        explicit RenderPass(std::shared_ptr<Device> device,
            const AttachmentDescription& attachment,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            const std::vector<SubpassDescription>& subpasses,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            const std::vector<SubpassDescription>& subpasses,
            const std::vector<SubpassDependency>& dependencies,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        ~RenderPass();
        const std::vector<AttachmentDescription>& getAttachments() const noexcept { return attachments; }
        bool hasClearOp() const noexcept;
        hash_t getHash() const noexcept { return hash; }

    protected:
        RenderPass(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator,
            const std::vector<AttachmentDescription>& attachments);

    protected:
        VkImageLayout optimalDepthStencilLayout(const Format& format) const;
        SubpassDependency subpassBeginDependency(bool colorAttachment, bool depthStencilAttachment) const noexcept;
        SubpassDependency subpassEndDependency(bool colorAttachment, bool depthStencilAttachment) const noexcept;

    private:
        void begin(std::shared_ptr<Framebuffer> framebuffer) const noexcept;
        void end(std::shared_ptr<Framebuffer> framebuffer) const noexcept;

    protected:
        std::vector<AttachmentDescription> attachments;
        hash_t hash;
        friend class CommandBuffer;
    };
} // namespace magma
