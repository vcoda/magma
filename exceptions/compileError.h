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
#pragma once
#include <shaderc/shaderc.h>
#include "exception.h"

namespace magma
{
    namespace exception
    {
        /* aux::ShaderCompiler exception. */

        class CompileError : public Exception
        {
        public:
            explicit CompileError(shaderc_compilation_result_t result,
                const source_location& location):
                Exception(shaderc_result_get_error_message(result), location),
                status_(shaderc_result_get_compilation_status(result)),
                warnings_(shaderc_result_get_num_warnings(result)),
                errors_(shaderc_result_get_num_errors(result))
            {
                shaderc_result_release(result);
            }

            shaderc_compilation_status status() const noexcept { return status_; }
            std::size_t warnings() const noexcept { return warnings_; }
            std::size_t errors() const noexcept { return errors_; }

        private:
            shaderc_compilation_status status_;
            std::size_t warnings_;
            std::size_t errors_;
        };
    } // namespace exception
} // namespace magma

