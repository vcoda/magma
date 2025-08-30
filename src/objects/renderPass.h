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
#pragma once
#include "model/nondispatchable.h"
#include "../renderpass/attachment.h"
#include "../renderpass/subpass.h"
#include "../barriers/subpassDependency.h"

namespace magma
{
    class Format;
    class ImageView;

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
            VkRenderPassCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkRenderPassCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            const std::vector<SubpassDescription>& subpasses,
            const std::vector<SubpassDependency>& dependencies,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkRenderPassCreateFlags flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~RenderPass();
        VkRenderPassCreateFlags getFlags() const noexcept { return flags; }
        const std::vector<AttachmentDescription>& getAttachments() const noexcept { return attachments; }
        hash_t getHash() const noexcept { return hash; }
        VkExtent2D getRenderAreaGranularity() const noexcept;
        bool usesClear() const noexcept;
        bool usesMultisampling() const noexcept;
        friend std::ostream& operator<<(std::ostream&, const RenderPass&);

    protected:
        RenderPass(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator,
            const std::vector<AttachmentDescription>& attachments,
            VkRenderPassCreateFlags flags);
        VkImageLayout optimalDepthStencilLayout(const Format& format) const;
        SubpassDependency beginDependency(bool colorAttachment,
            bool depthStencilAttachment) const noexcept;
        SubpassDependency endDependency(bool colorAttachment,
            bool depthStencilAttachment) const noexcept;

        const VkRenderPassCreateFlags flags;
        std::vector<AttachmentDescription> attachments;
        hash_t hash;

    private:
        void setInitialLayout(ImageView *imageView,
            const AttachmentDescription& attachment) const noexcept;
        void setFinalLayout(ImageView *imageView,
            const AttachmentDescription& attachment) const noexcept;
        void begin(const std::vector<ImageView *>& imageViews) const noexcept;
        void end(const std::vector<ImageView *>& imageViews) const noexcept;
        void begin(const std::vector<std::shared_ptr<ImageView>>& imageViews) const noexcept;
        void end(const std::vector<std::shared_ptr<ImageView>>& imageViews) const noexcept;
        friend class CommandBuffer;
    };
} // namespace magma

#include "renderPass.inl"
