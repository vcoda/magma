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

#ifdef VK_KHR_create_renderpass2
namespace magma
{
    /* Structure specifying a subpass dependency. */

    struct SubpassDependency2 : VkSubpassDependency2KHR
    {
        constexpr SubpassDependency2() noexcept;
        constexpr SubpassDependency2(uint32_t srcSubpass,
            uint32_t dstSubpass,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask,
            VkDependencyFlags dependencyFlags,
            int32_t viewOffset) noexcept;
        constexpr hash_t hash() const noexcept;
        constexpr SubpassDependency2 operator~() const noexcept; // Swaps source and destination stages
    };

    /* The first synchronization scope includes commands that occur earlier in submission order
       than the vkCmdBeginRenderPass used to begin the render pass instance.
       The second synchronization scope includes commands that occur later in submission order
       than the vkCmdEndRenderPass used to end the render pass instance. */

    struct SubpassExternalDependency2 : SubpassDependency2
    {
        constexpr SubpassExternalDependency2(VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask,
            VkDependencyFlags dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
            int32_t viewOffset = 0) noexcept;
    };

    /* For a given subpass to allow a pipeline barrier, the render pass must declare
       a self-dependency from that subpass to itself. That is, there must exist
       a subpass dependency with srcSubpass and dstSubpass both equal to that subpass index.
       More than one self-dependency can be declared for each subpass. */

    struct SubpassSelfDependency2 : SubpassDependency2
    {
        constexpr SubpassSelfDependency2(uint32_t subpassIndex,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkAccessFlags srcAccessMask,
            VkAccessFlags dstAccessMask,
            VkDependencyFlags dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
            int32_t viewOffset = 0) noexcept;
    };
} // namespace magma

#include "subpassDependency2.inl"
#endif // VK_KHR_create_renderpass2
