/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include <exception>

namespace magma
{
    namespace exception
    {
        /* Base exception class.
           Provides (optional) information as file name and
           line number where it was thrown. */

        class Exception : public std::exception
        {
        public:
            explicit Exception() noexcept;
            explicit Exception(const char *message) noexcept;
            explicit Exception(const std::string message) noexcept;
            explicit Exception(const char *message,
                const char *file, long line) noexcept;
            explicit Exception(const std::string message,
                const char *file, long line) noexcept;
            Exception(const Exception&) noexcept;
            virtual ~Exception() = default;
            Exception& operator=(const Exception&) noexcept;
            const char* what() const noexcept override;
            const char *file() const noexcept { return file_; }
            long line() const noexcept { return line_; }

        private:
            const char *builtin;
            std::string message;
            const char *file_;
            long line_;
        };

        /* Run time error codes are returned when a command needs to communicate
           a failure that could only be detected at runtime. */

        class ErrorResult : public Exception
        {
        public:
            explicit ErrorResult(VkResult result, const char *message) noexcept;
            explicit ErrorResult(VkResult result, const std::string message) noexcept;
            explicit ErrorResult(VkResult result, const char *message,
                const char *file, long line) noexcept;
            explicit ErrorResult(VkResult result, const std::string message,
                const char *file, long line) noexcept;
            VkResult error() const noexcept { return result; }

        private:
            VkResult result;
        };

        /* A host memory allocation has failed. */

        class OutOfHostMemory : public ErrorResult
        {
        public:
            explicit OutOfHostMemory(const char *message) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_HOST_MEMORY, message) {}
        };

        /* A device memory allocation has failed. */

        class OutOfDeviceMemory : public ErrorResult
        {
        public:
            explicit OutOfDeviceMemory(const char *message) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_DEVICE_MEMORY, message) {}
        };

        /* Initialization of an object could not be completed
           for implementation-specific reasons. */

        class InitializationFailed : public ErrorResult
        {
        public:
            explicit InitializationFailed(const char *message) noexcept:
                ErrorResult(VK_ERROR_INITIALIZATION_FAILED, message) {}
            explicit InitializationFailed(const std::string message) noexcept:
                ErrorResult(VK_ERROR_INITIALIZATION_FAILED, std::move(message)) {}
        };

        /* The logical or physical device has been lost. */

        class DeviceLost : public ErrorResult
        {
        public:
            explicit DeviceLost(const char *const message) noexcept:
                ErrorResult(VK_ERROR_DEVICE_LOST, message) {}
        };

        /* The requested version of Vulkan is not supported by the driver
           or is otherwise incompatible for implementation-specific reasons. */

        class IncompatibleDriver : public ErrorResult
        {
        public:
            explicit IncompatibleDriver(const char *const message) noexcept:
                ErrorResult(VK_ERROR_INCOMPATIBLE_DRIVER, message) {}
        };

        /* A surface is no longer available. */

        class SurfaceLost : public ErrorResult
        {
        public:
            explicit SurfaceLost(const char *const message) noexcept:
                ErrorResult(VK_ERROR_SURFACE_LOST_KHR, message) {}
        };

        /* A surface has changed in such a way that it is no longer compatible with the swapchain,
           and further presentation requests using the swapchain will fail. Applications must query
           the new surface properties and recreate their swapchain if they wish to continue presenting to the surface. */

        class OutOfDate : public ErrorResult
        {
        public:
            explicit OutOfDate(const char *const message) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_DATE_KHR, message) {}
        };

        /* An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT
           failed as it did not have exlusive full-screen access. This may occur due to
           implementation-dependent reasons, outside of the application’s control. */

#ifdef VK_EXT_full_screen_exclusive
        class FullScreenExclusiveModeLost : public ErrorResult
        {
        public:
            explicit FullScreenExclusiveModeLost(const char *const message) noexcept:
                ErrorResult(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, message) {}
        };
#endif // VK_EXT_full_screen_exclusive

        /* Vulkan instance doesn't support requested extension. */

        class UnsupportedInstanceExtension : public Exception
        {
        public:
            explicit UnsupportedInstanceExtension(const char *extension,
                const char *file, long line) noexcept:
                Exception(extension, file, line) {}
        };

        /* Logical device doesn't support requested extension. */

        class UnsupportedDeviceExtension : public Exception
        {
        public:
            explicit UnsupportedDeviceExtension(const char *extension,
                const char *file, long line) noexcept:
                Exception(extension, file, line) {}
        };

        /* Functionality not implemented or implemented partially. */

        class NotImplemented : public Exception
        {
        public:
            explicit NotImplemented(const char *function,
                const char *file, long line) noexcept:
                Exception(function, file, line) {}
        };
    } // namespace exception
} // namespace magma


#define MAGMA_THROW(message) throw magma::exception::Exception(message, __FILE__, __LINE__)

#define MAGMA_THROW_FAILURE(result, message)\
    if (!MAGMA_SUCCEEDED(result))\
        throw magma::exception::ErrorResult(result, message, __FILE__, __LINE__)

#ifdef _MSC_VER
#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(__FUNCSIG__, __FILE__, __LINE__)
#else
#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#endif
