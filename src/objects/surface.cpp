/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_surface
Surface::Surface(std::shared_ptr<const Instance> instance, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_SURFACE_KHR, std::move(allocator)),
    instance(std::move(instance))
{}

Surface::~Surface()
{
    vkDestroySurfaceKHR(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

bool Surface::supportsFullScreenExclusive() const
{
#ifdef VK_EXT_full_screen_exclusive
    std::shared_ptr<const Surface> self = shared_from_this();
    uint32_t physicalDeviceCount = instance->enumeratePhysicalDevices();
    for (uint32_t deviceId = 0; deviceId < physicalDeviceCount; ++deviceId)
    {
        std::shared_ptr<PhysicalDevice> physicalDevice = instance->getPhysicalDevice(deviceId);
        if (physicalDevice->extensionSupported(VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME))
        {
            if (physicalDevice->getFullScreenExclusiveSurfaceSupport(self))
                return true;
        }
    }
#endif // VK_EXT_full_screen_exclusive
    return false;
}
#endif // VK_KHR_surface

#ifdef VK_KHR_display
DisplaySurface::DisplaySurface(std::shared_ptr<const Instance> instance,
    std::shared_ptr<const DisplayMode> displayMode_,
    uint32_t planeIndex, uint32_t planeStackIndex,
    VkSurfaceTransformFlagBitsKHR transform,
    VkDisplayPlaneAlphaFlagBitsKHR alphaMode,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Surface(std::move(instance), std::move(allocator)),
    displayMode(std::move(displayMode_)),
    planeIndex(planeIndex),
    planeStackIndex(planeStackIndex),
    transform(transform),
    alphaMode(alphaMode)
{
    VkDisplaySurfaceCreateInfoKHR displaySurfaceInfo;
    displaySurfaceInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
    displaySurfaceInfo.pNext = nullptr;
    displaySurfaceInfo.flags = 0;
    displaySurfaceInfo.displayMode = MAGMA_HANDLE(displayMode);
    displaySurfaceInfo.planeIndex = planeIndex;
    displaySurfaceInfo.planeStackIndex = planeStackIndex;
    displaySurfaceInfo.transform = transform;
    displaySurfaceInfo.globalAlpha = 1.f;
    displaySurfaceInfo.alphaMode = alphaMode;
    displaySurfaceInfo.imageExtent = displayMode->getVisibleRegion();
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkCreateDisplayPlaneSurfaceKHR, VK_KHR_DISPLAY_EXTENSION_NAME);
    const VkResult result = vkCreateDisplayPlaneSurfaceKHR(MAGMA_HANDLE(instance), &displaySurfaceInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create display surface");
}
#endif // VK_KHR_display

#ifdef VK_EXT_headless_surface
HeadlessSurface::HeadlessSurface(std::shared_ptr<const Instance> instance,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkHeadlessSurfaceCreateFlagsEXT flags /* 0 */):
    Surface(std::move(instance), std::move(allocator))
{
    VkHeadlessSurfaceCreateInfoEXT headlessSurfaceInfo;
    headlessSurfaceInfo.sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT;
    headlessSurfaceInfo.pNext = nullptr;
    headlessSurfaceInfo.flags = flags;
    MAGMA_REQUIRED_INSTANCE_EXTENSION(vkCreateHeadlessSurfaceEXT, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
    const VkResult result = vkCreateHeadlessSurfaceEXT(MAGMA_HANDLE(instance), &headlessSurfaceInfo,
        MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create headless surface");
}
#endif // VK_EXT_headless_surface
} // namespace magma
