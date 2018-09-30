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
#include <memory>
#include "../nonCopyable.h"

namespace magma
{
    class CommandBuffer;

    class ScopedDebugMarker final : public NonCopyable
    {
    public:
        ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *name) noexcept;
        ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *name, float r, float g, float b, float a = 1.f) noexcept;
        ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *name, const float color[4]) noexcept;
        ~ScopedDebugMarker();

    private:
        std::shared_ptr<CommandBuffer> cmdBuffer;
    };
} // namespace magma
