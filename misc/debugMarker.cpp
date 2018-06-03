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
#include "debugMarker.h"
#include "../objects/commandBuffer.h"
#include "../shared.h"

namespace magma
{
DebugMarker::DebugMarker(std::shared_ptr<CommandBuffer> commandBuffer, const char *name):
    DebugMarker(std::move(commandBuffer), name, 0.f, 0.f, 0.f, 1.f)
{}
        
DebugMarker::DebugMarker(std::shared_ptr<CommandBuffer> commandBuffer, const char *name, 
    float r, float g, float b, float a /* 1 */) :
    commandBuffer(std::move(commandBuffer))
{
#ifdef MAGMA_DEBUG
    const float color[4] = {r, g, b, a};
    this->commandBuffer->beginDebugMarker(name, color);
#endif
}

DebugMarker::DebugMarker(std::shared_ptr<CommandBuffer> commandBuffer, const char *name, const float color[4]):
    commandBuffer(std::move(commandBuffer))
{
#ifdef MAGMA_DEBUG
    this->commandBuffer->beginDebugMarker(name, color);
#endif
}

DebugMarker::~DebugMarker()
{
#ifdef MAGMA_DEBUG
    this->commandBuffer->endDebugMarker();
#endif
}
} // namespace magma
