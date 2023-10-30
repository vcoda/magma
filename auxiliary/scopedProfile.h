/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    namespace aux
    {
        /* GPU profiling block. */

        class ScopedProfile final : NonCopyable
        {
        public:
            explicit ScopedProfile(const char *name, std::shared_ptr<CommandBuffer> cmdBuffer_,
                uint32_t color = 0xFFFFFFFF):
                cmdBuffer(std::move(cmdBuffer_)),
                profiler(Profiler::get(Profiler::Graphics))
            {
                if (profiler)
                    profiler->beginSection(name, color, cmdBuffer);
            }

            ~ScopedProfile()
            {
                if (profiler)
                    profiler->endSection(std::move(cmdBuffer));
            }

        private:
            std::shared_ptr<CommandBuffer> cmdBuffer;
            Profiler *profiler;
        };
    } // namespace aux
} // namespace magma

#ifdef MAGMA_DEBUG
    #define MAGMA_SCOPED_PROFILE(name, cmdBuffer, line) magma::aux::ScopedProfile _magma_profile_line##line(name, cmdBuffer);
    #define MAGMA_PROFILE_INDIRECT(name, cmdBuffer, line) MAGMA_SCOPED_PROFILE(name, cmdBuffer, line)

    #define MAGMA_PROFILE(cmdBuffer) MAGMA_PROFILE_INDIRECT(__FUNCTION__, cmdBuffer, __LINE__)
    #define MAGMA_PROFILE_NAME(label, cmdBuffer) MAGMA_PROFILE_INDIRECT(label, cmdBuffer, __LINE__)
#else
    #define MAGMA_PROFILE(cmdBuffer)
    #define MAGMA_PROFILE_NAME(label, cmdBuffer)
#endif
