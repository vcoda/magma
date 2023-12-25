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
#pragma once
#ifndef MAGMA_NO_EXCEPTIONS
#include "exception.h"

namespace magma
{
    namespace exception
    {
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

#ifdef _MSC_VER
#define MAGMA_FUNCTION_SIGNATURE __FUNCSIG__
#else
#define MAGMA_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(MAGMA_FUNCTION_SIGNATURE, MAGMA_SOURCE_LOCATION)

#else

#define MAGMA_THROW_NOT_IMPLEMENTED

#endif // !MAGMA_NO_EXCEPTIONS
