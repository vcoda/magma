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
        template<class Int>
        struct ConstexprModf
        {
            Int integer;
            Int fractional;

            template<class Float>
            constexpr ConstexprModf(const Float x) noexcept
            {
                integer = static_cast<Int>(x);
                Float ax = x >= 0 ? x : -x;
                Float frac = ax - static_cast<Int>(ax);
                fractional = frac * static_cast<Float>(1000000.0); // shift left
            }
        };
    } // namespace core
} // namespace magma
