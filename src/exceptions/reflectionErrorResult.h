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
#include "exception.h"
#include "errorHandler.h"

namespace magma
{
    namespace exception
    {
        /* Error result of SPIRV-Reflect library. */

    #ifndef MAGMA_NO_EXCEPTIONS
        class ReflectionErrorResult : public Exception
        {
        public:
            explicit ReflectionErrorResult(SpvReflectResult result,
                const char *message,
                const source_location& location) noexcept:
                Exception(message, location), result(result) {}
            SpvReflectResult error() const noexcept { return result; }
            const char *description() const noexcept;

        private:
            const SpvReflectResult result;
        };
    #endif // !MAGMA_NO_EXCEPTIONS
    } // namespace exception
} // namespace magma
