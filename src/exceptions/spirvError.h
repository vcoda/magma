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
#include "errorHandler.h"
#ifndef MAGMA_NO_EXCEPTIONS
#include "exception.h"

namespace magma::exception
{
    /* Error result of SPIRV library. */

    class SpirvError : public Exception
    {
    public:
        explicit SpirvError(spv_result_t error,
            const char *message,
            const source_location& location) noexcept:
            Exception(message, location), error(error) {}
        spv_result_t result() const noexcept { return error; }

    private:
        const spv_result_t error;
    };

} // namespace magma::exception
#endif // !MAGMA_NO_EXCEPTIONS
