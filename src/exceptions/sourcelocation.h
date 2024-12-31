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

namespace magma
{
    namespace exception
    {
        /* The source_location class represents certain information
           about the source code, such as file names, line numbers,
           and function names. Previously, functions that desire to
           obtain this information about the call site (for logging,
           testing, or debugging purposes) must use macros so that
           predefined macros like __LINE__ and __FILE__ are expanded
           in the context of the caller. The source_location class
           provides a better alternative. */

        struct source_location
        {
            /* constexpr */ std::uint_least32_t line() const noexcept { return static_cast<std::uint_least32_t>(ln); }
            /* constexpr */ std::uint_least32_t column() const noexcept { return 0; }
            /* constexpr */ const char* file_name() const noexcept { return file; }
            /* constexpr */ const char* function_name() const noexcept { return function; }

            const char *file = nullptr;
            long ln = 0;
            const char *function = nullptr;
        };
    } // namespace exception
} // namespace magma

#define MAGMA_SOURCE_LOCATION magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__}
