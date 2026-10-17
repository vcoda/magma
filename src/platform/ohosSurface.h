/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
       that refers to an OHNativeWindow, the native surface
       type of Open Harmony OS. Common consumer endpoints
       for OHNativeWindows are the system window compositor,
       video encoders, and application-specific compositors. */

#ifdef VK_OHOS_surface
    class OhOSSurface : public Surface
    {
    public:
        explicit OhOSSurface(Instance *instance,
            OHNativeWindow *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkSurfaceCreateFlagsOHOS flags = 0);
        OHNativeWindow *getWindow() const noexcept { return window; }

    private:
        OHNativeWindow *const window;
    };
#endif // VK_OHOS_surface
} // namespace magma
