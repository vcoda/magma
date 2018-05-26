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
#include "buffer.h"

namespace magma
{
    class Device;
    class IAllocator;

    class IndirectBuffer : public Buffer
    {
    public:
        IndirectBuffer(std::shared_ptr<const Device> device,
            uint32_t drawCount = 1,
            VkBufferCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
        void writeDrawCommand(uint32_t vertexCount, 
            uint32_t firstVertex = 0) noexcept;
        void writeDrawCommand(uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex,
            uint32_t firstInstance) noexcept;
        void writeDrawCommand(const VkDrawIndirectCommand& drawCmd) noexcept;
        void writeDrawCommands(const std::vector<VkDrawIndirectCommand>& drawCmdList) noexcept;
    };
} // namespace magma
