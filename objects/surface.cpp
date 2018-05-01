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
#include "surface.h"
#include "instance.h"
#include "../allocator/allocator.h"
#include "../shared.h"

namespace magma
{
Surface::Surface(std::shared_ptr<const Instance> instance, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT, nullptr, allocator),
    instance(instance)
{}

Surface::~Surface()
{
    vkDestroySurfaceKHR(*instance, handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)

Win32Surface::Win32Surface(std::shared_ptr<const Instance> instance,
    HINSTANCE hinstance, 
    HWND hwnd, 
    VkWin32SurfaceCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkWin32SurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.hinstance = hinstance;
    info.hwnd = hwnd;
    const VkResult create = vkCreateWin32SurfaceKHR(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Win32 surface");
}

#elif defined(VK_USE_PLATFORM_XLIB_KHR)

XlibSurface::XlibSurface(std::shared_ptr<const Instance> instance,
    Display *dpy,
    Window window,
    VkXlibSurfaceCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkXlibSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.dpy = dpy;
    info.window = window;
    const VkResult create = vkCreateXlibSurfaceKHR(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Xlib surface");
}

#elif defined(VK_USE_PLATFORM_XCB_KHR)

XcbSurface::XcbSurface(std::shared_ptr<const Instance> instance,
    xcb_connection_t *connection,
    xcb_window_t window,
    VkXcbSurfaceCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkXcbSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.connection = connection;
    info.window = window;
    const VkResult create = vkCreateXcbSurfaceKHR(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create XCB surface");
}

#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)

WaylandSurface::WaylandSurface(std::shared_ptr<const Instance> instance,
    wl_display *display,
    wl_surface *surface,
    VkWaylandSurfaceCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkWaylandSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.display = display;
    info.surface = surface;
    const VkResult create = vkCreateWaylandSurfaceKHR(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Wayland surface");
}

#elif defined(VK_USE_PLATFORM_MIR_KHR)

MirSurface::MirSurface(std::shared_ptr<const Instance> instance,
    MirConnection *connection,
    MirSurface *surface,
    VkMirSurfaceCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkMirSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.connection = connection;
    info.mirSurface = surface;
    const VkResult create = vkCreateMirSurfaceKHR(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Mir surface");
}

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)

AndroidSurface::AndroidSurface(std::shared_ptr<const Instance> instance,
    ANativeWindow *window,
    VkAndroidSurfaceCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkAndroidSurfaceCreateInfoKHR info.
    info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = flags;
    info.window = window;
    const VkResult create = vkCreateAndroidSurfaceKHR(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Android surface");
}

#elif defined(VK_USE_PLATFORM_IOS_MVK)

IosSurface::IosSurface(std::shared_ptr<const Instance> instance,
    const void *view,
    VkIOSSurfaceCreateFlagsMVK flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkIOSSurfaceCreateInfoMVK info.
    info.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
    info.pNext = nullptr;
    info.flags = flags;
    info.pView = view;
    const VkResult create = vkCreateIOSSurfaceMVK(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create iOS surface");
}

#elif defined(VK_USE_PLATFORM_MACOS_MVK)

MacosSurface::MacosSurface(std::shared_ptr<const Instance> instance,
    const void *view,
    VkMacOSSurfaceCreateFlagsMVK flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(instance, allocator)
{
    VkMacOSSurfaceCreateInfoMVK info.
    info.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    info.pNext = nullptr;
    info.flags = flags;
    info.pView = view;
    const VkResult create = vkCreateMacOSSurfaceMVK(*instance, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create macOS surface");
}

#endif // VK_USE_PLATFORM_MACOS_MVK
} // namespace magma
