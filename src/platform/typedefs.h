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

namespace magma
{
#ifdef VK_KHR_android_surface
    MAGMA_TYPEDEF_MANAGED_PTR(AndroidSurface);
#endif
#ifdef VK_EXT_directfb_surface
    MAGMA_TYPEDEF_MANAGED_PTR(DirectFBSurface);
#endif
#ifdef VK_FUCHSIA_imagepipe_surface
    MAGMA_TYPEDEF_MANAGED_PTR(FuchsiaImagePipeSurface);
#endif
#ifdef VK_MVK_ios_surface
    MAGMA_TYPEDEF_MANAGED_PTR(iOSSurface);
#endif
#ifdef VK_MVK_macos_surface
    MAGMA_TYPEDEF_MANAGED_PTR(MacOSSurface);
#endif
#ifdef VK_EXT_metal_surface
    MAGMA_TYPEDEF_MANAGED_PTR(MetalSurface);
#endif
#ifdef VK_QNX_screen_surface
    MAGMA_TYPEDEF_MANAGED_PTR(QnxSurface);
#endif
 #ifdef VK_NN_vi_surface
    MAGMA_TYPEDEF_MANAGED_PTR(ViSurface);
#endif
#ifdef VK_KHR_wayland_surface
    MAGMA_TYPEDEF_MANAGED_PTR(WaylandSurface);
#endif
#ifdef VK_KHR_win32_surface
    MAGMA_TYPEDEF_MANAGED_PTR(Win32Surface);
#endif
#ifdef VK_KHR_xcb_surface
    MAGMA_TYPEDEF_MANAGED_PTR(XcbSurface);
#endif
#ifdef VK_KHR_xlib_surface
    MAGMA_TYPEDEF_MANAGED_PTR(XlibSurface);
#endif

#ifdef VK_KHR_external_semaphore_win32
    MAGMA_TYPEDEF_MANAGED_PTR(D3d12ExternalSemaphore)
    #ifdef VK_KHR_timeline_semaphore
    MAGMA_TYPEDEF_MANAGED_PTR(D3d12ExternalTimelineSemaphore)
    #endif
#endif // VK_KHR_external_semaphore_win32
} // namespace magma
