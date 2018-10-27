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
#include <vector>
#include "../vulkan.h"

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
        Subpass(const Subpass&);
        Subpass& operator=(const Subpass&);
        ~Subpass();
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
        ComputeSubpass();
    };

    namespace subpasses
    {
        extern const GraphicsSubpass colorOptimal;
        extern const GraphicsSubpass colorOptimalDepthStencilOptimal;
        extern const GraphicsSubpass colorOptimalDepthReadOnlyStencilOptimal;
        extern const GraphicsSubpass colorOptimalDepthOptimalStencilReadOnlyOptimal;

        extern const GraphicsSubpass depthStencilOptimal;
        extern const GraphicsSubpass depthReadOnlyStencilOptimal;
        extern const GraphicsSubpass depthOptimalStencilReadOnlyOptimal;
    } // namespace subpasses
} // namespace magma
