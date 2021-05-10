/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "buffer.h"

namespace magma
{
    /* A buffer that is suitable for passing to indirect draw commands. */

    class IndirectBuffer : public Buffer
    {
    public:
        explicit IndirectBuffer(std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator = nullptr,
            uint32_t drawCommandCount = 1,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing());
        void writeDrawCommand(uint32_t vertexCount,
            uint32_t firstVertex = 0,
            uint32_t cmdIndex = 0) noexcept;
        void writeDrawCommand(uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex,
            uint32_t firstInstance,
            uint32_t cmdIndex = 0) noexcept;
        void writeDrawCommand(const VkDrawIndirectCommand& drawCmd,
            uint32_t cmdIndex = 0) noexcept;
        void writeDrawCommands(const std::vector<VkDrawIndirectCommand>& drawCmdList) noexcept;
        void writeDrawCommands(const std::list<VkDrawIndirectCommand>& drawCmdList) noexcept;
    };
} // namespace magma
