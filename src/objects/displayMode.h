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
#include "model/nondispatchable.h"

namespace magma
{
    class Display;

    /* Each display has one or more supported modes associated
       with it by default. */

#ifdef VK_KHR_display
    class DisplayMode : public NonDispatchable<VkDisplayModeKHR>
    {
    public:
        explicit DisplayMode(std::shared_ptr<const Display> display,
            const VkExtent2D& visibleRegion,
            uint32_t refreshRate,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const StructureChain& extendedInfo = StructureChain());
        VkDisplayPlaneCapabilitiesKHR getDisplayPlaneCapabilities(uint32_t planeIndex) const;
        const VkExtent2D& getVisibleRegion() const noexcept { return visibleRegion; }
        uint32_t getRefreshRate() const noexcept { return refreshRate; }

    private:
        const VkExtent2D visibleRegion;
        const uint32_t refreshRate;
    };
#endif // VK_KHR_display
} // namespace magma
