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
#include "pch.h"
#pragma hdrstop
#ifndef MAGMA_NO_EXCEPTIONS
#include "spirvError.h"

namespace magma::exception
{
const char *SpirvError::description() const noexcept
{   // TODO: Return meaningful description for each error code
    switch (error)
    {
    case SPV_SUCCESS: return "Operation successfully completed";
    case SPV_UNSUPPORTED: return "";
    case SPV_END_OF_STREAM: return "Unexpected end of token stream";
    case SPV_WARNING: return "Success, but with warning";
    case SPV_FAILED_MATCH: return "";
    case SPV_REQUESTED_TERMINATION: return "Success, but signals early termination";
    case SPV_ERROR_INTERNAL: return "Error due to SPIRV-Tools internals";
    case SPV_ERROR_OUT_OF_MEMORY: return "A host memory allocation has failed";
    case SPV_ERROR_INVALID_POINTER: return "";
    case SPV_ERROR_INVALID_BINARY: return "";
    case SPV_ERROR_INVALID_TEXT: return "";
    case SPV_ERROR_INVALID_TABLE: return "";
    case SPV_ERROR_INVALID_VALUE: return "";
    case SPV_ERROR_INVALID_DIAGNOSTIC: return "";
    case SPV_ERROR_INVALID_LOOKUP: return "";
    case SPV_ERROR_INVALID_ID: return "";
    case SPV_ERROR_INVALID_CFG: return "";
    case SPV_ERROR_INVALID_LAYOUT: return "";
    case SPV_ERROR_INVALID_CAPABILITY: return "";
    case SPV_ERROR_INVALID_DATA: return "Indicates data rules validation failure";
    case SPV_ERROR_MISSING_EXTENSION: return "Missing Vulkan/GLSL extension";
    case SPV_ERROR_WRONG_VERSION: return "Indicates wrong SPIR-V version";
    default: return "Unknown error code";
    }
}
} // namespace magma::exception
#endif // !MAGMA_NO_EXCEPTIONS
