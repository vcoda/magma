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
#include "pch.h"
#pragma hdrstop
#include "qnxSurface.h"
#include "../objects/instance.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_QNX_screen_surface
QnxSurface::QnxSurface(std::shared_ptr<Instance> instance_,
    struct _screen_context *context, struct _screen_window *window,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkScreenSurfaceCreateFlagsQNX flags /* 0 */):
    Surface(std::move(instance_), std::move(allocator)),
    context(context),
    window(window)
{
    VkScreenSurfaceCreateInfoQNX surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_SCREEN_SURFACE_CREATE_INFO_QNX;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = false;
    surfaceInfo.context = context;
    surfaceInfo.window = window;
    const VkResult result = vkCreateScreenSurfaceQNX(*instance, &surfaceInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create QNX surface");
}
#endif // VK_QNX_screen_surface
} // namespace magma
