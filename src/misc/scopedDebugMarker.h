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
    class CommandBuffer;

    /* Favors VK_EXT_debug_utils extension over deprecated VK_EXT_debug_marker.
       https://www.lunarg.com/wp-content/uploads/2018/05/Vulkan-Debug-Utils_05_18_v1.pdf */

#if defined(VK_EXT_debug_marker) || defined(VK_EXT_debug_utils)
    class ScopedDebugMarker final : NonCopyable
    {
    public:
        explicit ScopedDebugMarker(const std::unique_ptr<CommandBuffer>& cmdBuffer,
            const char *name,
            float r, float g, float b, float a = 1.f) noexcept;
        explicit ScopedDebugMarker(const std::unique_ptr<CommandBuffer>& cmdBuffer,
            const char *name,
            uint32_t color = 0x0) noexcept;
        ~ScopedDebugMarker();

    private:
        const std::unique_ptr<CommandBuffer>& cmdBuffer;
    };
#endif // VK_EXT_debug_marker || VK_EXT_debug_utils

    // Alpha is 1
    namespace Color
    {
        enum {
        Black = 0x000000FFU,
        White = 0xFFFFFFFFU,
        Red = 0xFF0000FFU,
        Lime = 0x00FF00FFU,
        Blue = 0x0000FFFFU,
        Yellow = 0xFFFF00FFU,
        Cyan = 0x00FFFFFFU,
        Magenta = 0xFF00FFFFU,
        Silver = 0xC0C0C0FFU,
        Gray = 0x808080FFU,
        Maroon = 0x800000FFU,
        Olive = 0x808000FFU,
        Green = 0x008000FFU,
        Purple = 0x800080FFU,
        Teal = 0x008080FFU,
        Navy = 0x000080FFU};
    };
} // namespace magma

#if defined(VK_EXT_debug_marker) || defined(VK_EXT_debug_utils)
#define MAGMA_SCOPED_DEBUG_MARKER(name, cmdBuffer, line) magma::ScopedDebugMarker _magma_debug_marker_##line(cmdBuffer, name);
#else
#define MAGMA_SCOPED_DEBUG_MARKER(name, cmdBuffer, line)
#endif // VK_EXT_debug_marker || VK_EXT_debug_utils

#ifdef MAGMA_DEBUG
    #define MAGMA_DEBUG_MARKER_INDIRECT(name, cmdBuffer, line) MAGMA_SCOPED_DEBUG_MARKER(name, cmdBuffer, line)
    #define MAGMA_DEBUG_MARKER(name, cmdBuffer) MAGMA_DEBUG_MARKER_INDIRECT(name, cmdBuffer, __LINE__)
    #define MAGMA_DEBUG_MARKER_BEGIN(name, cmdBuffer) {\
        MAGMA_DEBUG_MARKER(name, cmdBuffer)
    #define MAGMA_DEBUG_MARKER_END }
#else
    #define MAGMA_DEBUG_MARKER(name, cmdBuffer)
    #define MAGMA_DEBUG_MARKER_BEGIN(name, cmdBuffer) {
    #define MAGMA_DEBUG_MARKER_END }
#endif // MAGMA_DEBUG
