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
#include "../detail/copy.h"

namespace magma
{
    struct SpecializationEntry : VkSpecializationMapEntry
    {
        SpecializationEntry() noexcept = default;
        template<typename Block, typename Type>
        explicit SpecializationEntry(uint32_t index, Type Block::*member) noexcept
        {
            constantID = index;
            const ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Block*)0)->*member));
            offset = static_cast<uint32_t>(diff);
            size = sizeof(Type);
        }
    };

    /* Specialization constants are a mechanism whereby constants in a SPIR-V module
       can have their constant value specified at the time the pipeline is created.
       This allows a SPIR-V module to have constants that can be modified while
       executing an application that uses the Vulkan API. */

    class Specialization final : public VkSpecializationInfo
    {
    public:
        template<typename Block>
        explicit Specialization(const Block& data,
            const std::initializer_list<SpecializationEntry>& entryMap) noexcept
        {
            mapEntryCount = static_cast<uint32_t>(entryMap.size());
            pMapEntries = detail::copyInitializerList(entryMap);
            dataSize = sizeof(Block);
            pData = detail::copyArray<char>(&data, dataSize);
        }
        Specialization(const Specialization&) noexcept;
        Specialization& operator=(const Specialization&) noexcept;
        ~Specialization();
        std::size_t hash() const noexcept;
    };
} // namespace magma
