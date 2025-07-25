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
#include "macosSurface.h"
#include "../objects/instance.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_MVK_macos_surface
MacOSSurface::MacOSSurface(Instance *instance,
    const void *view,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkMacOSSurfaceCreateFlagsMVK flags /* 0 */):
    Surface(instance, std::move(allocator)),
    view(view)
{
    VkMacOSSurfaceCreateInfoMVK surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.pView = view;
    const VkResult result = vkCreateMacOSSurfaceMVK(*instance, &surfaceInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create macOS surface");
}
#endif // VK_MVK_macos_surface
} // namespace magma
