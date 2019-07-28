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

namespace magma
{
    struct ClearValue
    {
        constexpr operator VkClearValue&() noexcept
            { return value; }
        constexpr operator const VkClearValue&() const noexcept
            { return value; }

    protected:
        // C++ forbids inheritance from union.
        VkClearValue value;
    };

    struct ClearColor : public ClearValue
    {
        constexpr ClearColor(float r, float g, float b, float a = 1.f) noexcept;
        constexpr ClearColor(int32_t r, int32_t g, int32_t b, int32_t a) noexcept;
        constexpr ClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept;
        constexpr ClearColor(const float color[4]) noexcept;
        constexpr ClearColor(const int32_t color[4]) noexcept;
        constexpr ClearColor(const uint32_t color[4]) noexcept;
        constexpr ClearColor(uint32_t hexValue) noexcept;
    };

    struct ClearDepthStencil : public ClearValue
    {
        constexpr ClearDepthStencil(float depth, uint8_t stencil = 0) noexcept;
    };
} // namespace magma

#include "clearValue.inl"

namespace magma
{
    namespace clears
    {   // Alpha is one
        constexpr ClearColor blackColor(0x000000U);
        constexpr ClearColor whiteColor(0xFFFFFF);
        constexpr ClearColor redColor(0xFF0000);
        constexpr ClearColor limeColor(0x00FF00);
        constexpr ClearColor blueColor(0x0000FF);
        constexpr ClearColor yellowColor(0xFFFF00);
        constexpr ClearColor cyanColor(0x00FFFF);
        constexpr ClearColor magentaColor(0xFF00FF);
        constexpr ClearColor silverColor(0xC0C0C0);
        constexpr ClearColor grayColor(0x808080);
        constexpr ClearColor maroonColor(0x800000);
        constexpr ClearColor oliveColor(0x808000);
        constexpr ClearColor greenColor(0x008000);
        constexpr ClearColor purpleColor(0x800080);
        constexpr ClearColor tealColor(0x008080);
        constexpr ClearColor navyColor(0x000080);

        constexpr ClearDepthStencil depthOne(1.f, 0);
        constexpr ClearDepthStencil depthZero(0.f, 0);
    } // namespace clears
} // namespace magma
