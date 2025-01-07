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
    class CommandPool;
    class CommandBuffer;

    namespace helpers
    {
        /* Helper function to execute a number of calls to command buffer.
           It allocates command buffer, calls begin/end() and submits it
           to the graphics queue. */

        void executeCommandBuffer(const std::unique_ptr<CommandPool>& cmdPool,
            std::function<void(const std::shared_ptr<CommandBuffer>&)> cmdFn,
            const char *blockName = "magma::helpers::executeCommandBuffer",
            uint32_t blockColor = 0x0);
    } // namespace helpers
} // namespace magma
