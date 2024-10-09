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
#include "../objects/image.h"
#include "../objects/commandBuffer.h"
#include "../misc/finish.h"

namespace magma::helpers
{
void layoutTransition(std::shared_ptr<Image> image, VkImageLayout newLayout,
    std::shared_ptr<CommandBuffer> cmdBuffer)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        image->layoutTransition(newLayout, cmdBuffer);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(std::move(cmdBuffer));
}

void batchLayoutTransition(const std::unordered_map<std::shared_ptr<Image>, VkImageLayout>& imageLayouts,
    std::shared_ptr<CommandBuffer> cmdBuffer)
{
    MAGMA_ASSERT(cmdBuffer->allowsReset());
    MAGMA_ASSERT(cmdBuffer->getState() != CommandBuffer::State::Recording);
    cmdBuffer->reset();
    cmdBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        for (auto& [image, layout]: imageLayouts)
            image->layoutTransition(layout, cmdBuffer);
    }
    cmdBuffer->end();
    // Block until execution is complete
    finish(std::move(cmdBuffer));
}
} // namespace magma::helpers
