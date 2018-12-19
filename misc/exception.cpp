/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "exception.h"
#include "../internal/shared.h"

namespace magma
{
Exception::Exception(const char *const message,
    const char *file /* nullptr */, int line /* -1 */):
    std::runtime_error(message),
    fl(file),
    ln(line)
{}

Exception::Exception(const std::string& message,
    const char *file /* nullptr */, int line /* -1 */):
    std::runtime_error(message.c_str()),
    fl(file),
    ln(line)
{}

BadResult::BadResult(const VkResult result,
    const char *const message,
    const char *file, int line):
    Exception(message, file, line),
    result(result)
{}

BadResult::BadResult(const VkResult result,
    const std::string& message,
    const char *file, int line):
    Exception(message, file, line),
    result(result)
{}

const char *BadResult::codeString() const noexcept
{
    switch (code())
    {
    MAGMA_STRINGIZE_FIELD(VK_SUCCESS);
    MAGMA_STRINGIZE_FIELD(VK_NOT_READY);
    MAGMA_STRINGIZE_FIELD(VK_TIMEOUT);
    MAGMA_STRINGIZE_FIELD(VK_EVENT_SET);
    MAGMA_STRINGIZE_FIELD(VK_EVENT_RESET);
    MAGMA_STRINGIZE_FIELD(VK_INCOMPLETE);

    MAGMA_STRINGIZE_FIELD(VK_ERROR_OUT_OF_HOST_MEMORY);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_OUT_OF_DEVICE_MEMORY);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_INITIALIZATION_FAILED);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_DEVICE_LOST);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_MEMORY_MAP_FAILED);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_LAYER_NOT_PRESENT);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_EXTENSION_NOT_PRESENT);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_FEATURE_NOT_PRESENT);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_INCOMPATIBLE_DRIVER);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_TOO_MANY_OBJECTS);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_FORMAT_NOT_SUPPORTED);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_FRAGMENTED_POOL);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_OUT_OF_POOL_MEMORY);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_INVALID_EXTERNAL_HANDLE);

    MAGMA_STRINGIZE_FIELD(VK_ERROR_SURFACE_LOST_KHR);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
    MAGMA_STRINGIZE_FIELD(VK_SUBOPTIMAL_KHR);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_OUT_OF_DATE_KHR);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_VALIDATION_FAILED_EXT);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_INVALID_SHADER_NV);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_FRAGMENTATION_EXT);
    MAGMA_STRINGIZE_FIELD(VK_ERROR_NOT_PERMITTED_EXT);

    default:
        return "<unknown>";
    }
}

ExtensionNotPresent::ExtensionNotPresent(const char *const extension,
    const char *file, int line):
    Exception(extension, file, line)
{}

NotImplemented::NotImplemented(const char *const function,
    const char *file, int line):
    Exception(function, file, line)
{}
} // namespace magma
