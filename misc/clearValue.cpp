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
const ClearColor blackColor(0x000000U);
const ClearColor whiteColor(0xFFFFFF);
const ClearColor redColor(0xFF0000);
const ClearColor limeColor(0x00FF00);
const ClearColor blueColor(0x0000FF);
const ClearColor yellowColor(0xFFFF00);
const ClearColor cyanColor(0x00FFFF);
const ClearColor magentaColor(0xFF00FF);
const ClearColor silverColor(0xC0C0C0);
const ClearColor grayColor(0x808080);
const ClearColor maroonColor(0x800000);
const ClearColor oliveColor(0x808000);
const ClearColor greenColor(0x008000);
const ClearColor purpleColor(0x800080);
const ClearColor tealColor(0x008080);
const ClearColor navyColor(0x000080);

const ClearDepthStencil depthOne(1.f, 0x0);
const ClearDepthStencil depthZero(0.f, 0x0);
} // namespace clears
} // namespace magma
