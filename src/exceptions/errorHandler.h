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
#pragma once
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"
#include "sourcelocation.h"

namespace magma::exception
{
    /* If C++ exceptions are not enabled, application has an
       option to provide custom error handlers which will be
       called when library throws an exception or Vulkan/SpvReflect
       error is encountered. */

#ifdef MAGMA_NO_EXCEPTIONS
    typedef std::function<void(const char *, const source_location&)> ExceptionHandler;
    typedef std::function<void(VkResult, const char *, const source_location&)> ErrorHandler;
    typedef std::function<void(SpvReflectResult, const char *, const source_location&)> ReflectionErrorHandler;

    void setExceptionHandler(ExceptionHandler handler) noexcept;
    void setErrorHandler(ErrorHandler handler) noexcept;
    void setReflectionErrorHandler(ReflectionErrorHandler handler) noexcept;
#endif // MAGMA_NO_EXCEPTIONS

    void handleException(std::string_view message,
        const source_location& location);
    void handleResult(VkResult result,
        const char *message,
        const source_location& location);
    void handleReflectionResult(SpvReflectResult result,
        const char *message,
        const source_location& location);
} // namespace magma::exception

#define MAGMA_ERROR(message) magma::exception::handleException(message, MAGMA_SOURCE_LOCATION)

#define MAGMA_HANDLE_RESULT(result, message) magma::exception::handleResult(result, message, MAGMA_SOURCE_LOCATION)

#define MAGMA_HANDLE_REFLECTION_RESULT(result, message) magma::exception::handleReflectionResult(result, message, MAGMA_SOURCE_LOCATION)
