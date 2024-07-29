/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
        constexpr ClearValue() noexcept;
        constexpr operator VkClearValue&() noexcept { return value; }
        constexpr operator const VkClearValue&() const noexcept { return value; }

    protected:
        // C++ forbids inheritance from union
        VkClearValue value;
    };

    /* Specifies the color image clear values to use
       when clearing a color image or attachment. */

    struct ClearColor : ClearValue
    {
        constexpr explicit ClearColor(float r, float g, float b, float a = 1.f) noexcept;
        constexpr explicit ClearColor(int32_t r, int32_t g, int32_t b, int32_t a) noexcept;
        constexpr explicit ClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t a) noexcept;
        constexpr explicit ClearColor(const float color[4]) noexcept;
        constexpr explicit ClearColor(const int32_t color[4]) noexcept;
        constexpr explicit ClearColor(const uint32_t color[4]) noexcept;
        constexpr explicit ClearColor(uint32_t hexValue) noexcept;
    };

    /* Specifies the depth clear values to use when clearing
       a depth image or attachment. */

    struct ClearDepth : ClearValue
    {
        constexpr explicit ClearDepth(float depth) noexcept;
    };

    /* Specifies the depth and stencil clear values to use
       when clearing a depth/stencil image or attachment. */

    struct ClearDepthStencil : ClearValue
    {
        explicit ClearDepthStencil(float depth, uint32_t stencil) noexcept;
    };
} // namespace magma

#include "clearValue.inl"

