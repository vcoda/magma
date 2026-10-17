/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#include "ubmSurface.h"
#include "../objects/instance.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_SEC_ubm_surface
UbmSurface::UbmSurface(Instance *instance,
    ubm_device *device, ubm_surface *surface,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkUbmSurfaceCreateFlagsSEC flags /* 0 */):
    Surface(instance, std::move(allocator)),
    device(device),
    surface(surface)
{
    VkUbmSurfaceCreateInfoSEC surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_UBM_SURFACE_CREATE_INFO_SEC;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.flags = flags;
    surfaceInfo.device = device;
    surfaceInfo.surface = surface;
    const VkResult result = vkCreateUbmSurfaceSEC(*instance, &surfaceInfo,
        MAGMA_OPTIONAL(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create Ubm surface");
}
#endif // VK_SEC_ubm_surface
} // namespace magma
