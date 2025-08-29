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

#define MAGMA_EXTENSION_PREFIX "VK_"

#define MAGMA_LAYER_PREFIX "VK_LAYER_"

#if defined(VK_KHR_buffer_device_address) || defined(VK_EXT_buffer_device_address)
    #define MAGMA_NULL VkDeviceAddress(0ull)
#endif

#if defined(_DEBUG) || defined(DEBUG) || defined(QT_DEBUG)
    #define MAGMA_DEBUG
#endif

#if defined(__LP64__) || defined(_WIN64) ||\
    (defined(__x86_64__) && !defined(__ILP32__)) ||\
    defined(_M_X64) || defined(__ia64) || defined (_M_IA64) ||\
    defined(__aarch64__) || defined(__powerpc64__)
    #define MAGMA_X64
#endif

#ifndef VK_USE_64_BIT_PTR_DEFINES
#ifdef MAGMA_X64
    #define VK_USE_64_BIT_PTR_DEFINES 1
#else
    #define VK_USE_64_BIT_PTR_DEFINES 0
#endif // MAGMA_X64
#endif // !VK_USE_64_BIT_PTR_DEFINES

#if defined(__COUNTER__)
    #define MAGMA_COUNTER __COUNTER__
#else
    #define MAGMA_COUNTER __LINE__
#endif

#ifdef MAGMA_DEBUG
    #define MAGMA_NOOP
#else
    #define MAGMA_NOOP {}
#endif // MAGMA_DEBUG

#ifdef MAGMA_NO_EXCEPTIONS
    #define MAGMA_THROW return
#else
    #define MAGMA_THROW throw
#endif // MAGMA_NO_EXCEPTIONS

// Qt defines this as macro
#ifdef foreach
#undef foreach
#endif
