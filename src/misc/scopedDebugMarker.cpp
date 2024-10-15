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
#include "pch.h"
#pragma hdrstop
#include "scopedDebugMarker.h"
#include "../objects/commandBuffer.h"

namespace magma
{
#if defined(VK_EXT_debug_marker) || defined(VK_EXT_debug_utils)
ScopedDebugMarker::ScopedDebugMarker(const std::unique_ptr<CommandBuffer>& cmdBuffer,
    const char *name, float r, float g, float b, float a /* 1 */) noexcept:
    cmdBuffer(cmdBuffer)
{
    const uint32_t color = uint32_t(a * 255.f) |
        (uint32_t(b * 255.f) << 8) |
        (uint32_t(g * 255.f) << 16) |
        (uint32_t(r * 255.f) << 24);
#ifdef VK_EXT_debug_utils
    cmdBuffer->beginDebugLabel(name, color);
#else
    cmdBuffer->beginDebugMarker(name, color);
#endif
}

ScopedDebugMarker::ScopedDebugMarker(const std::unique_ptr<CommandBuffer>& cmdBuffer,
    const char *name, uint32_t color /* 0x0 */) noexcept:
    cmdBuffer(cmdBuffer)
{
#ifdef VK_EXT_debug_utils
    cmdBuffer->beginDebugLabel(name, color);
#else
    cmdBuffer->beginDebugMarker(name, color);
#endif
}

ScopedDebugMarker::~ScopedDebugMarker()
{
#ifdef VK_EXT_debug_utils
    cmdBuffer->endDebugLabel();
#else
    cmdBuffer->endDebugMarker();
#endif
}
#endif // VK_EXT_debug_marker || VK_EXT_debug_utils
} // namespace magma
