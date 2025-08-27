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
#include "model/iresource.h"
#include "ideviceMemory.h"
#include "../misc/sharing.h"

namespace magma
{
    /* Non-dispatchable resource object (buffer, image,
       acceleration structure). Buffers and images are
       created with a sharing mode controlling how they
       can be accessed from queues. */

    class Resource : public IResource
    {
    public:
        VkDeviceSize getSize() const noexcept { return size; }
        VkDeviceSize getOffset() const noexcept { return offset; }
        const Sharing& getSharing() const noexcept { return sharing; }
        const std::unique_ptr<IDeviceMemory>& getMemory() const noexcept override { return memory; }

    protected:
        Resource(VkDeviceSize size, const Sharing& sharing) noexcept:
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
