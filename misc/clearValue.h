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
    // C++ forbids inheritance from union
    struct ClearValue
    {
        operator const VkClearValue&() const noexcept
            { return value; }

    protected:
        VkClearValue value;
    };  

    struct ColorClear : public ClearValue 
    {
        ColorClear(float r, float g, float b, float a = 1.f) noexcept;
        ColorClear(int32_t r, int32_t g, int32_t b, int32_t a) noexcept;
        ColorClear(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept;
        ColorClear(const float color[4]) noexcept;
        ColorClear(const int32_t color[4]) noexcept;
        ColorClear(const uint32_t color[4]) noexcept;
        ColorClear(uint32_t hexValue) noexcept;
    };

    struct DepthStencilClear : public ClearValue
    {
        DepthStencilClear(float depth, uint8_t stencil = 0) noexcept;
    };

    namespace clears
    {   
        // Alpha is one
        extern const ColorClear blackColorClear;
        extern const ColorClear whiteColorClear;
        extern const ColorClear redColorClear;
        extern const ColorClear limeColorClear;
        extern const ColorClear blueColorClear;
        extern const ColorClear yellowColorClear;
        extern const ColorClear cyanColorClear;
        extern const ColorClear magentaColorClear;
        extern const ColorClear silverColorClear;
        extern const ColorClear grayColorClear;
        extern const ColorClear maroonColorClear;
        extern const ColorClear oliveColorClear;
        extern const ColorClear greenColorClear;
        extern const ColorClear purpleColorClear;
        extern const ColorClear tealColorClear;
        extern const ColorClear navyColorClear;

        // Stencil is zero
        extern const DepthStencilClear oneDepthClear;
        extern const DepthStencilClear zeroDepthClear;
    } // namespace clears
} // namespace magma

#include "clearValue.inl"
