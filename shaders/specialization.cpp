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
#include "pch.h"
#pragma hdrstop
#include "specialization.h"

namespace magma
{
Specialization::Specialization(const Specialization& other) noexcept
{
    mapEntryCount = other.mapEntryCount;
    pMapEntries = detail::copyArray(other.pMapEntries, mapEntryCount);
    dataSize = other.dataSize;
    pData = detail::copyArray<char>(other.pData, dataSize);
}

Specialization& Specialization::operator=(const Specialization& other) noexcept
{
    if (this != &other)
    {
        mapEntryCount = other.mapEntryCount;
        delete[] pMapEntries;
        pMapEntries = detail::copyArray(other.pMapEntries, mapEntryCount);
        dataSize = other.dataSize;
        delete[] reinterpret_cast<const char *>(pData);
        pData = detail::copyArray<char>(other.pData, dataSize);
    }
    return *this;
}

Specialization::~Specialization()
{
    delete[] pMapEntries;
    delete[] reinterpret_cast<const char *>(pData);
}

std::size_t Specialization::hash() const noexcept
{
    std::size_t hash = 0;
    for (uint32_t i = 0; i < mapEntryCount; ++i)
    {
        detail::hashCombine(hash, detail::hashArgs(
            pMapEntries[i].constantID,
            pMapEntries[i].offset,
            pMapEntries[i].size));
    }
    detail::hashCombine(hash, detail::hashArray(
        reinterpret_cast<const uint8_t *>(pData), dataSize));
    return hash;
}
} // namespace magma
