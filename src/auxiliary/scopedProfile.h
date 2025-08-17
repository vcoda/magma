/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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

namespace magma::aux
{
    /* GPU profiling block. */

    class ScopedProfile final : NonCopyable
    {
    public:
        explicit ScopedProfile(const char *name, lent_ptr<CommandBuffer> cmdBuffer,
            uint32_t color = 0xFFFFFFFF):
            cmdBuffer(cmdBuffer.get()),
            profiler(Profiler::get(Profiler::Queue::Graphics))
        {
            if (profiler)
                profiler->beginSection(std::move(cmdBuffer), name, color);
        }

        ~ScopedProfile()
        {
            if (profiler)
                profiler->endSection(cmdBuffer);
        }

    private:
        CommandBuffer *cmdBuffer;
        Profiler *profiler;
    };
} // namespace magma::aux

#define MAGMA_SCOPED_PROFILE(name, cmdBuffer, id) magma::aux::ScopedProfile _magma_profile_##id(name, cmdBuffer);

#ifdef MAGMA_DEBUG
    #define MAGMA_PROFILE(name, cmdBuffer) MAGMA_SCOPED_PROFILE(name, cmdBuffer, MAGMA_COUNTER)
    #define MAGMA_PROFILE_BEGIN(name, cmdBuffer) {\
        MAGMA_PROFILE(name, cmdBuffer)
    #define MAGMA_PROFILE_END }
#else
    #define MAGMA_PROFILE(name, cmdBuffer)
    #define MAGMA_PROFILE_BEGIN(name, cmdBuffer) {
    #define MAGMA_PROFILE_END }
#endif // MAGMA_DEBUG
