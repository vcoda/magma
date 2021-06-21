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
    /* Indirect buffer allows to source the draw command's parameters indirectly from memory. */

    class IndirectBuffer : public Buffer
    {
    public:
        ~IndirectBuffer();
        uint32_t getMaxDrawCommands() const noexcept { return maxDrawCommands; }
        uint32_t getDrawCommandCount() const noexcept { return cmdCount; }
        uint32_t getStride() const noexcept { return stride; }
        bool persistentlyMapped() const noexcept { return persistent; }
        void reset() noexcept { cmdCount = 0; }

    protected:
        explicit IndirectBuffer(std::shared_ptr<Device> device,
            uint32_t maxDrawCommands,
            std::size_t stride,
            bool persistentlyMapped,
            VkBufferCreateFlags flags,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator);

        const uint32_t maxDrawCommands;
        const uint32_t stride;
        const bool persistent;
        uint32_t cmdCount;
    };

    /* An array of VkDrawIndirectCommand structures. */

    class DrawIndirectBuffer : public IndirectBuffer
    {
    public:
        explicit DrawIndirectBuffer(std::shared_ptr<Device> device,
            uint32_t maxDrawIndexedCommands,
            std::shared_ptr<Allocator> allocator = nullptr,
            bool persistentlyMapped = false,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing());
        uint32_t writeDrawCommand(uint32_t vertexCount,
            uint32_t firstVertex = 0) noexcept;
        uint32_t writeDrawInstancedCommand(uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex = 0,
            uint32_t firstInstance = 0) noexcept;
        uint32_t writeDrawCommand(const VkDrawIndirectCommand& drawCmd) noexcept;

    private:
        VkDrawIndirectCommand *const mappedData;
    };

    /* An array of VkDrawIndexedIndirectCommand structures. */

    class DrawIndexedIndirectBuffer : public IndirectBuffer
    {
    public:
        explicit DrawIndexedIndirectBuffer(std::shared_ptr<Device> device,
            uint32_t maxDrawIndexedCommands,
            std::shared_ptr<Allocator> allocator = nullptr,
            bool persistentlyMapped = false,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing());
        uint32_t writeDrawIndexedCommand(uint32_t indexCount,
            uint32_t firstIndex = 0,
            uint32_t vertexOffset = 0) noexcept;
        uint32_t writeDrawIndexedInstancedCommand(uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t firstIndex = 0,
            uint32_t vertexOffset = 0,
            uint32_t firstInstance = 0) noexcept;
        uint32_t writeDrawIndexedCommand(const VkDrawIndexedIndirectCommand& drawIndexedCmd) noexcept;

    private:
        VkDrawIndexedIndirectCommand *const mappedData;
    };
} // namespace magma
