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
       that refers to a UBM ubm_surface. */

#ifdef VK_SEC_ubm_surface
    class UbmSurface : public Surface
    {
    public:
        explicit UbmSurface(Instance *instance,
            ubm_device *device,
            ubm_surface *surface,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkUbmSurfaceCreateFlagsSEC flags = 0);
        const ubm_device *getDevice() const noexcept { return device; }
        const ubm_surface *getSurface() const noexcept { return surface; }

    private:
        const ubm_device *const device;
        const ubm_surface *const surface;
    };
#endif // VK_SEC_ubm_surface
} // namespace magma
