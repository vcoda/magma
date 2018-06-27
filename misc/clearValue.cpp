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
#include "clearValue.h"

namespace magma
{
namespace clears
{
const ColorClear blackColorClear(0x000000U);
const ColorClear whiteColorClear(0xFFFFFF);
const ColorClear redColorClear(0xFF0000);
const ColorClear limeColorClear(0x00FF00);
const ColorClear blueColorClear(0x0000FF);
const ColorClear yellowColorClear(0xFFFF00);
const ColorClear cyanColorClear(0x00FFFF);
const ColorClear magentaColorClear(0xFF00FF);
const ColorClear silverColorClear(0xC0C0C0);
const ColorClear grayColorClear(0x808080);
const ColorClear maroonColorClear(0x800000);
const ColorClear oliveColorClear(0x808000);
const ColorClear greenColorClear(0x008000);
const ColorClear purpleColorClear(0x800080);
const ColorClear tealColorClear(0x008080);
const ColorClear navyColorClear(0x000080);

const DepthStencilClear oneDepthClear(1.f, 0x0);
const DepthStencilClear zeroDepthClear(0.f, 0x0);
} // namespace clears
} // namespace magma
