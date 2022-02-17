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
#ifdef _MSC_VER
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#endif

namespace magma
{
    namespace packed
    {
        /* IEEE 754 single-precision binary floating-point format. */

        union Float32
        {
            float f;
            uint32_t i;
            struct
            {   // Little-endian byte order
                uint32_t m: 23;
                uint32_t e: 8;
                uint32_t sign: 1;
            };
        };
    } // namespace packed
} // namespace magma
