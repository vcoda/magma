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
#include "../shared.h"

namespace magma
{
Surface::Surface(std::shared_ptr<const Instance> instance):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT, nullptr),
    instance(instance)
{}

Surface::~Surface()
{
	vkDestroySurfaceKHR(*instance, handle, nullptr);
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
Win32Surface::Win32Surface(std::shared_ptr<const Instance> instance, 
    HINSTANCE hinstance, 
    HWND hwnd, 
    VkWin32SurfaceCreateFlagsKHR flags /* 0 */):
	Surface(instance)
{
	VkWin32SurfaceCreateInfoKHR info;
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.pNext = nullptr;
	info.flags = flags;
	info.hinstance = hinstance;
	info.hwnd = hwnd;
	const VkResult create = vkCreateWin32SurfaceKHR(*instance, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create Win32 surface");
}
#endif // VK_USE_PLATFORM_WIN32_KHR
} // namespace magma
