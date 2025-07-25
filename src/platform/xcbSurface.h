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
       that refers to an X11 Window, using the XCB
       client-side library. */

#ifdef VK_KHR_xcb_surface
    class XcbSurface : public Surface
    {
    public:
        explicit XcbSurface(Instance * instance,
            xcb_connection_t *connection,
            xcb_window_t window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkXcbSurfaceCreateFlagsKHR flags = 0);
        const xcb_connection_t *getConnection() const noexcept { return connection; }
        xcb_window_t getWindow() const noexcept { return window; }

    private:
        const xcb_connection_t *const connection;
        const xcb_window_t window;
    };
#endif // VK_KHR_xcb_surface
} // namespace magma
