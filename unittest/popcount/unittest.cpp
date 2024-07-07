/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "core/pch.h"
#include "core/popcount.h"

int main()
{
    constexpr uint32_t a = 0b00000000000000000000000000000000;
    constexpr uint32_t b = 0b01010101010101010101010101010101;
    constexpr uint32_t c = 0b11111111111111111111111111111111;
    constexpr uint64_t d = 0b0000000000000000000000000000000000000000000000000000000000000000;
    constexpr uint64_t e = 0b0101010101010101010101010101010101010101010101010101010101010101;
    constexpr uint64_t f = 0b1111111111111111111111111111111111111111111111111111111111111111;
    std::cout << "Bit count: " << magma::core::popcount(a) << std::endl;
    std::cout << "Bit count: " << magma::core::popcount(b) << std::endl;
    std::cout << "Bit count: " << magma::core::popcount(c) << std::endl;
    std::cout << "Bit count: " << magma::core::popcount(d) << std::endl;
    std::cout << "Bit count: " << magma::core::popcount(e) << std::endl;
    std::cout << "Bit count: " << magma::core::popcount(f) << std::endl;
    return 0;
}
