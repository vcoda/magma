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
    /* Enables attaching arbitrary payloads to Vulkan objects.
       It introduces the idea of private data slots as a means
       of storing a 64-bit unsigned integer of application
       defined data. Private data slots can be created or
       destroyed any time an associated device is available.
       Private data slots can be reserved at device creation time,
       and limiting use to the amount reserved will allow the
       extension to exhibit better performance characteristics. */

#ifdef VK_EXT_private_data
    class PrivateDataSlot : public NonDispatchable<VkPrivateDataSlotEXT>
    {
    public:
        explicit PrivateDataSlot(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkPrivateDataSlotCreateFlagsEXT flags = 0,
            const StructureChain& extendedInfo = StructureChain());
        ~PrivateDataSlot();
    };
#endif // VK_EXT_private_data
} // namespace magma
