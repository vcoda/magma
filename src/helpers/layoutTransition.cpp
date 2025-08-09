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
#include "pch.h"
#pragma hdrstop
#include "layoutTransition.h"
#include "../objects/image.h"
#include "../objects/commandBuffer.h"
#include "../misc/finish.h"

namespace magma::helpers
{
void layoutTransition(lent_ptr<Image> image, VkImageLayout newLayout,
    lent_ptr<CommandBuffer> cmdBuffer,
    VkDependencyFlags dependencyFlags /* 0 */)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        image->layoutTransition(newLayout, cmdBuffer.get(),
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, dependencyFlags);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(std::move(cmdBuffer));
}

void batchLayoutTransition(const std::unordered_map<lent_ptr<Image>, LayoutTransition>& imageLayouts,
    lent_ptr<CommandBuffer> cmdBuffer,
    VkDependencyFlags dependencyFlags /* 0 */)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        std::vector<ImageMemoryBarrier> imageMemoryBarriers;
        imageMemoryBarriers.reserve(imageLayouts.size());
        for (auto const& [image, transition]: imageLayouts)
        {
            const VkImageSubresourceRange subresourceRange = image->getSubresourceRange(
                transition.baseMipLevel, transition.baseArrayLayer);
            imageMemoryBarriers.emplace_back(image.get(), transition.newLayout, subresourceRange);
        }
        // Use single barrier command for all images
        cmdBuffer->pipelineBarrier(
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            imageMemoryBarriers, dependencyFlags);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(std::move(cmdBuffer));
}
} // namespace magma::helpers
