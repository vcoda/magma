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
#include <vector>

namespace magma
{
    namespace helpers
    {
        template<typename Type>
        Type *copy(const Type *const src);
        template<typename Type>
        Type *copy(Type *const dst, const Type *const src);
        template<typename Type>
        Type *copyArray(const Type *const src, size_t count);
        template<typename Type>
        Type *copyVector(const std::vector<Type>& src);
        template<typename Type>
        Type *copyInitializer(const std::initializer_list<Type>& src);
        char *copyString(const char *const src);
        template<typename Type>
        bool compareArrays(const Type *const src, const Type *const dst, size_t count) noexcept;
    } // namespace helpers
} // namespace magma

#include "copy.inl"
