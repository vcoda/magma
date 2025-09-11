/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "model/iresource.h"
#include "../misc/sharing.h"

namespace magma
{
    class Allocator;
    class Device;
    class StructureChain;

    /* Non-dispatchable resource object (buffer, image,
       acceleration structure). Buffers and images are
       created with a sharing mode controlling how they
       can be accessed from queues. */

    class Resource : public IResource
    {
    public:
        bool unbound() const noexcept { return !memory; }
        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        const Sharing& getSharing() const noexcept { return sharing; }
        const std::unique_ptr<IDeviceMemory>& getMemory() const noexcept override { return memory; }
        static Resource *get(NonDispatchableHandle handle) noexcept;

    protected:
        Resource(VkDeviceSize size,
            const Sharing& sharing) noexcept;
        void postCreate(NonDispatchableHandle handle);
        void preDestroy(NonDispatchableHandle handle);
        std::unique_ptr<IDeviceMemory> allocateMemory(NonDispatchableHandle handle,
            const VkMemoryRequirements& memoryRequirements,
            VkMemoryPropertyFlags flags,
            const StructureChain& extendedMemoryInfo,
            std::shared_ptr<Device> device,
            std::shared_ptr<Allocator> allocator);

        const Sharing sharing;
        VkDeviceSize size;
        VkDeviceSize offset;
        std::unique_ptr<IDeviceMemory> memory;

    private:
        static std::unordered_map<NonDispatchableHandle, Resource*> resourceMap;
        static core::Spinlock mtx;
    };
} // namespace magma
