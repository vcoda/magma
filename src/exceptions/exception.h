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
#include "sourcelocation.h"

namespace magma
{
    namespace exception
    {
        /* Base exception class. Provides (optional) information as
           file name and line number where it was thrown. */

    #ifndef MAGMA_NO_EXCEPTIONS
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
    #endif // !MAGMA_NO_EXCEPTIONS

        /* If C++ exceptions are not enabled, application has an
           option to provide custom exception handler which will be
           called when library tries to throw an exception. */

    #ifdef MAGMA_NO_EXCEPTIONS
        typedef std::function<void(const char *, const source_location&)> ExceptionHandler;
        void setExceptionHandler(ExceptionHandler exceptionHandler) noexcept;
    #endif // MAGMA_NO_EXCEPTIONS

        void handleException(const char *message, const source_location& location);
    } // namespace exception
} // namespace magma

#define MAGMA_ERROR(message) magma::exception::handleException(message, MAGMA_SOURCE_LOCATION)
