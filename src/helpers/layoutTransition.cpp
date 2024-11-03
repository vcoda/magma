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
void layoutTransition(std::shared_ptr<Image> image, VkImageLayout newLayout,
    const std::unique_ptr<CommandBuffer>& cmdBuffer,
    VkDependencyFlags dependencyFlags /* 0 */)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        image->layoutTransition(newLayout, cmdBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, dependencyFlags);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(cmdBuffer);
}

void batchLayoutTransition(const std::vector<ImageLayoutTransition>& imageLayouts,
    const std::unique_ptr<CommandBuffer>& cmdBuffer,
    VkDependencyFlags dependencyFlags /* 0 */)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    std::vector<ImageMemoryBarrier> imageMemoryBarriers;
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        for (const ImageLayoutTransition& it: imageLayouts)
        {
            VkImageSubresourceRange subresourceRange = it.image->getSubresourceRange(it.baseMipLevel, it.baseArrayLayer);
            imageMemoryBarriers.emplace_back(it.image.get(), it.newLayout, subresourceRange);
        }
        // Use single barrier command for all images
        cmdBuffer->pipelineBarrier(
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            imageMemoryBarriers, dependencyFlags);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(cmdBuffer);
}
} // namespace magma::helpers
