/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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

namespace magma
{
    /* A render pass represents a collection of attachments, subpasses,
       and dependencies between the subpasses, and describes how the attachments
       are used over the course of the subpasses. The use of a render pass
       in a command buffer is a render pass instance. */

    class RenderPass : public NonDispatchable<VkRenderPass>
    {
    public:
        explicit RenderPass(std::shared_ptr<Device> device,
            const AttachmentDescription& attachment,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::initializer_list<AttachmentDescription>& attachments,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit RenderPass(std::shared_ptr<Device> device,
            const std::initializer_list<AttachmentDescription>& attachments,
            const std::initializer_list<Subpass>& subpasses,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~RenderPass();
        const std::vector<AttachmentDescription>& getAttachments() const noexcept { return attachments; }
        std::size_t getHash() const noexcept { return hash; }

    private:
        std::vector<AttachmentDescription> attachments;
        std::size_t hash = 0;
    };
} // namespace magma
