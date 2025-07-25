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
#include "../objects/surface.h"

namespace magma
{
    /* Provides a mechanism to create a surface object
       that refers to a Wayland wl_surface. */

#ifdef VK_KHR_wayland_surface
    class WaylandSurface : public Surface
    {
    public:
        explicit WaylandSurface(Instance *instance,
            wl_display *display,
            wl_surface *surface,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkWaylandSurfaceCreateFlagsKHR flags = 0);
        const wl_display *getDisplay() const noexcept { return display; }
        const wl_surface *getSurface() const noexcept { return surface; }

    private:
        const wl_display *const display;
        const wl_surface *const surface;
    };
#endif // VK_KHR_wayland_surface
} // namespace magma
