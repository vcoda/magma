/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(__FUNCSIG__,\
    magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__})
#else
#define MAGMA_THROW_NOT_IMPLEMENTED throw magma::exception::NotImplemented(__PRETTY_FUNCTION__,\
    magma::exception::source_location{__FILE__, __LINE__, __FUNCTION__})
#endif
