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
       that refers to a QNX Screen window. */

#ifdef VK_QNX_screen_surface
    class QnxSurface : public Surface
    {
    public:
        explicit QnxSurface(Instance * instance,
            struct _screen_context *context,
            struct _screen_window *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkScreenSurfaceCreateFlagsQNX flags = 0);
        struct _screen_context const *getContext() const noexcept { return context; }
        struct _screen_window const *getWindow() const noexcept { return window; }

    private:
        struct _screen_context const *const context;
        struct _screen_window const *const window;
    };
#endif // VK_QNX_screen_surface
} // namespace magma
