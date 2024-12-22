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
#include "buffer.h"

namespace magma
{
    /* Base class of uniform buffer. */

    class BaseUniformBuffer : public Buffer
    {
    public:
        ~BaseUniformBuffer();
        std::size_t getTypeSize() const noexcept { return typeSize; }
        uint32_t getArraySize() const noexcept { return arraySize; }
        VkDeviceSize getMapOffset() const noexcept
            { return persistent ? mapOffset : memory->getMapOffset(); }
        VkDeviceSize getNonCoherentAtomSize() const noexcept { return nonCoherentAtomSize; }
        virtual VkDeviceSize getAlignment() const noexcept = 0;
        virtual bool dynamic() const noexcept = 0;
        bool mappedPersistently() const noexcept { return persistent; }
        void *map() noexcept;
        void *mapRange(uint32_t firstIndex = 0,
            uint32_t arraySize = std::numeric_limits<uint32_t>::max()) noexcept;
        void *mapZeroMemory() noexcept;
        void unmap() noexcept;

    protected:
        BaseUniformBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::size_t typeSize,
            uint32_t arraySize,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            const Initializer& optional,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator,
            bool mappedPersistently);

    private:
        const std::size_t typeSize;
        const uint32_t arraySize;
        const bool persistent;
        VkDeviceSize mapOffset;
        VkDeviceSize nonCoherentAtomSize;
    };
} // namespace magma
