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
#include "allocable.h"
#include "../misc/exception.h"

namespace magma
{
namespace sys
{
std::shared_ptr<IObjectAllocator> Allocable::allocator;
std::atomic<int32_t> Allocable::allocCount;

void Allocable::setAllocator(std::shared_ptr<IObjectAllocator> allocator)
{
    if (allocCount)
        MAGMA_THROW("allocator should be defined only when allocation count is zero");
    allocator = std::move(allocator);
}
} // namespace sys
} // namespace magma
