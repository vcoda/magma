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
#ifndef MAGMA_NO_EXCEPTIONS
#include "sourcelocation.h"

namespace magma::exception
{
    /* Base exception class. Provides optional information
       such as an error message, file name, and line number
       where the exception was thrown. Meant to work with
       string literals only, to prevent memory allocations
       while handling exceptions. */

    class Exception : public std::exception
    {
    public:
        explicit Exception(const char *message = "unknown exception") noexcept;
        explicit Exception(const char *message,
            const source_location& location) noexcept;
        const char *what() const noexcept override { return message; }
        const source_location& where() const noexcept { return location; }

    private:
        const char *message;
        source_location location;
    };
} // namespace magma::exception

#include "exception.inl"
#endif // !MAGMA_NO_EXCEPTIONS
