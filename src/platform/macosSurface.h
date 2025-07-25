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
       based on an NSView, the native surface type of macOS,
       which is underpinned by a CAMetalLayer, to support
       rendering to the surface using Apple's Metal framework. */

#ifdef VK_MVK_macos_surface
    class MacOSSurface : public Surface
    {
    public:
        explicit MacOSSurface(Instance *instance,
            const void *view,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkMacOSSurfaceCreateFlagsMVK flags = 0);
        const void *getView() const noexcept { return view; }

    private:
        const void *const view;
    };
#endif // VK_MVK_macos_surface
} // namespace magma
