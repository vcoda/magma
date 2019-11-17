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
    namespace internal
    {
        /* Placement pool. */

        template<typename Type>
        class PlacementPool
        {
        public:
            PlacementPool(uint32_t maxObjects):
                pool(sizeof(Type) * maxObjects, 0),
                maxObjects(maxObjects),
                allocCount(0)
            {
            }

            void *allocate() noexcept
            {   // Linear allocation
                if (allocCount < maxObjects)
                    return pool.data() + sizeof(Type) * allocCount++;
                return nullptr;
            }

            uint32_t getAllocationCount() const noexcept
            {
                return allocCount;
            }

        private:
            std::vector<char> pool;
            const uint32_t maxObjects;
            uint32_t allocCount;
        };
    } // namespace internal
} // namespace magma

