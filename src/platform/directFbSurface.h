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
       that refers to a DirectFB IDirectFBSurface. */

#ifdef VK_EXT_directfb_surface
    class DirectFBSurface : public Surface
    {
    public:
        explicit DirectFBSurface(Instance * instance,
            IDirectFB *dfb,
            IDirectFBSurface *surface,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkDirectFBSurfaceCreateFlagsEXT flags = 0);
        IDirectFB *getDirectFB() const noexcept { return dfb; }
        IDirectFBSurface *getSurface() const noexcept { return surface; }

    private:
        IDirectFB *dfb;
        IDirectFBSurface *surface;
    };
#endif // VK_EXT_directfb_surface
} // namespace magma
