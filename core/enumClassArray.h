/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
        /* Template of array that can be indexed with enum class. */

        template<class Type, class Enum>
        class EnumClassArray
        {
        public:
            constexpr uint32_t size() const noexcept { return static_cast<uint32_t>(Enum::Max); }
            Type& operator[](uint32_t i) noexcept { return a[i]; }
            const Type& operator[](uint32_t i) const noexcept { return a[i]; }
            Type& operator[](Enum i) noexcept { return a[static_cast<uint32_t>(i)]; }
            const Type& operator[](Enum i) const noexcept { return a[static_cast<uint32_t>(i)]; }

        private:
            Type a[static_cast<int>(Enum::Max) + 1];
        };
    } // namespace core
} // namespace magma
