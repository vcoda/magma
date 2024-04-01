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
       that refers to an X11 Window, using the Xlib
       client-side library. */

#ifdef VK_KHR_xlib_surface
    class XlibSurface : public Surface
    {
    public:
        explicit XlibSurface(std::shared_ptr<const Instance> instance,
            ::Display *dpy,
            Window window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkXlibSurfaceCreateFlagsKHR flags = 0);
        const ::Display *getDisplay() const noexcept { return dpy; }
        Window getWindow() const noexcept { return window; }

    private:
        const ::Display *const dpy;
        const Window window;
    };
#endif // VK_KHR_xlib_surface
} // namespace magma
