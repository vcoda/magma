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
#include "nondispatchable.h"

namespace magma
{
    /* Opaque handle to a private data slot object. */

#ifdef VK_EXT_private_data
    class PrivateDataSlot : public NonDispatchable<VkPrivateDataSlotEXT>
    {
    public:
        explicit PrivateDataSlot(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPrivateDataSlotCreateFlags flags = 0);
        ~PrivateDataSlot();
    };
#endif // VK_EXT_private_data
} // namespace magma
