/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    class CommandPool;
    class CommandBuffer;

    namespace helpers
    {
        /* Helper function to execute a number of calls to command buffer.
           It allocates command buffer, calls begin/end() and submits it
           to the graphics queue. */

        void executeCommandBuffer(std::shared_ptr<CommandPool> cmdPool,
            std::function<void(std::shared_ptr<CommandBuffer>)> drawFn,
            VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT,
            const char *blockName = "magma::helpers::executeCommandBuffer",
            uint32_t blockColor = 0x0);

        /* Helper function to read pipelined writes of marker values into
           a buffer object. The primary purpose of these markers is to
           facilitate the development of debugging tools for tracking which
           pipelined command contributed to device loss. */

    #ifdef VK_AMD_buffer_marker
        std::vector<uint32_t> readBufferMarkers(std::shared_ptr<const CommandBuffer> cmdBuffer,
            VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT);
    #endif // VK_AMD_buffer_marker
    } // namespace helpers
} // namespace magma
