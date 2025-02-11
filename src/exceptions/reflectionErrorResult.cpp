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
#include "reflectionErrorResult.h"

namespace magma::exception
{
#ifndef MAGMA_NO_EXCEPTIONS
const char *ReflectionErrorResult::description() const noexcept
{
    switch (result)
    {
    case SPV_REFLECT_RESULT_SUCCESS: return "Operation successfully completed";
    case SPV_REFLECT_RESULT_NOT_READY: return "Reflection result not ready yet";
    case SPV_REFLECT_RESULT_ERROR_PARSE_FAILED: return "Failed to parse SPIR-V binary. The data may be corrupted or in an invalid format";
    case SPV_REFLECT_RESULT_ERROR_ALLOC_FAILED: return "Failed to allocates memory and initializes all bytes in the allocated storage to zero";
    case SPV_REFLECT_RESULT_ERROR_RANGE_EXCEEDED: return "Index or range exceeded valid bounds";
    case SPV_REFLECT_RESULT_ERROR_NULL_POINTER: return "Null pointer encountered where a valid object was expected";
    case SPV_REFLECT_RESULT_ERROR_INTERNAL_ERROR: return "Unexpected internal error occurred";
    case SPV_REFLECT_RESULT_ERROR_COUNT_MISMATCH: return "Expected and actual element count do not match";
    case SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND: return "The requested element was not found in the SPIR-V binary";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_CODE_SIZE: return "The size of SPIR-V binary is less than minimal size or not a multiple of SPIR-V word size";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_MAGIC_NUMBER: return "Not a SPIR-V binary, expected different magic number";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_EOF: return "Unexpected end of SPIR-V binary";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ID_REFERENCE: return "Failed to find SPIR-V node or invalid called function ID somewhere";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_SET_NUMBER_OVERFLOW: return "This error code is outdated";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_STORAGE_CLASS: return "This error code is outdated";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_RECURSION: return "The static function-call graph for an entry point must not contain cycles";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_INSTRUCTION: return "Invalid or unrecognized instruction in SPIR-V binary";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_BLOCK_DATA: return "Zero count of block variable members";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_BLOCK_MEMBER_REFERENCE: return "Failed to get member variable at the access's chain current index";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ENTRY_POINT: return "Entry point not found. Ensure the shader contains a valid main function";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_EXECUTION_MODE: return "This error code is outdated";
    case SPV_REFLECT_RESULT_ERROR_SPIRV_MAX_RECURSIVE_EXCEEDED: return "Maximum recursion depth exceeded while parsing PhysicalStorageBuffer objects";
    default: return "Unknown error code";
    }
}

#ifdef MAGMA_NO_EXCEPTIONS
static ReflectionErrorHandler errorHandler =
    [](SpvReflectResult, const char *message, const source_location&)
    {   // If no error handler is provided, abort program
        std::cerr << message << std::endl;
        abort();
    };

void setReflectionErrorHandler(ReflectionErrorHandler errorHandler_) noexcept
{
    errorHandler = std::move(errorHandler_);
}
#endif // MAGMA_NO_EXCEPTIONS

void handleReflectionResult(SpvReflectResult result, const char *message, const source_location& location)
{
    switch (result)
    {
    case SPV_REFLECT_RESULT_SUCCESS:
    case SPV_REFLECT_RESULT_NOT_READY:
        break;
    default:
    #ifdef MAGMA_NO_EXCEPTIONS
        errorHandler(result, message, location);
    #else
        throw ReflectionErrorResult(result, message, location);
    #endif // MAGMA_NO_EXCEPTIONS
    }
}
#endif // !MAGMA_NO_EXCEPTIONS
} // namespace magma::exception
