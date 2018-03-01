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
#pragma once
#include <stdexcept>
#include <string>
#include "../vulkan.h"

namespace magma
{
	class Exception : public std::runtime_error
    {
    public:
        Exception(const char *const message,
            const char *file = nullptr, int line = -1);
        Exception(const std::string& message,
            const char *file = nullptr, int line = -1);
        const char *file() const { return fl; }
        int line() const { return ln; }

    private:
        const char *fl;
        int ln;
    };

    class BadResultException : public Exception
    {
    public:
        BadResultException(const VkResult result, const char *const message,
            const char *file, int line);
        BadResultException(const VkResult result, const std::string& message,
            const char *file, int line);
        VkResult code() const { return result;  }
        const char *codeString() const;

    private:
        VkResult result;
    };

    class ExtensionNotPresentException : public Exception
    {
    public:
        ExtensionNotPresentException(const char *const extension,
            const char *file, int line);
    };

    class NotImplementedException : public Exception
    {
    public:
        NotImplementedException(const char *const function,
            const char *file, int line);
    };
} // namespace magma
