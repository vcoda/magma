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
#include "pch.h"
#pragma hdrstop
#include "surface.h"
#include "instance.h"
#include "device.h"
#include "displayMode.h"
#include "../allocator/allocator.h"
#include "../misc/instanceExtension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
Surface::Surface(std::shared_ptr<const Instance> instance, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_SURFACE_KHR, nullptr, std::move(allocator)),
    instance(std::move(instance))
{}

Surface::~Surface()
{
    vkDestroySurfaceKHR(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
Win32Surface::Win32Surface(std::shared_ptr<const Instance> instance,
    HINSTANCE hinstance,
    HWND hwnd,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkWin32SurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.hinstance = hinstance;
    surfaceInfo.hwnd = hwnd;
    const VkResult result = vkCreateWin32SurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Win32 surface");
}

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
XlibSurface::XlibSurface(std::shared_ptr<const Instance> instance,
    ::Display *dpy,
    Window window,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkXlibSurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkXlibSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.dpy = dpy;
    surfaceInfo.window = window;
    const VkResult result = vkCreateXlibSurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Xlib surface");
}

#elif defined(VK_USE_PLATFORM_XCB_KHR)
XcbSurface::XcbSurface(std::shared_ptr<const Instance> instance,
    xcb_connection_t *connection,
    xcb_window_t window,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkXcbSurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkXcbSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.connection = connection;
    surfaceInfo.window = window;
    const VkResult result = vkCreateXcbSurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Xcb surface");
}

#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
WaylandSurface::WaylandSurface(std::shared_ptr<const Instance> instance,
    wl_display *display,
    wl_surface *surface,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkWaylandSurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkWaylandSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.display = display;
    surfaceInfo.surface = surface;
    const VkResult result = vkCreateWaylandSurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Wayland surface");
}

#elif defined(VK_USE_PLATFORM_MIR_KHR)
MirSurface::MirSurface(std::shared_ptr<const Instance> instance,
    MirConnection *connection,
    MirSurface *surface,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkMirSurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkMirSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.connection = connection;
    surfaceInfo.mirSurface = surface;
    const VkResult result = vkCreateMirSurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Mir surface");
}

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
AndroidSurface::AndroidSurface(std::shared_ptr<const Instance> instance,
    ANativeWindow *window,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkAndroidSurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkAndroidSurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.window = window;
    const VkResult result = vkCreateAndroidSurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Android surface");
}

#elif defined(VK_USE_PLATFORM_IOS_MVK)
iOSSurface::iOSSurface(std::shared_ptr<const Instance> instance,
    const void *view,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkIOSSurfaceCreateFlagsMVK flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkIOSSurfaceCreateInfoMVK surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.pView = view;
    const VkResult result = vkCreateIOSSurfaceMVK(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create iOS surface");
}

#elif defined(VK_USE_PLATFORM_MACOS_MVK)
MacOSSurface::MacOSSurface(std::shared_ptr<const Instance> instance,
    const void *view,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkMacOSSurfaceCreateFlagsMVK flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkMacOSSurfaceCreateInfoMVK surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.pView = view;
    const VkResult result = vkCreateMacOSSurfaceMVK(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create macOS surface");
}

#elif defined(VK_USE_PLATFORM_METAL_EXT)
MetalSurface::MetalSurface(std::shared_ptr<const Instance> instance,
    const CAMetalLayer *layer,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkMacOSSurfaceCreateFlagsMVK flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkMetalSurfaceCreateInfoEXT surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.pLayer = layer;
    const VkResult result = vkCreateMetalSurfaceEXT(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to create Metal surface");
}
#endif // VK_USE_PLATFORM_METAL_EXT

#ifdef VK_KHR_display
DisplaySurface::DisplaySurface(std::shared_ptr<const Instance> instance,
    std::shared_ptr<const DisplayMode> displayMode,
    uint32_t planeIndex,
    uint32_t planeStackIndex,
    VkSurfaceTransformFlagBitsKHR transform,
    VkDisplayPlaneAlphaFlagBitsKHR alphaMode,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(std::move(instance), std::move(allocator))
{
    VkDisplaySurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = 0;
    surfaceInfo.displayMode = *displayMode;
    surfaceInfo.planeIndex = planeIndex;
    surfaceInfo.planeStackIndex = planeStackIndex;
    surfaceInfo.transform = transform;
    surfaceInfo.globalAlpha = 1.f;
    surfaceInfo.alphaMode = alphaMode;
    surfaceInfo.imageExtent = displayMode->getVisibleRegion();
    MAGMA_INSTANCE_EXTENSION(vkCreateDisplayPlaneSurfaceKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult result = vkCreateDisplayPlaneSurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_FAILURE(result, "failed to result display surface");
}
#endif // VK_KHR_display
} // namespace magma
