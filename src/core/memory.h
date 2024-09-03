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
#pragma once

namespace magma
{
    namespace core
    {
        template<class T> T *copy(const T *src) noexcept;
        template<class T> T *copy(T *dst, const T *src) noexcept;
        template<class T> T *copyArray(const T *src, std::size_t size) noexcept;
        template<class T> T *copyVector(const std::vector<T>& src) noexcept;
        template<class T1, class T2> T1 *copyVector(const std::vector<T2>& src) noexcept;
        template<class T> T *copyInitializerList(const std::initializer_list<T>& src) noexcept;
        template<class T1, class T2> T1 *copyInitializerList(const std::initializer_list<T2>& src) noexcept;
        char *copyString(const char *src) noexcept;
        char *copyString(char *dst, std::size_t size, const char *src) noexcept;
        uint8_t *copyBinaryData(const void *src, std::size_t size) noexcept;
        template<class T> uint8_t *copyBinaryData(const T& src) noexcept;
        template<class T> bool compare(const void *p1, const void *p2) noexcept;
        template<class T> bool compare(const T *p1, const T *p2) noexcept;
        template<class T> bool compareArrays(const T *p1, const T *p2, std::size_t count) noexcept;
        template<class T> void zeroMemory(T& var) noexcept;
        template<class T, std::size_t N> void zeroMemory(T (&array)[N]) noexcept;
    } // core
} // magma

#include "memory.inl"
