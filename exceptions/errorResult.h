/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "exception.h"

namespace magma
{
    namespace exception
    {
        /* Run time error codes are returned when a command
           needs to communicate a failure that could only be
           detected at runtime. */

    #ifndef MAGMA_NO_EXCEPTIONS
        class ErrorResult : public Exception
        {
        public:
            explicit ErrorResult(VkResult result, const char *message) noexcept:
                Exception(message), result(result) {}
            explicit ErrorResult(VkResult result, std::string message) noexcept:
                Exception(std::move(message)), result(result) {}
            explicit ErrorResult(VkResult result, const char *message,
                const source_location& location) noexcept:
                Exception(message, location), result(result) {}
            explicit ErrorResult(VkResult result, std::string message,
                const source_location& location) noexcept:
                Exception(std::move(message), location), result(result) {}
            VkResult error() const noexcept { return result; }

        private:
            VkResult result;
        };

        /* A host memory allocation has failed. */

        class OutOfHostMemory : public ErrorResult
        {
        public:
            explicit OutOfHostMemory(const char *message,
                const source_location& location) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_HOST_MEMORY, message, location) {}
        };

        /* A device memory allocation has failed. */

        class OutOfDeviceMemory : public ErrorResult
        {
        public:
            explicit OutOfDeviceMemory(const char *message,
                const source_location& location) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_DEVICE_MEMORY, message, location) {}
        };

        /* Initialization of an object could not be completed
           for implementation-specific reasons. */

        class InitializationFailed : public ErrorResult
        {
        public:
            explicit InitializationFailed(const char *message) noexcept:
                ErrorResult(VK_ERROR_INITIALIZATION_FAILED, message) {}
            explicit InitializationFailed(std::string message) noexcept:
                ErrorResult(VK_ERROR_INITIALIZATION_FAILED, std::move(message)) {}
        };

        /* The logical or physical device has been lost. */

        class DeviceLost : public ErrorResult
        {
        public:
            explicit DeviceLost(const char *message,
                const source_location& location) noexcept:
                ErrorResult(VK_ERROR_DEVICE_LOST, message, location) {}
        };

        /* Mapping of a memory object has failed. */

        class MemoryMapFailed : public ErrorResult
        {
        public:
            explicit MemoryMapFailed(const char *message) noexcept:
                ErrorResult(VK_ERROR_MEMORY_MAP_FAILED, message) {}
        };

        /* The requested version of Vulkan is not supported
           by the driver or is otherwise incompatible for
           implementation-specific reasons. */

        class IncompatibleDriver : public ErrorResult
        {
        public:
            explicit IncompatibleDriver(const char *message) noexcept:
                ErrorResult(VK_ERROR_INCOMPATIBLE_DRIVER, message) {}
        };

        /* A surface is no longer available. */

  #ifdef VK_KHR_surface
        class SurfaceLost : public ErrorResult
        {
        public:
            explicit SurfaceLost(const char *message) noexcept:
                ErrorResult(VK_ERROR_SURFACE_LOST_KHR, message) {}
        };

        /* The requested window is already connected to a surface. */

        class NativeWindowInUse : public ErrorResult
        {
        public:
            explicit NativeWindowInUse(const char *message) noexcept:
                ErrorResult(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, message) {}
        };
    #endif // VK_KHR_surface

        /* A surface has changed in such a way that it is no longer
           compatible with the swapchain, and further presentation
           requests using the swapchain will fail. Applications must
           query the new surface properties and recreate their swapchain
           if they wish to continue presenting to the surface. */

    #ifdef VK_KHR_swapchain
        class OutOfDate : public ErrorResult
        {
        public:
            explicit OutOfDate(const char *message) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_DATE_KHR, message) {}
        };
    #endif // VK_KHR_swapchain

        /* The display used by a swapchain does not use the same
           presentable image layout, or is incompatible in a way
           that prevents sharing an image. */

    #ifdef VK_KHR_display_swapchain
        class IncompatibleDisplay : public ErrorResult
        {
        public:
            explicit IncompatibleDisplay(const char *message) noexcept:
                ErrorResult(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, message) {}
        };
    #endif // VK_KHR_display_swapchain

        /* An operation on a swapchain created with
           VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT
           failed as it did not have exlusive full-screen access.
           This may occur due to implementation-dependent reasons,
           outside of the application's control. */

    #ifdef VK_EXT_full_screen_exclusive
        class FullScreenExclusiveModeLost : public ErrorResult
        {
        public:
            explicit FullScreenExclusiveModeLost(const char *message) noexcept:
                ErrorResult(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, message) {}
        };
    #endif // VK_EXT_full_screen_exclusive
    #endif // !MAGMA_NO_EXCEPTIONS

        /* If C++ exceptions are not enabled, application has an
           option to provide custom error handler which will be
           called when a Vulkan error is encountered. */

    #ifdef MAGMA_NO_EXCEPTIONS
        typedef std::function<void(VkResult, const char *, const source_location&)> ErrorHandler;
        void setErrorHandler(ErrorHandler errorHandler) noexcept;
    #endif // MAGMA_NO_EXCEPTIONS

        void handleResult(VkResult result, const char *message, const source_location& location);
    } // namespace exception
} // namespace magma

#define MAGMA_HANDLE_RESULT(result, message) magma::exception::handleResult(result, message, MAGMA_SOURCE_LOCATION)

