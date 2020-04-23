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
#pragma once

namespace magma
{
    /* A subpass description describes the subset of attachments that is involved
       in the execution of a subpass. Each subpass can read from some attachments
       as input attachments, write to some as color attachments or depth/stencil
       attachments, and perform multisample resolve operations to resolve attachments.
       A subpass description can also include a set of preserve attachments, which are
       attachments that are not read or written by the subpass but whose contents
       must be preserved throughout the subpass. */

    class Subpass : public VkSubpassDescription
    {
    protected:
        Subpass(VkSubpassDescriptionFlags flags,
            VkPipelineBindPoint pipelineBindPoint) noexcept;

    public:
        Subpass(const Subpass&) noexcept;
        Subpass& operator=(const Subpass&) noexcept;
        ~Subpass();
        std::size_t hash() const noexcept;
    };

    class GraphicsSubpass : public Subpass
    {
    public:
        GraphicsSubpass(VkImageLayout colorLayout);
        GraphicsSubpass(VkImageLayout colorLayout,
            VkImageLayout depthStencilLayout);
        GraphicsSubpass(const std::vector<VkImageLayout>& colorLayouts);
        GraphicsSubpass(const std::vector<VkImageLayout>& colorLayouts,
            const VkImageLayout& depthStencilLayout);
    };

    class ComputeSubpass : public Subpass
    {
    public:
        ComputeSubpass() noexcept;
    };
} // namespace magma

namespace magma
{
    namespace subpasses
    {
        extern const GraphicsSubpass colorAttachment;
        extern const GraphicsSubpass colorDepthStencilAttachment;
        extern const GraphicsSubpass colorAttachmentDepthStencilReadOnly;
        extern const GraphicsSubpass depthStencilAttachment;
        extern const GraphicsSubpass depthStencilReadOnly;

#ifdef VK_KHR_maintenance2
        extern const GraphicsSubpass colorStencilAttachmentDepthReadOnly;
        extern const GraphicsSubpass colorDepthAttachmentStencilReadOnly;
        extern const GraphicsSubpass depthReadOnlyStencilAttachment;
        extern const GraphicsSubpass depthAttachmentStencilReadOnly;
#endif // VK_KHR_maintenance2

#ifdef VK_KHR_separate_depth_stencil_layouts
        extern const GraphicsSubpass depthAttachment;
        extern const GraphicsSubpass depthReadOnly;
        extern const GraphicsSubpass stencilAttachment;
        extern const GraphicsSubpass stencilReadOnly;
#endif // VK_KHR_separate_depth_stencil_layouts
    } // namespace subpasses
} // namespace magma
