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

#ifdef _MSC_VER
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4189) // local variable is initialized but not referenced
#pragma warning(disable: 4505) // unreferenced local function has been removed
#endif

#define VMA_IMPLEMENTATION
#include "../third-party/VulkanMemoryAllocator/include/vk_mem_alloc.h"
