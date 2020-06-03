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
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

namespace magma
{
    namespace exception
    {
        /* The source_location class represents certain information about the source code,
           such as file names, line numbers, and function names. Previously, functions
           that desire to obtain this information about the call site (for logging, testing,
           or debugging purposes) must use macros so that predefined macros like
           __LINE__ and __FILE__ are expanded in the context of the caller. The source_location
           class provides a better alternative. */

        struct source_location
        {
            /* constexpr */ std::uint_least32_t line() const noexcept { return ln; }
            /* constexpr */ std::uint_least32_t column() const noexcept { return 0; }
            /* constexpr */ const char* file_name() const noexcept { return file; }
            /* constexpr */ const char* function_name() const noexcept { return function; }

            const char *file = nullptr;
            long ln = 0;
            const char *function = nullptr;
        };

        /* Base exception class. Provides (optional) information as
           file name and line number where it was thrown. */

        class Exception : public std::exception
        {
        public:
            explicit Exception() noexcept;
            explicit Exception(const char *message) noexcept;
            explicit Exception(std::string message) noexcept;
            explicit Exception(const char *message,
                const source_location& location) noexcept;
            explicit Exception(std::string message,
                const source_location& location) noexcept;
            Exception(const Exception&) noexcept;
            virtual ~Exception() = default;
            Exception& operator=(const Exception&) noexcept;
            const char* what() const noexcept override;
            const source_location& location() const noexcept { return location_; }

        private:
            std::string message;
            source_location location_;
        };

        /* Run time error codes are returned when a command needs to communicate
           a failure that could only be detected at runtime. */

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
            explicit DeviceLost(const char *message) noexcept:
                ErrorResult(VK_ERROR_DEVICE_LOST, message) {}
        };

        /* The requested version of Vulkan is not supported by the driver
           or is otherwise incompatible for implementation-specific reasons. */

        class IncompatibleDriver : public ErrorResult
        {
        public:
            explicit IncompatibleDriver(const char *message) noexcept:
                ErrorResult(VK_ERROR_INCOMPATIBLE_DRIVER, message) {}
        };

        /* A surface is no longer available. */

        class SurfaceLost : public ErrorResult
        {
        public:
            explicit SurfaceLost(const char *message) noexcept:
                ErrorResult(VK_ERROR_SURFACE_LOST_KHR, message) {}
        };

        /* A surface has changed in such a way that it is no longer compatible with the swapchain,
           and further presentation requests using the swapchain will fail. Applications must query
           the new surface properties and recreate their swapchain if they wish to continue presenting to the surface. */

        class OutOfDate : public ErrorResult
        {
        public:
            explicit OutOfDate(const char *message) noexcept:
                ErrorResult(VK_ERROR_OUT_OF_DATE_KHR, message) {}
        };

        /* The display used by a swapchain does not use the same presentable image layout,
           or is incompatible in a way that prevents sharing an image. */

#ifdef VK_KHR_display_swapchain
        class IncompatibleDisplay : public ErrorResult
        {
        public:
            explicit IncompatibleDisplay(const char *message) noexcept:
                ErrorResult(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, message) {}
        };
#endif // VK_KHR_display_swapchain

        /* An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT
           failed as it did not have exlusive full-screen access. This may occur due to
           implementation-dependent reasons, outside of the application’s control. */

#ifdef VK_EXT_full_screen_exclusive
        class FullScreenExclusiveModeLost : public ErrorResult
        {
        public:
            explicit FullScreenExclusiveModeLost(const char *message) noexcept:
                ErrorResult(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, message) {}
        };
#endif // VK_EXT_full_screen_exclusive

        /* Vulkan instance doesn't support requested extension. */

        class UnsupportedInstanceExtension : public Exception
        {
        public:
            explicit UnsupportedInstanceExtension(const char *extension,
                const source_location& location) noexcept:
                Exception(extension, location) {}
        };

        /* Logical device doesn't support requested extension. */

        class UnsupportedDeviceExtension : public Exception
        {
        public:
            explicit UnsupportedDeviceExtension(const char *extension,
                const source_location& location) noexcept:
                Exception(extension, location) {}
        };

        /* SPIRV-Reflect error result. */

        class ReflectException : public Exception
        {
        public:
            ReflectException(SpvReflectResult result,
                const char *message,
                const source_location& location) noexcept:
                Exception(message, location), result(result) {}
            SpvReflectResult error() const noexcept { return result; }

        private:
            SpvReflectResult result;
        };

        /* Functionality not implemented or implemented partially. */

        class NotImplemented : public Exception
        {
        public:
            explicit NotImplemented(const char *function,
                const source_location& location) noexcept:
                Exception(function, location) {}
        };
    } // namespace exception
} // namespace magma

#define MAGMA_THROW(message) throw magma::exception::Exception(message, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__})

#define MAGMA_THROW_FAILURE(result, message)\
    switch (result) {\
    case VK_SUCCESS:\
    case VK_NOT_READY:\
    case VK_TIMEOUT:\
    case VK_EVENT_SET:\
    case VK_EVENT_RESET:\
    case VK_INCOMPLETE:\
    case VK_SUBOPTIMAL_KHR:\
        break;\
    case VK_ERROR_OUT_OF_HOST_MEMORY:\
        throw magma::exception::OutOfHostMemory(message, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__});\
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:\
        throw magma::exception::OutOfDeviceMemory(message, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__});\
    default:\
        throw magma::exception::ErrorResult(result, message, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__});\
    }

#define MAGMA_THROW_REFLECTION_FAILURE(result, message)\
    switch (result) {\
    case SPV_REFLECT_RESULT_SUCCESS:\
    case SPV_REFLECT_RESULT_NOT_READY:\
        break;\
    default:\
        throw magma::exception::ReflectException(result, message, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__});\
    }

#ifdef _MSC_VER
#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(__FUNCSIG__, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__})
#else
#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(__PRETTY_FUNCTION__, magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__})
#endif
