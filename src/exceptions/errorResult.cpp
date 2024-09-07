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
#include "errorResult.h"

namespace magma::exception
{
#ifdef MAGMA_NO_EXCEPTIONS
static ErrorHandler errorHandler =
    [](VkResult, const char *message, const source_location&)
    {   // If no error handler is provided, abort program
        std::cerr << message << std::endl;
        abort();
    };

void setErrorHandler(ErrorHandler errorHandler_) noexcept
{
    errorHandler = std::move(errorHandler_);
}
#endif // MAGMA_NO_EXCEPTIONS

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
#ifndef MAGMA_NO_EXCEPTIONS
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        throw OutOfHostMemory(message, location);
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        throw OutOfDeviceMemory(message, location);
    case VK_ERROR_DEVICE_LOST:
        throw DeviceLost(message, location);
#ifdef VK_KHR_swapchain
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        throw NativeWindowInUse(message);
#endif // VK_KHR_swapchain
    default:
        throw ErrorResult(result, message, location);
#else
    default:
        errorHandler(result, message, location);
#endif // !MAGMA_NO_EXCEPTIONS
    }
}
} // namespace magma::exception
