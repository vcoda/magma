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
#include "../shared.h"

namespace magma
{
    class Subpass 
    {
    public:
        Subpass(VkSubpassDescriptionFlags flags, VkPipelineBindPoint pipelineBindPoint);
        ~Subpass();
        MAGMA_TYPE_CAST(VkSubpassDescription, desc)

    protected:
        VkSubpassDescription desc;
    };

    class GraphicsSubpass : public Subpass
    {
    public:
        GraphicsSubpass(const VkImageLayout& colorLayout);
        GraphicsSubpass(const VkImageLayout& colorLayout, const VkImageLayout& depthStencilLayout);
        GraphicsSubpass(const std::vector<VkImageLayout>& colorLayouts);
        GraphicsSubpass(const std::vector<VkImageLayout>& colorLayouts, const VkImageLayout& depthStencilLayout);
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
        extern const GraphicsSubpass colorOptimalDepthStencilReadOnlyOptimal;
    } // namespace subpasses
} // namespace magma
