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
#include "physicalDevice.h"
#include "displayMode.h"
#include "../allocator/allocator.h"
#include "../misc/instanceExtension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_surface
Surface::Surface(std::shared_ptr<const Instance> instance, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_SURFACE_KHR, nullptr, std::move(allocator)),
    instance(std::move(instance))
{}

Surface::~Surface()
{
    vkDestroySurfaceKHR(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool Surface::hasFullScreenExclusiveSupport() const
{
    bool fullScreenExclusiveSupported = false;
#ifdef VK_EXT_full_screen_exclusive
    if (instance->checkExtensionSupport(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME))
    {
        std::shared_ptr<PhysicalDevice> physicalDevice = instance->getPhysicalDevice(0);
        if (physicalDevice->checkExtensionSupport(VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME))
        {
            fullScreenExclusiveSupported = physicalDevice->getSurfaceFullScreenExclusiveSupport(shared_from_this());
        }
    }
#endif // VK_EXT_full_screen_exclusive
    return fullScreenExclusiveSupported;
}
#endif // VK_KHR_surface

#ifdef VK_KHR_win32_surface
Win32Surface::Win32Surface(std::shared_ptr<const Instance> instance,
    HINSTANCE hInstance,
    HWND hWnd,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkWin32SurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.hinstance = hInstance;
    surfaceInfo.hwnd = hWnd;
    const VkResult result = vkCreateWin32SurfaceKHR(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Win32 surface");
}
#endif // VK_KHR_win32_surface

#ifdef VK_KHR_xlib_surface
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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Xlib surface");
}
#endif // VK_KHR_xlib_surface

#ifdef VK_KHR_xcb_surface
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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Xcb surface");
}
#endif // VK_KHR_xcb_surface

#ifdef VK_KHR_wayland_surface
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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Wayland surface");
}
#endif // VK_KHR_wayland_surface

#ifdef VK_QNX_screen_surface
QnxSurface::QnxSurface(std::shared_ptr<const Instance> instance,
    struct _screen_context *context,
    struct _screen_window *window,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkScreenSurfaceCreateFlagsQNX flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkScreenSurfaceCreateInfoQNX surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_SCREEN_SURFACE_CREATE_INFO_QNX;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = false;
    surfaceInfo.context = context;
    surfaceInfo.window = window;
    const VkResult result = vkCreateScreenSurfaceQNX(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create QNX surface");
}
#endif // VK_QNX_screen_surface

#ifdef VK_KHR_android_surface
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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Android surface");
}
#endif // VK_KHR_android_surface

#ifdef VK_FUCHSIA_imagepipe_surface
FuchsiaImagePipeSurface::FuchsiaImagePipeSurface(std::shared_ptr<const Instance> instance, zx_handle_t imagePipeHandle,
    std::shared_ptr<IAllocator> allocator /* nullptr */, VkImagePipeSurfaceCreateFlagsFUCHSIA flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkImagePipeSurfaceCreateInfoFUCHSIA surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_IMAGEPIPE_SURFACE_CREATE_INFO_FUCHSIA;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.imagePipeHandle = imagePipeHandle;
    const VkResult result = vkCreateImagePipeSurfaceFUCHSIA(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Fuchsia image pipe surface");
}
#endif // VK_FUCHSIA_imagepipe_surface

#ifdef VK_MVK_ios_surface
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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create iOS surface");
}
#endif // VK_MVK_ios_surface

#ifdef VK_MVK_macos_surface
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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create macOS surface");
}
#endif // VK_MVK_macos_surface

#ifdef VK_EXT_metal_surface
MetalSurface::MetalSurface(std::shared_ptr<const Instance> instance,
    const CAMetalLayer *layer,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkMetalSurfaceCreateFlagsEXT flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkMetalSurfaceCreateInfoEXT surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.pLayer = layer;
    const VkResult result = vkCreateMetalSurfaceEXT(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Metal surface");
}
#endif // VK_EXT_metal_surface

#ifdef VK_NN_vi_surface
ViSurface::ViSurface(std::shared_ptr<const Instance> instance,
    void *window,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkViSurfaceCreateFlagsNN flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkViSurfaceCreateInfoNN surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.window = window;
    const VkResult result = vkCreateViSurfaceNN(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create Vi surface");
}
#endif // VK_NN_vi_surface

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
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create display surface");
}
#endif // VK_KHR_display

#ifdef VK_EXT_headless_surface
HeadlessSurface::HeadlessSurface(std::shared_ptr<const Instance> instance,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkWin32SurfaceCreateFlagsKHR flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkHeadlessSurfaceCreateInfoEXT surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    MAGMA_INSTANCE_EXTENSION(vkCreateHeadlessSurfaceEXT, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
    const VkResult result = vkCreateHeadlessSurfaceEXT(MAGMA_HANDLE(instance), &surfaceInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_THROW_SURFACE_FAILURE(result, "failed to create headless surface");
}
#endif // VK_EXT_headless_surface
} // namespace magma
