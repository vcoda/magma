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
#include "pch.h"
#pragma hdrstop
#include "exception.h"

namespace magma
{
namespace exception
{
#ifdef MAGMA_NO_EXCEPTIONS
static ExceptionHandler exceptionHandler =
    [](const char *message, const source_location&)
    {   // If no exception handler is provided, simply call abort()
        std::cerr << message << std::endl;
        abort();
    };

void setExceptionHandler(ExceptionHandler exceptionHandler_) noexcept
{
    exceptionHandler = std::move(exceptionHandler_);
}
#endif // MAGMA_NO_EXCEPTIONS

#ifndef MAGMA_NO_EXCEPTIONS
Exception::Exception() noexcept:
    location_{}
{}

Exception::Exception(const char *message) noexcept:
    location_{}
{   // Alloc string
    try {
        this->message = message;
    } catch (...) {
    }
}

Exception::Exception(std::string message) noexcept:
    message(std::move(message)),
    location_{}
{}

Exception::Exception(const char *message, const source_location& location) noexcept:
    location_(location)
{   // Alloc string
    try {
        this->message = message;
    } catch (...) {
    }
}

Exception::Exception(std::string message, const source_location& location) noexcept:
    message(std::move(message)),
    location_(location)
{}

Exception::Exception(const Exception& other) noexcept:
    location_(other.location_)
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
        if (!other.message.empty())
        {   // Try copy
            try {
                message = other.message;
            } catch (...) {
            }
        }
        location_ = other.location_;
    }
    return *this;
}

const char* Exception::what() const noexcept
{
    if (!message.empty())
        return message.c_str();
    return "unknown";
}
#endif // !MAGMA_NO_EXCEPTIONS

void handleException(const char *message, const source_location& location)
{
#ifndef MAGMA_NO_EXCEPTIONS
    throw magma::exception::Exception(message, location);
#else
    exceptionHandler(message, location);
#endif
}
} // namespace exception
} // namespace magma
