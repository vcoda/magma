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
    namespace detail
    {
        /* Placement pool. */

        class PlacementPool : public IObjectAllocator
        {
        public:
            PlacementPool(std::size_t typeSize, uint32_t maxObjectCount):
                pool(typeSize * maxObjectCount, 0),
                typeSize(typeSize),
                maxObjectCount(maxObjectCount),
                allocCount(0)
            {}

            template<typename Type, typename... Types>
            Type *placementNew(Types&&... args)
            {
                assert(sizeof(Type) == typeSize);
                void *const placement = alloc(sizeof(Type));
                if (placement)
                    return new (placement) Type(std::forward<Types &&>(args)...);
                return nullptr;
            }

            void *alloc(std::size_t size) override
            {   // Linear incremental allocation
                if (allocCount < maxObjectCount)
                    return pool.data() + size * allocCount++;
                return nullptr;
            }

            void free(void *p) noexcept override
            {
                if ((p >= pool.data()) &&
                    (p <= pool.data() + typeSize * (maxObjectCount - 1)))
                {
                    // Not yet implemented
                }
            }

            std::size_t getBytesAllocated() const noexcept override
            {
                return typeSize * allocCount;
            }

        private:
            std::vector<char> pool;
            const std::size_t typeSize;
            const uint32_t maxObjectCount;
            uint32_t allocCount;
        };
    } // namespace detail
} // namespace magma

