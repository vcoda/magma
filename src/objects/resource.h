/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "iresource.h"
#include "nondispatchable.h"
#include "../misc/sharing.h"
#include "../allocator/allocator.h"

namespace magma
{
    /* Non-dispatchable resource object (buffer, image,
       acceleration structure etc.) that has template declaration
       to handle different resource types. Buffers and images are
       created with a sharing mode controlling how they can be
       accessed from queues. */

    template<class Self, class Type>
    class Resource : public IResource,
        public NonDispatchable<Type>,
        public std::enable_shared_from_this<Self>
    {
    public:
        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        const Sharing& getSharing() const noexcept { return sharing; }
        const std::unique_ptr<IDeviceMemory>& getMemory() const noexcept override { return memory; }

    protected:
        Resource(VkObjectType objectType,
            std::shared_ptr<Device> device,
            VkDeviceSize size,
            const Sharing& sharing,
            std::shared_ptr<Allocator> allocator) noexcept:
            NonDispatchable<Type>(objectType, std::move(device), MAGMA_HOST_ALLOCATOR(allocator)),
            sharing(sharing),
            size(size),
            offset(0ull)
        {}

        const Sharing sharing;
        VkDeviceSize size;
        VkDeviceSize offset;
        std::unique_ptr<IDeviceMemory> memory;
    };
} // namespace magma
