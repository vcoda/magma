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

    class Surface : public NonDispatchable<VkSurfaceKHR>
    {
    public:
        ~Surface();

    protected:
        explicit Surface(std::shared_ptr<const Instance> instance,
            std::shared_ptr<IAllocator> allocator);
        std::shared_ptr<const Instance> getInstance() const noexcept { return instance; }

    protected:
        std::shared_ptr<const Instance> instance;
    };

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    class Win32Surface : public Surface
    {
    public:
        explicit Win32Surface(std::shared_ptr<const Instance> instance,
            HINSTANCE hinstance,
            HWND hwnd,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkWin32SurfaceCreateFlagsKHR flags = 0);
    };

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    class XlibSurface : public Surface
    {
    public:
        explicit XlibSurface(std::shared_ptr<const Instance> instance,
            ::Display *dpy,
            Window window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkXlibSurfaceCreateFlagsKHR flags = 0);
    };

#elif defined(VK_USE_PLATFORM_XCB_KHR)
    class XcbSurface : public Surface
    {
    public:
        explicit XcbSurface(std::shared_ptr<const Instance> instance,
            xcb_connection_t *connection,
            xcb_window_t window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkXcbSurfaceCreateFlagsKHR flags = 0);
    };

#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    class WaylandSurface : public Surface
    {
    public:
        explicit WaylandSurface(std::shared_ptr<const Instance> instance,
            wl_display *display,
            wl_surface *surface,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkWaylandSurfaceCreateFlagsKHR flags = 0);
    };

#elif defined(VK_USE_PLATFORM_MIR_KHR)
    class MirSurface : public Surface
    {
    public:
        explicit MirSurface(std::shared_ptr<const Instance> instance,
            MirConnection *connection,
            MirSurface *surface,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkMirSurfaceCreateFlagsKHR flags = 0);
    };

#elif defined(VK_USE_PLATFORM_SCREEN_QNX)

    class QnxSurface : public Surface
    {
    public:
        explicit QnxSurface(std::shared_ptr<const Instance> instance,
            struct _screen_context *context,
            struct _screen_window *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkScreenSurfaceCreateFlagsQNX flags = 0);
    };

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    class AndroidSurface : public Surface
    {
    public:
        explicit AndroidSurface(std::shared_ptr<const Instance> instance,
            ANativeWindow *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkAndroidSurfaceCreateFlagsKHR flags = 0);
    };

#elif defined(VK_USE_PLATFORM_IOS_MVK)
    class iOSSurface : public Surface
    {
    public:
        explicit iOSSurface(std::shared_ptr<const Instance> instance,
            const void *view,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkIOSSurfaceCreateFlagsMVK flags = 0);
    };

#elif defined(VK_USE_PLATFORM_MACOS_MVK)
    class MacOSSurface : public Surface
    {
    public:
        explicit MacOSSurface(std::shared_ptr<const Instance> instance,
            const void *view,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkMacOSSurfaceCreateFlagsMVK flags = 0);
    };

#elif defined(VK_USE_PLATFORM_METAL_EXT)
    class MetalSurface : public Surface
    {
    public:
        explicit MetalSurface(std::shared_ptr<const Instance> instance,
            const CAMetalLayer *layer,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkMacOSSurfaceCreateFlagsMVK flags = 0);
    };

#elif defined(VK_USE_PLATFORM_VI_NN)

    class ViSurface : public Surface
    {
    public:
        explicit ViSurface(std::shared_ptr<const Instance> instance,
            void *window,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkViSurfaceCreateFlagsNN flags = 0);
    };

#endif // VK_USE_PLATFORM_VI_NN

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
} // namespace magma
