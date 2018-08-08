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
#include "../vulkan.h"
#include "../shared.h"

namespace magma
{
    struct ClearValue
    {
        operator VkClearValue&() noexcept
            { return value; }
        operator const VkClearValue&() const noexcept
            { return value; }

    protected:
        // C++ forbids inheritance from union
        VkClearValue value;
    };

    struct ClearColor : public ClearValue
    {
        ClearColor(float r, float g, float b, float a = 1.f) noexcept;
        ClearColor(int32_t r, int32_t g, int32_t b, int32_t a) noexcept;
        ClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept;
        ClearColor(const float color[4]) noexcept;
        ClearColor(const int32_t color[4]) noexcept;
        ClearColor(const uint32_t color[4]) noexcept;
        ClearColor(uint32_t hexValue) noexcept;
    };

    struct ClearDepthStencil : public ClearValue
    {
        ClearDepthStencil(float depth, uint8_t stencil = 0) noexcept;
    };

    namespace clears
    {   // Alpha is one
        extern const ClearColor blackColor;
        extern const ClearColor whiteColor;
        extern const ClearColor redColor;
        extern const ClearColor limeColor;
        extern const ClearColor blueColor;
        extern const ClearColor yellowColor;
        extern const ClearColor cyanColor;
        extern const ClearColor magentaColor;
        extern const ClearColor silverColor;
        extern const ClearColor grayColor;
        extern const ClearColor maroonColor;
        extern const ClearColor oliveColor;
        extern const ClearColor greenColor;
        extern const ClearColor purpleColor;
        extern const ClearColor tealColor;
        extern const ClearColor navyColor;

        extern const ClearDepthStencil depthOne;
        extern const ClearDepthStencil depthZero;
    } // namespace clears
} // namespace magma

#include "clearValue.inl"
