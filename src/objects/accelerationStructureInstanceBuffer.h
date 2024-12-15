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
#ifdef VK_KHR_acceleration_structure
#include "accelerationStructureInputBuffer.h"
#include "srcTransferBuffer.h"

namespace magma
{
    /* An array of acceleration structure instances, e.g.
       VkAccelerationStructureInstanceKHR or
       VkAccelerationStructureMotionInstanceNV. */

    template<class Instance>
    class AccelerationStructureInstanceBuffer : public AccelerationStructureInputBuffer
    {
    public:
        explicit AccelerationStructureInstanceBuffer(std::shared_ptr<Device> device,
            uint32_t instanceCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        uint32_t getInstanceCount() const noexcept { return instanceCount; }
        Instance& getInstance(uint32_t index) noexcept;
        const Instance& getInstance(uint32_t index) const noexcept { return instances[index]; }
        void updateModified(lent_ptr<CommandBuffer> cmdBuffer);
        void updateWhole(lent_ptr<CommandBuffer> cmdBuffer);

    private:
        const uint32_t instanceCount;
        uint32_t first;
        uint32_t last;
        Instance *instances;
        std::unique_ptr<SrcTransferBuffer> srcBuffer;
    };
} // namespace magma

#include "accelerationStructureInstanceBuffer.inl"
#endif // VK_KHR_acceleration_structure
