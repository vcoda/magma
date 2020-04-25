/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
    namespace mem
    {
        /* Linear placement allocator. */

        class LinearPlacementPool : public IObjectAllocator
        {
        public:
            LinearPlacementPool(const std::size_t sizeofType, uint32_t maxObjectCount):
                pool(sizeofType * maxObjectCount, 0),
                sizeofType(sizeofType),
                maxObjectCount(maxObjectCount),
                allocCount(0)
            {}

            template<typename Type, typename... Types>
            Type *placementNew(Types&&... args)
            {
                MAGMA_ASSERT(sizeof(Type) == sizeofType);
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
                    (p <= pool.data() + sizeofType * (maxObjectCount - 1)))
                {
                    // Not yet implemented
                }
            }

            std::size_t getBytesAllocated() const noexcept override
            {
                return sizeofType * allocCount;
            }

        private:
            std::vector<char> pool;
            const std::size_t sizeofType;
            const uint32_t maxObjectCount;
            uint32_t allocCount;
        };
    } // namespace mem
} // namespace magma

