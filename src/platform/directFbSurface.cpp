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
#include "directFbSurface.h"
#include "../objects/instance.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_EXT_directfb_surface
DirectFBSurface::DirectFBSurface(Instance *instance,
    IDirectFB *dfb, IDirectFBSurface *surface,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDirectFBSurfaceCreateFlagsEXT flags /* 0 */):
    Surface(instance, std::move(allocator)),
{
    VkDirectFBSurfaceCreateInfoEXT surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_DIRECTFB_SURFACE_CREATE_INFO_EXT;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.dfb = dfb;
    surfaceInfo.surface = surface;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkCreateDirectFBSurfaceEXT, VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME);
    const VkResult result = vkCreateDirectFBSurfaceEXT(*instance, &surfaceInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create DirectFB surface");
}
#endif // VK_EXT_directfb_surface
} // namespace magma
