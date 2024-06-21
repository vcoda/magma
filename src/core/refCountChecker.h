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
    namespace core
    {
        /* Reference counter to check that there are no circular 
           references that prevents destruction of Vulkan objects. */

    #ifdef MAGMA_DEBUG
        class RefCountChecker final
        {
        public:
            RefCountChecker() noexcept: refCount(0ll) {}
            ~RefCountChecker()
            {
                MAGMA_ASSERT(0ll == refCount);
            }
            void addRef() noexcept { ++refCount; }
            void release() noexcept { --refCount; }
            int64_t getRefCount() const noexcept { return refCount; }

        private:
            std::atomic<int64_t> refCount;
        };
    #endif // MAGMA_DEBUG
    } // namespace core
} // namespace magma
