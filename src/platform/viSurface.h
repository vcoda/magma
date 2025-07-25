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
       associated with an nn::vi::Layer. */

#ifdef VK_NN_vi_surface
    class ViSurface : public Surface
    {
    public:
        explicit ViSurface(Instance * instance,
            nn::vi::NativeWindowHandle window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkViSurfaceCreateFlagsNN flags = 0);
        nn::vi::NativeWindowHandle getWindow() const noexcept { return window; }

    private:
        const nn::vi::NativeWindowHandle window;
    };
#endif // VK_NN_vi_surface
} // namespace magma
