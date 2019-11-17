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
#include "../allocator/objectAllocator.h"

namespace magma
{
    namespace internal
    {
        /* Placement pool. */

        template<typename Type>
        class PlacementPool : public IObjectAllocator
        {
        public:
            PlacementPool(uint32_t maxObjectCount):
                pool(sizeof(Type) * maxObjectCount, 0),
                maxObjectCount(maxObjectCount),
                allocCount(0)
            {}

            void *alloc(size_t size) override
            {
                if (size != sizeof(Type))
                    return nullptr;
                // Linear allocation
                if (allocCount < maxObjectCount)
                    return pool.data() + sizeof(Type) * allocCount++;
                return nullptr;
            }

            void free(void *p) noexcept override
            {
                if ((p >= pool.data()) &&
                    (p <= pool.data() + sizeof(Type) * (maxObjectCount - 1)))
                {
                    // Not yet implemented
                }
            }

            size_t getBytesAllocated() const noexcept override
            {
                return sizeof(Type) * allocCount;
            }

        private:
            std::vector<char> pool;
            const uint32_t maxObjectCount;
            uint32_t allocCount;
        };
    } // namespace internal
} // namespace magma

