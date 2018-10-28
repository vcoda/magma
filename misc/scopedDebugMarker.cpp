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
#include "scopedDebugMarker.h"
#include "../objects/commandBuffer.h"
#include "../utilities/hexColor.h"
#include "../shared.h"

namespace magma
{
ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name) noexcept:
    ScopedDebugMarker(std::move(cmdBuffer), name, 0.f, 0.f, 0.f, 1.f)
{}

ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name,
    float r, float g, float b, float a /* 1 */) noexcept:
    cmdBuffer(std::move(cmdBuffer))
{
#ifdef MAGMA_DEBUG
    const float color[4] = {r, g, b, a};
    this->cmdBuffer->beginDebugLabel(name, color);
#elif defined(_MSC_VER)
    cmdBuffer;
    name;
    r; g; b; a;
#endif // MAGMA_DEBUG
}

ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name, const float color[4]) noexcept:
    cmdBuffer(std::move(cmdBuffer))
{
#ifdef MAGMA_DEBUG
    this->cmdBuffer->beginDebugLabel(name, color);
#elif defined(_MSC_VER)
    cmdBuffer;
    name;
    color;
#endif // MAGMA_DEBUG
}

ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name, uint32_t hexColor) noexcept:
    cmdBuffer(std::move(cmdBuffer))
{
#ifdef MAGMA_DEBUG
    float color[4];
    utilities::hexColorToFloat4(hexColor, color);
    this->cmdBuffer->beginDebugLabel(name, color);
#elif defined(_MSC_VER)
    cmdBuffer;
    name;
    hexColor;
#endif // MAGMA_DEBUG
}

ScopedDebugMarker::~ScopedDebugMarker()
{
#ifdef MAGMA_DEBUG
    this->cmdBuffer->endDebugLabel();
#endif
}

namespace marker
{
const uint32_t blackColor(0x000000FF);
const uint32_t whiteColor(0xFFFFFFFF);
const uint32_t redColor(0xFF0000FF);
const uint32_t limeColor(0x00FF00FF);
const uint32_t blueColor(0x0000FFFF);
const uint32_t yellowColor(0xFFFF00FF);
const uint32_t cyanColor(0x00FFFFFF);
const uint32_t magentaColor(0xFF00FFFF);
const uint32_t silverColor(0xC0C0C0FF);
const uint32_t grayColor(0x808080FF);
const uint32_t maroonColor(0x800000FF);
const uint32_t oliveColor(0x808000FF);
const uint32_t greenColor(0x008000FF);
const uint32_t purpleColor(0x800080FF);
const uint32_t tealColor(0x008080FF);
const uint32_t navyColor(0x000080FF);
} // namespace marker
} // namespace magma
