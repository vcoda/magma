/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
namespace exception
{
Exception::Exception() noexcept:
    builtin(nullptr),
    file_(__FILE__),
    line_(__LINE__)
{}

Exception::Exception(const char *message) noexcept:
    builtin(message),
    file_(__FILE__),
    line_(__LINE__)
{}

Exception::Exception(const std::string message) noexcept:
    builtin(nullptr),
    message(std::move(message)),
    file_(__FILE__),
    line_(__LINE__)
{}

Exception::Exception(const char *message, const char *file, long line) noexcept:
    builtin(message),
    file_(file),
    line_(line)
{}

Exception::Exception(const std::string message, const char *file, long line) noexcept:
    builtin(nullptr),
    message(std::move(message)),
    file_(file),
    line_(line)
{}

Exception::Exception(const Exception& other) noexcept:
    builtin(other.builtin),
    file_(other.file_),
    line_(other.line_)
{
    if (!other.message.empty())
    {   // Try copy
        try {
            message = other.message;
        } catch (...) {
        }
    }
}

Exception& Exception::operator=(const Exception& other) noexcept
{
    if (this != &other)
    {
        builtin = other.builtin;
        if (!other.message.empty())
        {   // Try copy
            try {
                message = other.message;
            } catch (...) {
            }
        }
        file_ = other.file_;
        line_ = other.line_;
    }
    return *this;
}

const char* Exception::what() const noexcept
{
    if (builtin)
        return builtin;
    if (!message.empty())
        return message.c_str();
    return "unknown";
}

ErrorResult::ErrorResult(VkResult result, const char *message) noexcept:
    Exception(message),
    result(result)
{}

ErrorResult::ErrorResult(VkResult result, const std::string message) noexcept:
    Exception(std::move(message)),
    result(result)
{}

ErrorResult::ErrorResult(VkResult result, const char *message, const char *file, long line) noexcept:
    Exception(message, file, line),
    result(result)
{}

ErrorResult::ErrorResult(VkResult result, const std::string message, const char *file, long line) noexcept:
    Exception(std::move(message), file, line),
    result(result)
{}
} // namespace exception
} // namespace magma
