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

    namespace helpers
    {
        /* Helper function to read pipelined writes of marker values into
           a buffer object. The primary purpose of these markers is to
           facilitate the development of debugging tools for tracking which
           pipelined command contributed to device loss. */

        std::vector<uint32_t> readbackBufferMarkers(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *blockName = "magma::helpers::readbackBufferMarkers",
            uint32_t blockColor = 0x0);
    } // namespace helpers
} // namespace magma
