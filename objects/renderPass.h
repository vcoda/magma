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
#pragma once
#include "nondispatchable.h"
#include "../descriptors/attachment.h"
#include "../descriptors/subpass.h"
#include "../misc/createInfo.h"

namespace magma
{
    class Format;

    /* A render pass represents a collection of attachments, subpasses,
       and dependencies between the subpasses, and describes how the attachments
       are used over the course of the subpasses. The use of a render pass
       in a command buffer is a render pass instance. */

    class RenderPass : public NonDispatchable<VkRenderPass>
    {
    public:
        explicit RenderPass(std::shared_ptr<Device> device,
            const AttachmentDescription& attachment,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const CreateInfo& chainedInfo = CreateInfo());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const CreateInfo& chainedInfo = CreateInfo());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            const std::vector<SubpassDescription>& subpasses,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const CreateInfo& chainedInfo = CreateInfo());
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::vector<AttachmentDescription>& attachments,
            const std::vector<SubpassDescription>& subpasses,
            const std::vector<SubpassDependency>& dependencies,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const CreateInfo& chainedInfo = CreateInfo());
        ~RenderPass();
        const std::vector<AttachmentDescription>& getAttachments() const noexcept { return attachments; }
        bool hasClearOp() const noexcept;
        std::size_t getHash() const noexcept { return hash; }

    private:
        VkImageLayout optimalDepthStencilLayout(const Format& format) const;
        SubpassDependency subpassStartDependency(bool colorAttachment, bool depthStencilAttachment) const noexcept;
        SubpassDependency subpassEndDependency(bool colorAttachment, bool depthStencilAttachment) const noexcept;

        std::vector<AttachmentDescription> attachments;
        std::size_t hash;
    };
} // namespace magma
