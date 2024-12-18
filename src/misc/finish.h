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
    class CommandBuffer;

    /* Submits command buffer to a queue and blocks until
       submitted command buffer have completed execution.
       If <waitIdle> is true, it blocks until all queue
       operations are finished. This is an analogue of
       glFinish() function in OpenGL. */

    void finish(lent_ptr<CommandBuffer> cmdBuffer,
        lent_ptr<Queue> queue,
        bool waitIdle = false);
    void finish(lent_ptr<CommandBuffer> cmdBuffer,
        bool waitIdle = false);
} // namespace magma
