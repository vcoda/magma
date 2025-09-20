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
    /* A storage buffer described in a shader as a structure with
       various members that load, store, and atomic operations can
       be performed on. */

    class StorageBuffer : public Buffer
    {
    public:
        explicit StorageBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit StorageBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            VkDeviceSize size,
            const void *data,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
        explicit StorageBuffer(lent_ptr<CommandBuffer> cmdBuffer,
            lent_ptr<const SrcTransferBuffer> srcBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkDeviceSize size = 0,
            VkDeviceSize srcOffset = 0,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };

    /* A storage buffer with HOST_VISIBLE_BIT | HOST_COHERENT_BIT flags.
       This buffer may be written by device and read by host, but user
       has no intention to update it each frame. */

    class HostStorageBuffer : public Buffer
    {
    public:
        explicit HostStorageBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };

    /* Major GPU vendors expose a 256MiB-ish staging buffer
       with the DEVICE_LOCAL | HOST_VISIBLE | HOST_COHERENT
       flags where the GPU and CPU can both write into shared
       memory visible to each other. This limit correlates with
       the 256MiB PCIE-specified BAR limit that defines the size
       of the VRAM window that the host can access. */

    class DynamicStorageBuffer : public Buffer
    {
    public:
        explicit DynamicStorageBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            bool stagedPool,
            std::shared_ptr<Allocator> allocator = nullptr,
            const void *initialData = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing(),
            CopyMemoryFn copyMem = nullptr);
    };

    /* Storage buffer for acceleration structure placement. */

#ifdef VK_KHR_acceleration_structure
    class AccelerationStructureStorageBuffer : public Buffer
    {
    public:
        explicit AccelerationStructureStorageBuffer(std::shared_ptr<Device> device,
            VkDeviceSize size,
            VkAccelerationStructureBuildTypeKHR buildType,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
