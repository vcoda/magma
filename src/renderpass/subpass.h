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

namespace magma
{
    /* A subpass description describes the subset of attachments
       that is involved in the execution of a subpass. Each subpass
       can read from some attachments as input attachments, write to
       some as color attachments or depth/stencil attachments, and
       perform multisample resolve operations to resolve attachments.
       A subpass description can also include a set of preserve
       attachments, which are attachments that are not read or
       written by the subpass but whose contents must be preserved
       throughout the subpass. */

    struct SubpassDescription final : VkSubpassDescription
    {
        constexpr SubpassDescription(VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) noexcept;
        explicit SubpassDescription(VkImageLayout colorLayout) noexcept;
        explicit SubpassDescription(VkImageLayout colorLayout,
            VkImageLayout depthStencilLayout) noexcept;
        explicit SubpassDescription(const std::vector<VkImageLayout>& colorLayouts) noexcept;
        explicit SubpassDescription(const std::vector<VkImageLayout>& colorLayouts,
            VkImageLayout depthStencilLayout) noexcept;
        SubpassDescription(const SubpassDescription&) noexcept;
        SubpassDescription(SubpassDescription&&) noexcept;
        SubpassDescription& operator=(const SubpassDescription&) noexcept;
        SubpassDescription& operator=(SubpassDescription&&) noexcept;
        ~SubpassDescription();
        hash_t hash() const noexcept;
    };

    namespace subpass
    {
        extern const SubpassDescription colorAttachment;
        extern const SubpassDescription colorDepthStencilAttachment;
        extern const SubpassDescription colorAttachmentDepthStencilReadOnly;
        extern const SubpassDescription depthStencilAttachment;
        extern const SubpassDescription depthStencilReadOnly;

    #ifdef VK_KHR_maintenance2
        extern const SubpassDescription colorStencilAttachmentDepthReadOnly;
        extern const SubpassDescription colorDepthAttachmentStencilReadOnly;
        extern const SubpassDescription depthReadOnlyStencilAttachment;
        extern const SubpassDescription depthAttachmentStencilReadOnly;
    #endif // VK_KHR_maintenance2

    #ifdef VK_KHR_separate_depth_stencil_layouts
        extern const SubpassDescription colorDepthAttachment;
        extern const SubpassDescription colorStencilAttachment;
        extern const SubpassDescription colorAttachmentDepthReadOnly;
        extern const SubpassDescription colorAttachmentStencilReadOnly;
        extern const SubpassDescription depthAttachment;
        extern const SubpassDescription stencilAttachment;
        extern const SubpassDescription depthReadOnly;
        extern const SubpassDescription stencilReadOnly;
    #endif // VK_KHR_separate_depth_stencil_layouts
    } // namespace subpass
} // namespace magma

#include "subpass.inl"
