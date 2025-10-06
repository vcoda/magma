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
#include "errorHandler.h"
#ifndef MAGMA_NO_EXCEPTIONS
#include "exception.h"

namespace magma::exception
{
    /* Run time error codes are returned when a command
       needs to communicate a failure that could only be
       detected at runtime. */

    class Error : public Exception
    {
    public:
        explicit Error(VkResult error, const char *message) noexcept:
            Exception(message), error(error) {}
        explicit Error(VkResult error, const char *message,
            const source_location& location) noexcept:
            Exception(message, location), error(error) {}
        VkResult result() const noexcept { return error; }
        const char *description() const noexcept;

    private:
        const VkResult error;
    };

    /* A host memory allocation has failed. */

    class OutOfHostMemory : public Error
    {
    public:
        explicit OutOfHostMemory(const char *message,
            const source_location& location) noexcept:
            Error(VK_ERROR_OUT_OF_HOST_MEMORY, message, location) {}
    };

    /* A device memory allocation has failed. */

    class OutOfDeviceMemory : public Error
    {
    public:
        explicit OutOfDeviceMemory(const char *message,
            const source_location& location) noexcept:
            Error(VK_ERROR_OUT_OF_DEVICE_MEMORY, message, location) {}
    };

    /* Initialization of an object could not be completed
       for implementation-specific reasons. */

    class InitializationFailed : public Error
    {
    public:
        explicit InitializationFailed(const char *message) noexcept:
            Error(VK_ERROR_INITIALIZATION_FAILED, message) {}
    };

    /* The logical or physical device has been lost. */

    class DeviceLost : public Error
    {
    public:
        explicit DeviceLost(const char *message,
            const source_location& location) noexcept:
            Error(VK_ERROR_DEVICE_LOST, message, location) {}
    };

    /* A requested extension is not supported. */

    class ExtensionNotPresent : public Error
    {
    public:
        explicit ExtensionNotPresent(const char *message,
            const source_location& location) noexcept:
            Error(VK_ERROR_EXTENSION_NOT_PRESENT, message, location) {}
    };

    /* A requested feature is not supported. */

    class FeatureNotPresent : public Error
    {
    public:
        explicit FeatureNotPresent(const char *message,
            const source_location& location) noexcept:
            Error(VK_ERROR_FEATURE_NOT_PRESENT, message, location) {}
    };

    /* The requested version of Vulkan is not supported
       by the driver or is otherwise incompatible for
       implementation-specific reasons. */

    class IncompatibleDriver : public Error
    {
    public:
        explicit IncompatibleDriver(const char *message) noexcept:
            Error(VK_ERROR_INCOMPATIBLE_DRIVER, message) {}
    };

    /* A surface is no longer available. */

#ifdef VK_KHR_surface
    class SurfaceLost : public Error
    {
    public:
        explicit SurfaceLost(const char *message) noexcept:
            Error(VK_ERROR_SURFACE_LOST_KHR, message) {}
    };

    /* The requested window is already connected to a surface. */

    class NativeWindowInUse : public Error
    {
    public:
        explicit NativeWindowInUse(const char *message) noexcept:
            Error(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, message) {}
    };
#endif // VK_KHR_surface

    /* A surface has changed in such a way that it is no longer
       compatible with the swapchain, and further presentation
       requests using the swapchain will fail. Applications must
       query the new surface properties and recreate their swapchain
       if they wish to continue presenting to the surface. */

#ifdef VK_KHR_swapchain
    class OutOfDate : public Error
    {
    public:
        explicit OutOfDate(const char *message) noexcept:
            Error(VK_ERROR_OUT_OF_DATE_KHR, message) {}
    };
#endif // VK_KHR_swapchain

    /* The display used by a swapchain does not use the same
       presentable image layout, or is incompatible in a way
       that prevents sharing an image. */

#ifdef VK_KHR_display_swapchain
    class IncompatibleDisplay : public Error
    {
    public:
        explicit IncompatibleDisplay(const char *message) noexcept:
            Error(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, message) {}
    };
#endif // VK_KHR_display_swapchain

    /* An operation on a swapchain created with
       VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT
       failed as it did not have exlusive full-screen access.
       This may occur due to implementation-dependent reasons,
       outside of the application's control. */

#ifdef VK_EXT_full_screen_exclusive
    class FullScreenExclusiveModeLost : public Error
    {
    public:
        explicit FullScreenExclusiveModeLost(const char *message) noexcept:
            Error(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, message) {}
    };
#endif // VK_EXT_full_screen_exclusive
} // namespace magma::exception
#endif // !MAGMA_NO_EXCEPTIONS
