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
#include <functional>
#include "buffer.h"

namespace magma
{
    template <typename Block>
    class UniformBuffer : public Buffer
    {
    public:
        UniformBuffer(std::shared_ptr<const Device> device,
            uint32_t arraySize = 1,
            VkBufferCreateFlags flags = 0):
            Buffer(device, sizeof(Block) * arraySize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, flags,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        {
            static_assert(sizeof(Block)%16 == 0, "uniform block should have 16-byte alignment");
        }

        Block *map(bool clearMemory = false) noexcept
        {
            if (void *block = memory->map(0, size))
            {
                if (clearMemory)
                    memset(block, 0, static_cast<size_t>(size));
                return static_cast<Block *>(block);
            }
            return nullptr;
        }

        void unmap() noexcept
        { 
            memory->unmap(); 
        }
    };
} // namespace magma
