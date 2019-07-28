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
#include "pch.h"
#pragma hdrstop
#include "exception.h"

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

ExtensionNotPresent::ExtensionNotPresent(const char *const extension,
    const char *file, int line):
    Exception(extension, file, line)
{}

NotImplemented::NotImplemented(const char *const function,
    const char *file, int line):
    Exception(function, file, line)
{}
} // namespace magma
