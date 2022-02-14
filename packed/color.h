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
        /* Specifies a four-component, 16-bit packed unsigned normalized format that has
           a 4-bit R component in bits 12..15, a 4-bit G component in bits 8..11,
           a 4-bit B component in bits 4..7, and a 4-bit A component in bits 0..3.
           Corresponding format is VK_FORMAT_R4G4B4A4_UNORM_PACK16. */

        struct R4g4b4a4Unorm
        {
            union
            {
                struct
                {
                    uint16_t a: 4; // [0,15]
                    uint16_t b: 4; // [0,15]
                    uint16_t g: 4; // [0,15]
                    uint16_t r: 4; // [0,15]
                };
                uint16_t v;
            };

            R4g4b4a4Unorm() noexcept = default;
            explicit R4g4b4a4Unorm(uint16_t v) noexcept: v(v) {}
            explicit R4g4b4a4Unorm(float r, float g, float b, float a) noexcept;
            explicit R4g4b4a4Unorm(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
            explicit R4g4b4a4Unorm(const float v[4]) noexcept;
            explicit R4g4b4a4Unorm(const uint8_t v[4]) noexcept;

            static constexpr auto format = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        };

        /* Specifies a three-component, 16-bit packed unsigned normalized format that has
           a 5-bit B component in bits 11..15, a 6-bit G component in bits 5..10,
           and a 5-bit R component in bits 0..4.
           Corresponding format is VK_FORMAT_B5G6R5_UNORM_PACK16. */

        struct R5g6b5unorm
        {
            union
            {
                struct
                {
                    uint16_t r: 5; // [0,31]
                    uint16_t g: 6; // [0,63]
                    uint16_t b: 5; // [0,31]
                };
                uint16_t v;
            };

            R5g6b5unorm() noexcept = default;
            explicit R5g6b5unorm(uint16_t v) noexcept: v(v) {}
            explicit R5g6b5unorm(float r, float g, float b) noexcept;
            explicit R5g6b5unorm(const float v[3]) noexcept:
                R5g6b5unorm(v[0], v[1], v[2]) {}

            static constexpr auto format = VK_FORMAT_B5G6R5_UNORM_PACK16;
        };

        /* Specifies a three-component, 16-bit packed unsigned normalized format that has
           a 5-bit R component in bits 11..15, a 6-bit G component in bits 5..10,
           and a 5-bit B component in bits 0..4.
           Corresponding format is VK_FORMAT_R5G6B5_UNORM_PACK16. */

        struct B5g6r5unorm
        {
            union
            {
                struct
                {
                    uint16_t b: 5; // [0,31]
                    uint16_t g: 6; // [0,63]
                    uint16_t r: 5; // [0,31]
                };
                uint16_t v;
            };

            B5g6r5unorm() noexcept = default;
            explicit B5g6r5unorm(uint16_t v) noexcept: v(v) {}
            explicit B5g6r5unorm(float b, float g, float r) noexcept;
            explicit B5g6r5unorm(const float v[3]) noexcept:
                B5g6r5unorm(v[0], v[1], v[2]) {}

            static constexpr auto format = VK_FORMAT_R5G6B5_UNORM_PACK16;
        };

        /* Specifies a four-component, 16-bit packed unsigned normalized format that has
           a 5-bit B component in bits 11..15, a 5-bit G component in bits 6..10,
           a 5-bit R component in bits 1..5, and a 1-bit A component in bit 0.
           Corresponding format is VK_FORMAT_B5G5R5A1_UNORM_PACK16. */

        struct A1r5g5b5unorm
        {
            union
            {
                struct
                {
                    uint16_t a: 1; // [0,1]
                    uint16_t r: 5; // [0,31]
                    uint16_t g: 5; // [0,31]
                    uint16_t b: 5; // [0,31]
                };
                uint16_t v;
            };

            A1r5g5b5unorm() noexcept = default;
            explicit A1r5g5b5unorm(uint16_t v) noexcept: v(v) {}
            explicit A1r5g5b5unorm(float r, float g, float b, float a) noexcept;
            explicit A1r5g5b5unorm(const float v[4]) noexcept:
                A1r5g5b5unorm(v[0], v[1], v[2], v[3]) {}

            static constexpr auto format = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        };

        /* Specifies a four-component, 16-bit packed unsigned normalized format that has
           a 5-bit R component in bits 11..15, a 5-bit G component in bits 6..10,
           a 5-bit B component in bits 1..5, and a 1-bit A component in bit 0.
           Corresponding format is VK_FORMAT_R5G5B5A1_UNORM_PACK16. */

        struct A1b5g5r5unorm
        {
            union
            {
                struct
                {
                    uint16_t a: 1; // [0,1]
                    uint16_t b: 5; // [0,31]
                    uint16_t g: 5; // [0,31]
                    uint16_t r: 5; // [0,31]
                };
                uint16_t v;
            };

            A1b5g5r5unorm() noexcept = default;
            explicit A1b5g5r5unorm(uint16_t v) noexcept: v(v) {}
            explicit A1b5g5r5unorm(float b, float g, float r, float a) noexcept;
            explicit A1b5g5r5unorm(const float v[4]) noexcept:
                A1b5g5r5unorm(v[0], v[1], v[2], v[3]) {}

            static constexpr auto format = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        };
    } // namespace packed
} // namespace magma

#include "r4g4b4a4unorm.inl"
#include "r5g6b5unorm.inl"
#include "b5g6r5unorm.inl"
#include "a1r5g5b5unorm.inl"
#include "a1b5g5r5unorm.inl"
