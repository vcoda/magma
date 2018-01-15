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
#pragma once
#include "handle.h"

namespace magma
{
    class Instance;

    class Surface : public NonDispatchable<VkSurfaceKHR>
    {
    public:
	    Surface(std::shared_ptr<const Instance> instance);
	    virtual ~Surface();

    protected:
        std::shared_ptr<const Instance> instance;
    };

#ifdef VK_USE_PLATFORM_WIN32_KHR
    class Win32Surface : public Surface
    {
    public:
	    Win32Surface(std::shared_ptr<const Instance> instance, 
            HINSTANCE hinstance, 
            HWND hwnd,
		    VkWin32SurfaceCreateFlagsKHR flags = 0);
    };
#endif // VK_USE_PLATFORM_WIN32_KHR
} // namespace magma
