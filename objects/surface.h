/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "nondispatchable.h"

namespace magma
{
    class Instance;

    /* A surface object abstracts a native platform surface or window object for use with Vulkan.
       Separate platform-specific extensions each provide a function for creating a surface object
       for the respective platform. */

#ifdef VK_KHR_surface
    class Surface : public NonDispatchable<VkSurfaceKHR>,
        public std::enable_shared_from_this<Surface>
    {
    public:
        ~Surface();
        std::shared_ptr<const Instance> getInstance() const noexcept { return instance; }
        bool hasFullScreenExclusiveSupport() const;

    protected:
        explicit Surface(std::shared_ptr<const Instance> instance,
            std::shared_ptr<IAllocator> allocator);

        std::shared_ptr<const Instance> instance;
    };
#endif // VK_KHR_surface

    /*  Provides a mechanism to create a surface object that refers to a Win32 HWND. */

#ifdef VK_KHR_win32_surface
    class Win32Surface : public Surface
    {
    public:
        explicit Win32Surface(std::shared_ptr<const Instance> instance,
            HINSTANCE hInstance,
            HWND hWnd,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkWin32SurfaceCreateFlagsKHR flags = 0);
    };
#endif // VK_KHR_win32_surface

    /* Provides a mechanism to create a surface object that refers to an X11 Window,
       using the Xlib client-side library. */

#ifdef VK_KHR_xlib_surface
    class XlibSurface : public Surface
    {
    public:
        explicit XlibSurface(std::shared_ptr<const Instance> instance,
            ::Display *dpy,
            Window window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkXlibSurfaceCreateFlagsKHR flags = 0);
    };
#endif // VK_KHR_xlib_surface

    /* Provides a mechanism to create a surface object that refers to an X11 Window,
       using the XCB client-side library. */

#ifdef VK_KHR_xcb_surface
    class XcbSurface : public Surface
    {
    public:
        explicit XcbSurface(std::shared_ptr<const Instance> instance,
            xcb_connection_t *connection,
            xcb_window_t window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkXcbSurfaceCreateFlagsKHR flags = 0);
    };
#endif // VK_KHR_xcb_surface

    /* Provides a mechanism to create a surface object that refers to a Wayland wl_surface. */

#ifdef VK_KHR_wayland_surface
    class WaylandSurface : public Surface
    {
    public:
        explicit WaylandSurface(std::shared_ptr<const Instance> instance,
            wl_display *display,
            wl_surface *surface,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkWaylandSurfaceCreateFlagsKHR flags = 0);
    };
#endif // VK_KHR_wayland_surface

    /* Provides a mechanism to create a surface object that refers to a QNX Screen window. */

#ifdef VK_QNX_screen_surface
    class QnxSurface : public Surface
    {
    public:
        explicit QnxSurface(std::shared_ptr<const Instance> instance,
            struct _screen_context *context,
            struct _screen_window *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkScreenSurfaceCreateFlagsQNX flags = 0);
    };
#endif // VK_QNX_screen_surface

    /* Provides a mechanism to create a surface object that refers to an ANativeWindow,
       Android's native surface type. */

#ifdef VK_KHR_android_surface
    class AndroidSurface : public Surface
    {
    public:
        explicit AndroidSurface(std::shared_ptr<const Instance> instance,
            ANativeWindow *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkAndroidSurfaceCreateFlagsKHR flags = 0);
    };
#endif // VK_KHR_android_surface

    /* Provides a mechanism to create a surface object based on a UIView,
       the native surface type of iOS, which is underpinned by a CAMetalLayer,
       to support rendering to the surface using Apple's Metal framework. */

#ifdef VK_MVK_ios_surface
    class iOSSurface : public Surface
    {
    public:
        explicit iOSSurface(std::shared_ptr<const Instance> instance,
            const void *view,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkIOSSurfaceCreateFlagsMVK flags = 0);
    };
#endif // VK_MVK_ios_surface

    /* Provides a mechanism to create a surface object based on an NSView,
       the native surface type of macOS, which is underpinned by a CAMetalLayer,
       to support rendering to the surface using Apple's Metal framework. */

#ifdef VK_MVK_macos_surface
    class MacOSSurface : public Surface
    {
    public:
        explicit MacOSSurface(std::shared_ptr<const Instance> instance,
            const void *view,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkMacOSSurfaceCreateFlagsMVK flags = 0);
    };
#endif // VK_MVK_macos_surface

    /* Provides a mechanism to create a surface object from CAMetalLayer,
       which is the native rendering surface of Apple's Metal framework. */

#ifdef VK_EXT_metal_surface
    class MetalSurface : public Surface
    {
    public:
        explicit MetalSurface(std::shared_ptr<const Instance> instance,
            const CAMetalLayer *layer,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkMetalSurfaceCreateFlagsEXT flags = 0);
    };
#endif // VK_EXT_metal_surface

    /* Provides a mechanism to create a surface object associated with an nn::vi::Layer. */

#ifdef VK_NN_vi_surface
    class ViSurface : public Surface
    {
    public:
        explicit ViSurface(std::shared_ptr<const Instance> instance,
            void *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkViSurfaceCreateFlagsNN flags = 0);
    };
#endif // VK_NN_vi_surface

    /* Display surface describes the configuration of a single plane
       within a complete display configuration. */

#ifdef VK_KHR_display
    class DisplayMode;

    class DisplaySurface : public Surface
    {
    public:
        explicit DisplaySurface(std::shared_ptr<const Instance> instance,
            std::shared_ptr<const DisplayMode> displayMode,
            uint32_t planeIndex,
            uint32_t planeStackIndex,
            VkSurfaceTransformFlagBitsKHR transform,
            VkDisplayPlaneAlphaFlagBitsKHR alphaMode,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
#endif // VK_KHR_display

    /* Provides a mechanism to create surface objects independently of
       any window system or display device. The presentation operation
       for a swapchain created from a headless surface is by default a no-op,
       resulting in no externally-visible result. */

#ifdef VK_EXT_headless_surface
    class HeadlessSurface : public Surface
    {
    public:
        explicit HeadlessSurface(std::shared_ptr<const Instance> instance,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkHeadlessSurfaceCreateFlagsEXT flags = 0);
    };
#endif // VK_EXT_headless_surface
} // namespace magma
