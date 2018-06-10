/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "handle.h"

namespace magma
{
    class Instance;

    class Surface : public NonDispatchable<VkSurfaceKHR>
    {
    public:
        ~Surface();

    protected:
        Surface(std::shared_ptr<const Instance> instance,
            std::shared_ptr<IAllocator> allocator);

    protected:
        std::shared_ptr<const Instance> instance;
    };

#if defined(VK_USE_PLATFORM_WIN32_KHR)

    class Win32Surface : public Surface
    {
    public:
        Win32Surface(std::shared_ptr<const Instance> instance,
            HINSTANCE hinstance, 
            HWND hwnd,
            VkWin32SurfaceCreateFlagsKHR flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_XLIB_KHR)

    class XlibSurface : public Surface
    {
    public:
        XlibSurface(std::shared_ptr<const Instance> instance,
            Display *dpy,
            Window window,
            VkXlibSurfaceCreateFlagsKHR flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_XCB_KHR)

    class XcbSurface : public Surface
    {
    public:
        XcbSurface(std::shared_ptr<const Instance> instance,
            xcb_connection_t *connection,
            xcb_window_t window,
            VkXcbSurfaceCreateFlagsKHR flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)

    class WaylandSurface : public Surface
    {
    public:
        WaylandSurface(std::shared_ptr<const Instance> instance,
            wl_display *display,
            wl_surface *surface,
            VkWaylandSurfaceCreateFlagsKHR flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_MIR_KHR)

    class MirSurface : public Surface
    {
    public:
        MirSurface(std::shared_ptr<const Instance> instance,
            MirConnection *connection,
            MirSurface *surface,
            VkMirSurfaceCreateFlagsKHR flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)

    class AndroidSurface : public Surface
    {
    public:
        AndroidSurface(std::shared_ptr<const Instance> instance,
            ANativeWindow *window,
            VkAndroidSurfaceCreateFlagsKHR flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_IOS_MVK)

    class IosSurface : public Surface
    {
    public:
        IosSurface(std::shared_ptr<const Instance> instance,
            const void *view,
            VkIOSSurfaceCreateFlagsMVK flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#elif defined(VK_USE_PLATFORM_MACOS_MVK)

    class MacosSurface : public Surface
    {
    public:
        MacosSurface(std::shared_ptr<const Instance> instance,
            const void *view,
            VkMacOSSurfaceCreateFlagsMVK flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

#endif // VK_USE_PLATFORM_MACOS_MVK

    class DisplayMode;

    class DisplaySurface : public Surface
    {
    public:
        DisplaySurface(std::shared_ptr<const Instance> instance,
            std::shared_ptr<const DisplayMode> displayMode, 
            uint32_t planeIndex, 
            uint32_t planeStackIndex,
            VkSurfaceTransformFlagBitsKHR transform,
            VkDisplayPlaneAlphaFlagBitsKHR alphaMode,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma
