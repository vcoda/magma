/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "errorHandler.h"
#include "errorResult.h"
#include "reflectionErrorResult.h"

namespace magma::exception
{
#ifdef MAGMA_NO_EXCEPTIONS
static ExceptionHandler exceptionHandler =
    [](std::string_view message, const source_location&)
    {   // If no exception handler is provided, abort program
        std::cerr << message << std::endl;
        abort();
    };

static ErrorHandler errorHandler =
    [](VkResult, const char *message, const source_location&)
    {   // If no Vulkan error handler is provided, abort program
        std::cerr << message << std::endl;
        abort();
    };

static ReflectionErrorHandler reflectionErrorHandler =
    [](SpvReflectResult, const char *message, const source_location&)
    {   // If no SPIRV-Reflect error handler is provided, abort program
        std::cerr << message << std::endl;
        abort();
    };

void setExceptionHandler(ExceptionHandler handler) noexcept
{
    exceptionHandler = std::move(handler);
}

void setErrorHandler(ErrorHandler handler) noexcept
{
    errorHandler = std::move(handler);
}

void setReflectionErrorHandler(ReflectionErrorHandler handler) noexcept
{
    reflectionErrorHandler = std::move(handler);
}
#endif // MAGMA_NO_EXCEPTIONS

void handleException(std::string_view message, const source_location& location)
{
#ifdef MAGMA_NO_EXCEPTIONS
    exceptionHandler(std::move(message), location);
#else
    throw Exception(std::move(message), location);
#endif
}

void handleResult(VkResult result, const char *message, const source_location& location)
{
    switch (result)
    {
    case VK_SUCCESS:
    case VK_NOT_READY:
    case VK_TIMEOUT:
    case VK_EVENT_SET:
    case VK_EVENT_RESET:
    case VK_INCOMPLETE:
#ifdef VK_KHR_swapchain
    case VK_SUBOPTIMAL_KHR:
#endif
        break;

#ifdef MAGMA_NO_EXCEPTIONS
    default:
        errorHandler(result, message, location);
#else
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        throw OutOfHostMemory(message, location);
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        throw OutOfDeviceMemory(message, location);
    case VK_ERROR_DEVICE_LOST:
        throw DeviceLost(message, location);
#ifdef VK_KHR_swapchain
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        throw NativeWindowInUse(message);
    case VK_ERROR_OUT_OF_DATE_KHR:
        throw OutOfDate(message);
#endif // VK_KHR_swapchain
#ifdef VK_KHR_surface
    case VK_ERROR_SURFACE_LOST_KHR:
        throw SurfaceLost(message);
#endif
#ifdef VK_EXT_full_screen_exclusive
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        throw FullScreenExclusiveModeLost(message);
#endif
    default:
        throw ErrorResult(result, message, location);
#endif // MAGMA_NO_EXCEPTIONS
    }
}

void handleReflectionResult(SpvReflectResult result, const char *message, const source_location& location)
{
    switch (result)
    {
    case SPV_REFLECT_RESULT_SUCCESS:
    case SPV_REFLECT_RESULT_NOT_READY:
        break;
    default:
    #ifdef MAGMA_NO_EXCEPTIONS
        errorHandler(result, message, location);
    #else
        throw ReflectionErrorResult(result, message, location);
    #endif // MAGMA_NO_EXCEPTIONS
    }
}
} // namespace magma::exception
