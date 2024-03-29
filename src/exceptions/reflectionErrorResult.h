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
#include "exception.h"
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

namespace magma
{
    namespace exception
    {
        /* Error result of SPIRV-Reflect library. */

    #ifndef MAGMA_NO_EXCEPTIONS
        class ReflectionErrorResult : public Exception
        {
        public:
            ReflectionErrorResult(SpvReflectResult result,
                const char *message,
                const source_location& location) noexcept:
                Exception(message, location), result(result) {}
            SpvReflectResult error() const noexcept { return result; }

        private:
            const SpvReflectResult result;
        };
    #endif // !MAGMA_NO_EXCEPTIONS

        /* If C++ exceptions are not enabled, application has an
           option to provide custom error handler which will be
           called when the SpvReflect error is encountered. */

    #ifdef MAGMA_NO_EXCEPTIONS
        typedef std::function<void(SpvReflectResult, const char *, const source_location&)> ReflectionErrorHandler;
        void setReflectionErrorHandler(ReflectionErrorHandler errorHandler) noexcept;
    #endif // MAGMA_NO_EXCEPTIONS

        void handleReflectionResult(SpvReflectResult result,
            const char *message,
            const source_location& location);
    } // namespace exception
} // namespace magma

#define MAGMA_HANDLE_REFLECTION_RESULT(result, message) magma::exception::handleReflectionResult(result, message, MAGMA_SOURCE_LOCATION)
