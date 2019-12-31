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
#pragma once

namespace magma
{
    class Exception : public std::runtime_error
    {
    public:
        Exception(const char *const message, const char *file = nullptr, int line = -1);
        Exception(const std::string& message, const char *file = nullptr, int line = -1);
        const char *file() const noexcept { return fl; }
        int line() const noexcept { return ln; }

    private:
        const char *fl;
        int ln;
    };

    class BadResult : public Exception
    {
    public:
        BadResult(const VkResult result, const char *const message, const char *file, int line);
        BadResult(const VkResult result, const std::string& message, const char *file, int line);
        VkResult getResult() const noexcept { return result; }

    private:
        VkResult result;
    };

    class PresentException : public BadResult
    {
    public:
        PresentException(const VkResult result, const char *const message, const char *file, int line):
            BadResult(result, message, file, line) {}
    };

    class ExtensionNotPresent: public Exception
    {
    public:
        ExtensionNotPresent(const char *const extension, const char *file, int line):
            Exception(extension, file, line) {}
    };

    class NotImplemented : public Exception
    {
    public:
        NotImplemented(const char *const function, const char *file, int line):
            Exception(function, file, line) {}
    };
} // namespace magma

#define MAGMA_THROW(message) throw Exception(message, __FILE__, __LINE__)
#define MAGMA_THROW_FAILURE(result, message)\
    if (!MAGMA_SUCCEEDED(result))\
        throw BadResult(result, message, __FILE__, __LINE__)
#define MAGMA_THROW_NOT_PRESENT(extension) throw ExtensionNotPresent(extension, __FILE__, __LINE__)
#ifdef _MSC_VER
#   define MAGMA_THROW_NOT_IMPLEMENTED throw NotImplemented(__FUNCSIG__, __FILE__, __LINE__)
#else
#   define MAGMA_THROW_NOT_IMPLEMENTED throw NotImplemented(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#endif
