/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#ifndef MAGMA_NO_EXCEPTIONS
#include "spirvError.h"

namespace magma::exception
{
const char *SpirvError::description() const noexcept
{
    switch (error)
    {
    case SPV_SUCCESS: return "Operation successfully completed";
    case SPV_UNSUPPORTED: return "The requested operation or feature is not supported by the current SPIR-V implementation";
    case SPV_END_OF_STREAM: return "The end of the input stream has been reached";
    case SPV_WARNING: return "A problematic condition was detected while processing SPIR-V data";
    case SPV_FAILED_MATCH: return "The input does not match the expected SPIR-V instruction or operand pattern";
    case SPV_REQUESTED_TERMINATION: return "SPIR-V processing was terminated early at the request of the caller";
    case SPV_ERROR_INTERNAL: return "The operation could not be completed due to an unexpected condition in the SPIR-V implementation";
    case SPV_ERROR_OUT_OF_MEMORY: return "The SPIR-V library failed to allocate the memory required to complete the operation";
    case SPV_ERROR_INVALID_POINTER: return "An invalid pointer was supplied to the SPIR-V processing library";
    case SPV_ERROR_INVALID_BINARY: return "The SPIR-V binary module is malformed or cannot be decoded correctly";
    case SPV_ERROR_INVALID_TEXT: return "The SPIR-V assembly text is invalid and cannot be assembled";
    case SPV_ERROR_INVALID_TABLE: return "The SPIR-V library could not use the table to interpret the requested data";
    case SPV_ERROR_INVALID_VALUE: return "An invalid value was encountered during SPIR-V processing";
    case SPV_ERROR_INVALID_DIAGNOSTIC: return "Invalid diagnostic information was supplied to the SPIR-V library";
    case SPV_ERROR_INVALID_LOOKUP: return "A required SPIR-V instruction, operand, or enumerated value could not be found in the lookup table";
    case SPV_ERROR_INVALID_ID: return "The SPIR-V module contains an invalid identifier or violates identifier usage rules";
    case SPV_ERROR_INVALID_CFG: return "The SPIR-V module contains an invalid control flow graph";
    case SPV_ERROR_INVALID_LAYOUT: return "The SPIR-V module contains instructions or declarations in an invalid order";
    case SPV_ERROR_INVALID_CAPABILITY: return "The SPIR-V module violates capability requirements";
    case SPV_ERROR_INVALID_DATA: return "The SPIR-V module failed semantic validation";
    case SPV_ERROR_MISSING_EXTENSION: return "The SPIR-V module uses missing Vulkan/GLSL extension";
    case SPV_ERROR_WRONG_VERSION: return "The SPIR-V module uses a version that is incompatible with the selected target environment";
    default: return "An unknown SPIR-V processing error has occurred";
    }
}
} // namespace magma::exception

#endif // !MAGMA_NO_EXCEPTIONS
