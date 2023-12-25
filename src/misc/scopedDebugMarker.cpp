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
ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name) noexcept:
    ScopedDebugMarker(std::move(cmdBuffer), name, 0.f, 0.f, 0.f, 1.f)
{}

ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name,
    float r, float g, float b, float a /* 1 */) noexcept:
    cmdBuffer(std::move(cmdBuffer))
{
#ifdef MAGMA_DEBUG_LABEL
    uint32_t color = uint32_t(a * 255.f) |
                    (uint32_t(b * 255.f) << 8) |
                    (uint32_t(g * 255.f) << 16) |
                    (uint32_t(r * 255.f) << 24);
    this->cmdBuffer->beginDebugLabel(name, color);
#else
    MAGMA_UNUSED(cmdBuffer);
    MAGMA_UNUSED(name);
    MAGMA_UNUSED(r);
    MAGMA_UNUSED(g);
    MAGMA_UNUSED(b);
    MAGMA_UNUSED(a);
#endif // MAGMA_DEBUG_LABEL
}

ScopedDebugMarker::ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer, const char *name, uint32_t color) noexcept:
    cmdBuffer(std::move(cmdBuffer))
{
#ifdef MAGMA_DEBUG_LABEL
    this->cmdBuffer->beginDebugLabel(name, color);
#else
    MAGMA_UNUSED(cmdBuffer);
    MAGMA_UNUSED(name);
    MAGMA_UNUSED(color);
#endif // MAGMA_DEBUG_LABEL
}

ScopedDebugMarker::~ScopedDebugMarker()
{
#ifdef MAGMA_DEBUG_LABEL
    this->cmdBuffer->endDebugLabel();
#endif // MAGMA_DEBUG_LABEL
}
} // namespace magma
