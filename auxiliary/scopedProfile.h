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
        /* Helper class to call begin/end within scope. */

        class ScopedProfile final : core::NonCopyable
        {
        public:
            explicit ScopedProfile(const char *name, uint32_t color, std::shared_ptr<Profiler> profiler_, std::shared_ptr<CommandBuffer> cmdBuffer_):
                profiler(std::move(profiler_)),
                cmdBuffer(std::move(cmdBuffer_))
            {
                profiler->beginSection(name, color, cmdBuffer);
            }

            ~ScopedProfile()
            {
                profiler->endSection(std::move(cmdBuffer));
            }

        private:
            std::shared_ptr<Profiler> profiler;
            std::shared_ptr<CommandBuffer> cmdBuffer;
        };
    } // namespace aux
} // namespace magma

#ifdef MAGMA_DEBUG
#define MAGMA_PROFILE(name, color, profiler, cmdBuffer) magma::aux::ScopedProfile _magma_profile_##name(MAGMA_STRINGIZE(name), color, profiler, cmdBuffer)
#else
#define MAGMA_PROFILE(name, color, profiler, cmdBuffer)
#endif
