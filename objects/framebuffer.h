/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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

namespace magma
{
    class RenderPass;
    class ImageView;

    /* Framebuffers represent a collection of specific memory attachments
       that a render pass instance uses. Render pass and a framebuffer define
       the complete render target state for one or more subpasses as well as
       the algorithmic dependencies between the subpasses.*/

    class Framebuffer : public NonDispatchable<VkFramebuffer>
    {
    public:
        explicit Framebuffer(std::shared_ptr<const RenderPass> renderPass,
            std::shared_ptr<ImageView> attachment,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkFramebufferCreateFlags = 0);
        explicit Framebuffer(std::shared_ptr<const RenderPass> renderPass,
            const std::vector<std::shared_ptr<ImageView>>& attachments,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkFramebufferCreateFlags = 0);
        ~Framebuffer();
        std::shared_ptr<const RenderPass> getRenderPass() const noexcept { return renderPass; }
        const std::vector<std::shared_ptr<ImageView>>& getAttachments() const noexcept { return attachments; }
        const VkExtent2D& getExtent() const noexcept { return extent; }
        uint32_t getLayers() const noexcept;
        bool layered() const noexcept { return getLayers() > 1; }

    private:
        std::shared_ptr<const RenderPass> renderPass;
        const std::vector<std::shared_ptr<ImageView>> attachments;
        const VkExtent2D extent;
    };
} // namespace magma
