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
#ifdef MAGMA_SSE
#pragma message("Using x86 SSE")
#elif defined(MAGMA_NEON)
#pragma message("Using ARM Neon")
#else
#pragma message("Using FPU")
#endif

#include "core/pch.h"
#include "packed/color.h"
#include "packed/vector.h"

void testPack16Formats()
{
    constexpr float r = .25f, g = .5f, b = .75f;
    if (magma::packed::R4g4b4a4Unorm(r, g, b, 1.f).v != 0x48bf)
        std::cout << "R4G4B4A4_UNORM_PACK16 value is invalid!" << std::endl;
    if (magma::packed::B4g4r4a4Unorm(b, g, r, 1.f).v != 0xb84f)
        std::cout << "B4G4R4A4_UNORM_PACK16 value is invalid!" << std::endl;
    if (magma::packed::R5g6b5Unorm(r, g, b).v != 0x4417)
        std::cout << "R5G6B5_UNORM_PACK16 value is invalid!" << std::endl;
    if (magma::packed::B5g6r5Unorm(b, g, r).v != 0xbc08)
        std::cout << "B5G6R5_UNORM_PACK16 value is invalid!" << std::endl;
    if (magma::packed::R5g5b5a1Unorm(r, g, b, 1.f).v != 0x442f)
        std::cout << "R5G5B5A1_UNORM_PACK16 value is invalid!" << std::endl;
    if (magma::packed::B5g5r5a1Unorm(b, g, r, 1.f).v != 0xbc11)
        std::cout << "B5G5R5A1_UNORM_PACK16 value is invalid!" << std::endl;
}

void testPack32Formats()
{
    constexpr float x = .25f, y = .5f, z = .75f;
    if (magma::packed::X10y10z10w2Unorm(x, y, z, 1).v != 0x6ff80100)
        std::cout << "A2B10G10R10_UNORM_PACK32 value is invalid!" << std::endl;
    if (magma::packed::X10y10z10w2Snorm(-x, -y, -z, 1).v != 0x681c0380)
        std::cout << "A2B10G10R10_SNORM_PACK32 value is invalid!" << std::endl;
    if (magma::packed::X11y11z10Ufloat(x, y, z).v != 0x741c0340)
        std::cout << "B10G11R11_UFLOAT_PACK32 value is invalid!" << std::endl;
}

void testSharedExponentFormat()
{
    constexpr float r = .2f, g = .3f, b = .5f;
    float rgb[3];
    const magma::packed::R9g9b9e5Ufloat color(r, g, b);
    color.unpack(rgb);
    std::cout << "R delta: " << std::abs(rgb[0] - r) << std::endl;
    std::cout << "G delta: " << std::abs(rgb[1] - g) << std::endl;
    std::cout << "B delta: " << std::abs(rgb[2] - b) << std::endl;
    std::cout << "Binary: " << color.v << std::endl;
}

int main()
{
    std::cout << "Test PACK16 format packing" << std::endl;
    testPack16Formats();
    std::cout << "Test PACK32 format packing" << std::endl;
    testPack32Formats();
    std::cout << "Test E5B9G9R9 format packing" << std::endl;
    testSharedExponentFormat();
    return 0;
}
