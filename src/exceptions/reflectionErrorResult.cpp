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
#include "pch.h"
#pragma hdrstop
#include "reflectionErrorResult.h"

namespace magma
{
namespace exception
{
#ifdef MAGMA_NO_EXCEPTIONS
static ReflectionErrorHandler errorHandler =
    [](SpvReflectResult, const char *message, const source_location&)
    {   // If no error handler is provided, abort program
        std::cerr << message << std::endl;
        abort();
    };

void setReflectionErrorHandler(ReflectionErrorHandler errorHandler_) noexcept
{
    errorHandler = std::move(errorHandler_);
}
#endif // MAGMA_NO_EXCEPTIONS

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
} // namespace exception
} // namespace magma
