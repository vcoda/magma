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
       that refers to an ANativeWindow, Android's
       native surface type. */

#ifdef VK_KHR_android_surface
    class AndroidSurface : public Surface
    {
    public:
        explicit AndroidSurface(std::shared_ptr<const Instance> instance,
            ANativeWindow *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkAndroidSurfaceCreateFlagsKHR flags = 0);
        const ANativeWindow *getWindow() const noexcept { return window; }

    private:
        const ANativeWindow *const window;
    };
#endif // VK_KHR_android_surface
} // namespace magma
