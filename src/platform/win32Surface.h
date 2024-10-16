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
#pragma once
#include "../objects/surface.h"

namespace magma
{
    /*  Provides a mechanism to create a surface object
        that refers to a Win32 HWND. */

#ifdef VK_KHR_win32_surface
    class Win32Surface : public Surface
    {
    public:
        explicit Win32Surface(std::shared_ptr<Instance> instance,
            HINSTANCE hInstance,
            HWND hWnd,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkWin32SurfaceCreateFlagsKHR flags = 0);
        HINSTANCE getHInstance() const noexcept { return hInstance; }
        HWND getWindow() const noexcept { return hWnd; }

    private:
        const HINSTANCE hInstance;
        const HWND hWnd;
    };
#endif // VK_KHR_win32_surface
} // namespace magma
