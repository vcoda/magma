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

namespace magma
{
    class CommandBuffer;

    /* Uses VK_EXT_debug_utils extension in favor of deprecated VK_EXT_debug_marker.
       https://www.lunarg.com/wp-content/uploads/2018/05/Vulkan-Debug-Utils_05_18_v1.pdf */

    class ScopedDebugMarker final : public internal::NonCopyable
    {
    public:
        ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *name) noexcept;
        ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *name, float r, float g, float b, float a = 1.f) noexcept;
        ScopedDebugMarker(std::shared_ptr<CommandBuffer> cmdBuffer,
            const char *name, uint32_t hexColor) noexcept;
        ~ScopedDebugMarker();

    private:
        std::shared_ptr<CommandBuffer> cmdBuffer;
    };

    enum class marker : uint32_t
    {   // Alpha is 1
        blackColor = 0x000000FF,
        whiteColor = 0xFFFFFFFF,
        redColor = 0xFF0000FF,
        limeColor = 0x00FF00FF,
        blueColor = 0x0000FFFF,
        yellowColor = 0xFFFF00FF,
        cyanColor = 0x00FFFFFF,
        magentaColor = 0xFF00FFFF,
        silverColor = 0xC0C0C0FF,
        grayColor = 0x808080FF,
        maroonColor = 0x800000FF,
        oliveColor = 0x808000FF,
        greenColor = 0x008000FF,
        purpleColor = 0x800080FF,
        tealColor = 0x008080FF,
        navyColor = 0x000080FF
    };
} // namespace magma
