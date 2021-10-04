/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
    /* A node in the chained input structures that
       is assigned to pNext member of Vk*CreateInfo structure. */

    class CreateInfo
    {
    public:
        CreateInfo() noexcept: hash(0) {}
        virtual const void *getNode() const noexcept { return nullptr; }
        virtual std::size_t getHash() const noexcept { return hash; }

    protected:
        std::size_t hash;
    };
} // namespace magma