namespace magma
{
    namespace clear
    {
        constexpr ClearColor maroon(0x800000);
        constexpr ClearColor darkRed(0x8B0000);
        constexpr ClearColor brown(0xA52A2A);
        constexpr ClearColor firebrick(0xB22222);
        constexpr ClearColor crimson(0xDC143C);
        constexpr ClearColor red(0xFF0000);
        constexpr ClearColor tomato(0xFF6347);
        constexpr ClearColor coral(0xFF7F50);
        constexpr ClearColor indianRed(0xCD5C5C);
        constexpr ClearColor lightCoral(0xF08080);
        constexpr ClearColor darkSalmon(0xE9967A);
        constexpr ClearColor salmon(0xFA8072);
        constexpr ClearColor lightSalmon(0xFFA07A);
        constexpr ClearColor orangeRed(0xFF4500);
        constexpr ClearColor darkOrange(0xFF8C00);
        constexpr ClearColor orange(0xFFA500);
        constexpr ClearColor gold(0xFFD700);
        constexpr ClearColor darkGoldenRod(0xB8860B);
        constexpr ClearColor goldenRod(0xDAA520);
        constexpr ClearColor paleGoldenRod(0xEEE8AA);
        constexpr ClearColor darkKhaki(0xBDB76B);
        constexpr ClearColor khaki(0xF0E68C);
        constexpr ClearColor olive(0x808000);
        constexpr ClearColor yellow(0xFFFF00);
        constexpr ClearColor yellowGreen(0x9ACD32);
        constexpr ClearColor darkOliveGreen(0x556B2F);
        constexpr ClearColor oliveDrab(0x6B8E23);
        constexpr ClearColor lawnGreen(0x7CFC00);
        constexpr ClearColor chartReuse(0x7FFF00);
        constexpr ClearColor greenYellow(0xADFF2F);
        constexpr ClearColor darkGreen(0x006400);
        constexpr ClearColor green(0x008000);
        constexpr ClearColor forestGreen(0x228B22);
        constexpr ClearColor lime(0x00FF00);
        constexpr ClearColor limeGreen(0x32CD32);
        constexpr ClearColor lightGreen(0x90EE90);
        constexpr ClearColor paleGreen(0x98FB98);
        constexpr ClearColor darkSeaGreen(0x8FBC8F);
        constexpr ClearColor mediumSpringGreen(0x00FA9A);
        constexpr ClearColor springGreen(0x00FF7F);
        constexpr ClearColor seaGreen(0x2E8B57);
        constexpr ClearColor mediumAquaMarine(0x66CDAA);
        constexpr ClearColor mediumSeaGreen(0x3CB371);
        constexpr ClearColor lightSeaGreen(0x20B2AA);
        constexpr ClearColor darkSlateGray(0x2F4F4F);
        constexpr ClearColor teal(0x008080);
        constexpr ClearColor darkCyan(0x008B8B);
        constexpr ClearColor aqua(0x00FFFF);
        constexpr ClearColor cyan(0x00FFFF);
        constexpr ClearColor lightCyan(0xE0FFFF);
        constexpr ClearColor darkTurquoise(0x00CED1);
        constexpr ClearColor turquoise(0x40E0D0);
        constexpr ClearColor mediumTurquoise(0x48D1CC);
        constexpr ClearColor paleTurquoise(0xAFEEEE);
        constexpr ClearColor aquaMarine(0x7FFFD4);
        constexpr ClearColor powderBlue(0xB0E0E6);
        constexpr ClearColor cadetBlue(0x5F9EA0);
        constexpr ClearColor steelBlue(0x4682B4);
        constexpr ClearColor cornFlowerBlue(0x6495ED);
        constexpr ClearColor deepSkyBlue(0x00BFFF);
        constexpr ClearColor dodgerBlue(0x1E90FF);
        constexpr ClearColor lightBlue(0xADD8E6);
        constexpr ClearColor skyBlue(0x87CEEB);
        constexpr ClearColor lightSkyBlue(0x87CEFA);
        constexpr ClearColor midnightBlue(0x191970);
        constexpr ClearColor navy(0x000080);
        constexpr ClearColor darkBlue(0x00008B);
        constexpr ClearColor mediumBlue(0x0000CD);
        constexpr ClearColor blue(0x0000FF);
        constexpr ClearColor royalBlue(0x4169E1);
        constexpr ClearColor blueViolet(0x8A2BE2);
        constexpr ClearColor indigo(0x4B0082);
        constexpr ClearColor darkSlateBlue(0x483D8B);
        constexpr ClearColor slateBlue(0x6A5ACD);
        constexpr ClearColor mediumSlateBlue(0x7B68EE);
        constexpr ClearColor mediumPurple(0x9370DB);
        constexpr ClearColor darkMagenta(0x8B008B);
        constexpr ClearColor darkViolet(0x9400D3);
        constexpr ClearColor darkOrchid(0x9932CC);
        constexpr ClearColor mediumOrchid(0xBA55D3);
        constexpr ClearColor purple(0x800080);
        constexpr ClearColor thistle(0xD8BFD8);
        constexpr ClearColor plum(0xDDA0DD);
        constexpr ClearColor violet(0xEE82EE);
        constexpr ClearColor magenta(0xFF00FF);
        constexpr ClearColor orchid(0xDA70D6);
        constexpr ClearColor mediumVioletRed(0xC71585);
        constexpr ClearColor paleVioletRed(0xDB7093);
        constexpr ClearColor deepPink(0xFF1493);
        constexpr ClearColor hotPink(0xFF69B4);
        constexpr ClearColor lightPink(0xFFB6C1);
        constexpr ClearColor pink(0xFFC0CB);
        constexpr ClearColor antiqueWhite(0xFAEBD7);
        constexpr ClearColor beige(0xF5F5DC);
        constexpr ClearColor bisque(0xFFE4C4);
        constexpr ClearColor blanchedAlmond(0xFFEBCD);
        constexpr ClearColor wheat(0xF5DEB3);
        constexpr ClearColor cornSilk(0xFFF8DC);
        constexpr ClearColor lemonChiffon(0xFFFACD);
        constexpr ClearColor lightGoldenRodYellow(0xFAFAD2);
        constexpr ClearColor lightYellow(0xFFFFE0);
        constexpr ClearColor saddleBrown(0x8B4513);
        constexpr ClearColor sienna(0xA0522D);
        constexpr ClearColor chocolate(0xD2691E);
        constexpr ClearColor peru(0xCD853F);
        constexpr ClearColor sandyBrown(0xF4A460);
        constexpr ClearColor burlyWood(0xDEB887);
        constexpr ClearColor tan(0xD2B48C);
        constexpr ClearColor rosyBrown(0xBC8F8F);
        constexpr ClearColor moccasin(0xFFE4B5);
        constexpr ClearColor navajoWhite(0xFFDEAD);
        constexpr ClearColor peachPuff(0xFFDAB9);
        constexpr ClearColor mistyRose(0xFFE4E1);
        constexpr ClearColor lavenderBlush(0xFFF0F5);
        constexpr ClearColor linen(0xFAF0E6);
        constexpr ClearColor oldLace(0xFDF5E6);
        constexpr ClearColor papayaWhip(0xFFEFD5);
        constexpr ClearColor seaShell(0xFFF5EE);
        constexpr ClearColor mintCream(0xF5FFFA);
        constexpr ClearColor slateGray(0x708090);
        constexpr ClearColor lightSlateGray(0x778899);
        constexpr ClearColor lightSteelBlue(0xB0C4DE);
        constexpr ClearColor lavender(0xE6E6FA);
        constexpr ClearColor floralWhite(0xFFFAF0);
        constexpr ClearColor aliceBlue(0xF0F8FF);
        constexpr ClearColor ghostWhite(0xF8F8FF);
        constexpr ClearColor honeydew(0xF0FFF0);
        constexpr ClearColor ivory(0xFFFFF0);
        constexpr ClearColor azure(0xF0FFFF);
        constexpr ClearColor snow(0xFFFAFA);
        constexpr ClearColor black(0x000000U);
        constexpr ClearColor dimGray(0x696969);
        constexpr ClearColor gray(0x808080);
        constexpr ClearColor darkGray(0xA9A9A9);
        constexpr ClearColor silver(0xC0C0C0);
        constexpr ClearColor lightGray(0xD3D3D3);
        constexpr ClearColor gainsboro(0xDCDCDC);
        constexpr ClearColor whiteSmoke(0xF5F5F5);
        constexpr ClearColor white(0xFFFFFF);

        constexpr ClearDepth depthZero(0.f);
        constexpr ClearDepth depthOne(1.f);
    } // namespace clear
} // namespace magma
